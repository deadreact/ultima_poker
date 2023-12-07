#include <boost/asio.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cassert>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <lmcons.h>
#include <lm.h>
#include <winsock2.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>

namespace gbp {

std::string GetWindowsVersionString() {
    std::string     winver;
    OSVERSIONINFOEX osver;
    SYSTEM_INFO     sysInfo;
    typedef void(__stdcall *GETSYSTEMINFO) (LPSYSTEM_INFO);
    LPBYTE pinfoRawData = 0;

    memset(&osver, 0, sizeof(osver));
    osver.dwOSVersionInfoSize = sizeof(osver);
    GetVersionEx((LPOSVERSIONINFO)&osver);

    DWORD major = 0;
    DWORD minor = 0;
    if (NERR_Success == NetWkstaGetInfo(NULL, 100, &pinfoRawData)) {
        WKSTA_INFO_100* pworkstationInfo = (WKSTA_INFO_100*)pinfoRawData;
        osver.dwMajorVersion = pworkstationInfo->wki100_ver_major;
        osver.dwMinorVersion = pworkstationInfo->wki100_ver_minor;
        ::NetApiBufferFree(pinfoRawData);
    } else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2) {
        OSVERSIONINFOEXW osvi;
        ULONGLONG cm = 0;
        cm = VerSetConditionMask(cm, VER_MINORVERSION, VER_EQUAL);
        ZeroMemory(&osvi, sizeof(osvi));
        osvi.dwOSVersionInfoSize = sizeof(osvi);
        osvi.dwMinorVersion = 3;
        if (VerifyVersionInfoW(&osvi, VER_MINORVERSION, cm)) {
            osver.dwMinorVersion = 3;
        }
    }

    GETSYSTEMINFO getSysInfo = (GETSYSTEMINFO)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetNativeSystemInfo");
    if (getSysInfo == NULL) {
        getSysInfo = ::GetSystemInfo;
    }
	
    getSysInfo(&sysInfo);

    if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType != VER_NT_WORKSTATION)
        winver = "Windows 10 Server";
    if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType == VER_NT_WORKSTATION)
        winver = "Windows 10";
    if (osver.dwMajorVersion == 6  && osver.dwMinorVersion == 3 && osver.wProductType != VER_NT_WORKSTATION)
        winver = "Windows Server 2012 R2";
    if (osver.dwMajorVersion == 6  && osver.dwMinorVersion == 3 && osver.wProductType == VER_NT_WORKSTATION)
        winver = "Windows 8.1";
    if (osver.dwMajorVersion == 6  && osver.dwMinorVersion == 2 && osver.wProductType != VER_NT_WORKSTATION)
        winver = "Windows Server 2012";
    if (osver.dwMajorVersion == 6  && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION)
        winver = "Windows 8";
    if (osver.dwMajorVersion == 6  && osver.dwMinorVersion == 1 && osver.wProductType != VER_NT_WORKSTATION)
        winver = "Windows Server 2008 R2";
    if (osver.dwMajorVersion == 6  && osver.dwMinorVersion == 1 && osver.wProductType == VER_NT_WORKSTATION)
        winver = "Windows 7";
    if (osver.dwMajorVersion == 6  && osver.dwMinorVersion == 0 && osver.wProductType != VER_NT_WORKSTATION)
        winver = "Windows Server 2008";
    if (osver.dwMajorVersion == 6  && osver.dwMinorVersion == 0 && osver.wProductType == VER_NT_WORKSTATION)
        winver = "Windows Vista";
    if (osver.dwMajorVersion == 5  && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION && sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
        winver = "Windows XP x64";
    if (osver.dwMajorVersion == 5  && osver.dwMinorVersion == 2)
        winver = "Windows Server 2003";
    if (osver.dwMajorVersion == 5  && osver.dwMinorVersion == 1)
        winver = "Windows XP";
    if (osver.dwMajorVersion == 5  && osver.dwMinorVersion == 0)
        winver = "Windows 2000";
    if (osver.dwMajorVersion < 5)
        winver = "unknown";

    if (osver.wServicePackMajor != 0) {
        std::ostringstream os;
        os << " Service Pack " << osver.wServicePackMajor;
        winver += os.str();
    }

    return winver;
}

/**************************************************************************/

std::string wide_to_utf8(const wchar_t *str, DWORD size) {
	char *buf = static_cast<char*>(::malloc(size*4));
	assert(buf);
	memset(buf, 0, size*4);
	
	::SetLastError(0);
	DWORD r = ::WideCharToMultiByte(
		 CP_UTF8
		,0
		,str
		,size
		,buf
		,size*4
		,NULL
		,NULL
	);
	DWORD le = ::GetLastError();
	//std::cout << "WideCharToMultiByte: r=" << r << ", le=" << le << std::endl;
	assert(r);
	
	std::string res(buf);
	
	::free(buf);
	
	return res;
}

/**************************************************************************/

std::string get_osname() {
    return GetWindowsVersionString();
}

/**************************************************************************/

std::string get_username() {
	wchar_t username[UNLEN+1];
	memset(username, 0, sizeof(username));
	
	DWORD len = UNLEN;
	DWORD r = ::GetUserNameW(username, &len);
	if ( !r ) {
        return "unknown-user-name";
	}

	return wide_to_utf8(username, len);
}

/**************************************************************************/

std::string get_hostname() {
	wchar_t hostname[MAX_COMPUTERNAME_LENGTH+1];
	memset(hostname, 0, sizeof(hostname));
	
	DWORD len = MAX_COMPUTERNAME_LENGTH;
	DWORD r = ::GetComputerNameW(hostname, &len);
	if ( !r ) {
        return "unknown-host-name";
	}
	
	return wide_to_utf8(hostname, len);
}

/**************************************************************************/

std::vector<std::string>
get_disk_drives() {
	enum { buf_size = 1024 };
	wchar_t buf[buf_size+1];
	std::vector<std::string> v;

	DWORD r = ::GetLogicalDriveStringsW(0, NULL);
	if ( r > buf_size ) {
		v.emplace_back("error");
		
		return v;
	}
	
	r = ::GetLogicalDriveStringsW(buf_size, buf);
	if ( r > buf_size ) {
		v.emplace_back("error");
		
		return v;
	}

	wchar_t *p = buf;
	while ( *p ) {
		UINT dt = ::GetDriveTypeW(p);
//		std::cout << "dt=" << dt << std::endl;
		std::string s = wide_to_utf8(p, wcslen(p));
		s.pop_back();
		
		switch ( dt ) {
			case DRIVE_FIXED: s += ", DT-HDD"; break;
			case DRIVE_CDROM: s += ", DT-CDROM"; break;
			case DRIVE_REMOTE:s += ", DT-REMOTE"; break;
			default: s += ", DT-UNKNOWN"; break;
		}
		
		if ( dt == DRIVE_FIXED || dt == DRIVE_CDROM || dt == DRIVE_REMOTE ) {
			wchar_t volbuf[MAX_PATH+1] = {0};
			wchar_t fstypebuf[MAX_PATH+1] = {0};
			DWORD sernum=0;
			
			::SetLastError(0);
			DWORD r = ::GetVolumeInformationW(
				 p
				,volbuf
				,MAX_PATH
				,&sernum
				,NULL
				,NULL
				,fstypebuf
				,MAX_PATH
			);
			DWORD le = ::GetLastError();
			//std::cout << "GetVolumeInformation: r=" << r << ", le=" << le << std::endl;
			
			if ( r ) {
				if ( wcslen(volbuf) == 0 ) {
					s += ", VOLNAME-UNKNOWN";
				} else {
					s += ", VOLNAME-" + wide_to_utf8(volbuf, wcslen(volbuf));
				}
				
				if ( wcslen(fstypebuf) == 0 ) {
					s += ", FSTYPE-UNKNOWN";
				} else {
					s += ", FSTYPE-" + wide_to_utf8(fstypebuf, wcslen(fstypebuf));
				}
				
				s += ", VOLSN-" + std::to_string(sernum);
			}
		}

		v.push_back(std::move(s));
		
		p += wcslen(p)+1;
	}
	
	return v;
}

/**************************************************************************/

#pragma pack(push) 
#pragma pack(1)
typedef struct _RawSMBIOSData
{
	BYTE	Used20CallingMethod;
	BYTE	SMBIOSMajorVersion;
	BYTE	SMBIOSMinorVersion;
	BYTE	DmiRevision;
	DWORD	Length;
	PBYTE	SMBIOSTableData;
} RawSMBIOSData, *PRawSMBIOSData;

typedef struct _SMBIOSHEADER_
{
	BYTE Type;
	BYTE Length;
	WORD Handle;
} SMBIOSHEADER, *PSMBIOSHEADER;

typedef struct _TYPE_0_ {
	SMBIOSHEADER	Header;
	UCHAR	Vendor;
	UCHAR	Version;
	UINT16	StartingAddrSeg;
	UCHAR	ReleaseDate;
	UCHAR	ROMSize;
	ULONG64 Characteristics;
	UCHAR	Extension[2]; // spec. 2.3
	UCHAR	MajorRelease;
	UCHAR	MinorRelease;
	UCHAR	ECFirmwareMajor;
	UCHAR	ECFirmwareMinor;
} BIOSInfo, *PBIOSInfo;

typedef struct _TYPE_2_ {
	SMBIOSHEADER	Header;
	UCHAR	Manufacturer;
	UCHAR	Product;
	UCHAR	Version;
	UCHAR	SN;
	UCHAR	AssetTag;
	UCHAR	FeatureFlags;
	UCHAR	LocationInChassis;
	UINT16	ChassisHandle;
	UCHAR	Type;
	UCHAR	NumObjHandle;
	UINT16	*pObjHandle;
} BoardInfo, *PBoardInfo;

typedef struct _TYPE_4_ {
	SMBIOSHEADER Header;
	UCHAR	SocketDesignation;
	UCHAR	Type;
	UCHAR	Family;
	UCHAR	Manufacturer;
	ULONG64 ID;
	UCHAR	Version;
	UCHAR	Voltage;
	UINT16	ExtClock;
	UINT16	MaxSpeed;
	UINT16	CurrentSpeed;
	// Todo, Here
} ProcessorInfo, *PProcessorInfo;

#pragma pack(push) 

enum {
	 smbios_bios_info   = 0
	,smbios_board_info  = 2
	,smbios_cpu_info    = 4
};

const char* locate_string(const char* str, std::size_t i) {
	if (!i || !*str)
		return "Null String";

	while (--i) {
		str += std::strlen((char*)str) + 1;
	}
	
	return str;
}

const char* to_point_string(const void *p) {
	return (char*)p + ((PSMBIOSHEADER)p)->Length;
}

std::string get_smbios_info_bios(const void *ptr) {
	PBIOSInfo pBIOS = (PBIOSInfo)ptr;
	const char *str = to_point_string(ptr);
	
	std::string res;
	res += locate_string(str, pBIOS->Vendor);
	res += " ";
	res += locate_string(str, pBIOS->Version);
	
	return res;
}

std::string get_smbios_info_board(const void *ptr) {
	PBoardInfo pBoard = (PBoardInfo)ptr;
	const char *str = to_point_string(ptr);
	
	std::string res;
	res += locate_string(str, pBoard->Manufacturer);
	res += " ";
	res += locate_string(str, pBoard->Product);
	
	return res;
}

std::string get_smbios_info_cpu(const void *ptr) {
	PProcessorInfo	pProcessor = (PProcessorInfo)ptr;
	const char *str = to_point_string(ptr);

	std::string res = locate_string(str, pProcessor->Version);
	
	return res;
}

std::string get_smbios_info(std::size_t id) {
	std::string res;
	static const unsigned char signature_str[4] = {'B', 'M', 'S', 'R'};
	DWORD Signature=0;
	memcpy(&Signature, signature_str, 4);

	DWORD needBufferSize = ::GetSystemFirmwareTable(Signature, 0, NULL, 0);
	LPBYTE pBuff = (LPBYTE)::malloc(needBufferSize);
	if ( pBuff ) {
		::GetSystemFirmwareTable(Signature, 0, pBuff, needBufferSize);
		const PRawSMBIOSData pDMIData = (PRawSMBIOSData)pBuff;
		
		LPBYTE p = (LPBYTE)&(pDMIData->SMBIOSTableData);
		const DWORD lastAddress = reinterpret_cast<size_t>((void*)p) + pDMIData->Length;

		for (;;) {
			PSMBIOSHEADER pHeader = (PSMBIOSHEADER)p;
			
			if ( pHeader->Type == id ) {
				switch ( pHeader->Type ) {
					case smbios_bios_info: res = get_smbios_info_bios(pHeader); break;
					case smbios_board_info: res = get_smbios_info_board(pHeader); break;
					case smbios_cpu_info: res = get_smbios_info_cpu(pHeader); break;
					default: break;
				}
			}
			
			PBYTE nt = p + pHeader->Length; // point to struct end
			while (0 != (*nt | *(nt + 1))) nt++; // skip string area
			nt += 2;
			if (reinterpret_cast<size_t>(nt) >= lastAddress)
				break;
			p = nt;
		}

		::free(pBuff);
	}
	
	return res;
}

/**************************************************************************/

std::string get_cpu() {
	std::string res = get_smbios_info(smbios_cpu_info);
	if ( !res.empty() ) {
		return res;
	}
	
	return "UNKNOWN";
}

/**************************************************************************/

std::string get_mb() {
	std::string res = get_smbios_info(smbios_board_info);
	if ( !res.empty() ) {
		return res;
	}
	
	return "UNKNOWN";
}

std::string get_mac_addr() {
	std::string res = "UNKNOWN";

    WSAData wsaData;
    in_addr *ip = NULL;
    char hostname[128];
    if (::WSAStartup(0x101, &wsaData) == 0) {
        ::gethostname(hostname, 128);
        struct ::hostent *h = ::gethostbyname(hostname);
        ip = (reinterpret_cast<in_addr *>(*(h->h_addr_list)));
    }
    ::WSACleanup();
    
	if ( !ip ) {
		return res;
	}
	
//    printf("IP adress : %d.%d.%d.%d\n", ip->S_un.S_un_b.s_b1, ip->S_un.S_un_b.s_b2, ip->S_un.S_un_b.s_b3, ip->S_un.S_un_b.s_b4);

    DWORD dwRetVal = 0;
    PMIB_IPADDRTABLE pIPAddrTable = NULL;
    DWORD dwSize = 0;

	if ( ::GetIpAddrTable(NULL, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER ) {
		pIPAddrTable = (PMIB_IPADDRTABLE)::malloc(dwSize);
		if ( !pIPAddrTable ) {
			printf("can't allocate memory");
			return res;
		}
	}
	
    if ((dwRetVal = ::GetIpAddrTable(pIPAddrTable, &dwSize, 0)) != NO_ERROR) {
        printf("GetIpAddrTable failed with error %d\n", (int)dwRetVal);
        return res;
    }

    DWORD index = 0;
    for (int i = 0; i < (int)pIPAddrTable->dwNumEntries; i++) {
        if (ip->S_un.S_addr == pIPAddrTable->table[i].dwAddr)
            index = pIPAddrTable->table[i].dwIndex;
    }
	::free(pIPAddrTable);

    ULONG adapter_info_size = 0;
    PIP_ADAPTER_INFO ptr_adapter_info = NULL;
    PIP_ADAPTER_INFO ptr_adapter_info_first = NULL;

    ::GetAdaptersInfo(NULL, &adapter_info_size);

    ptr_adapter_info_first = ptr_adapter_info = (PIP_ADAPTER_INFO)::malloc(adapter_info_size);

    if ( ::GetAdaptersInfo(ptr_adapter_info, &adapter_info_size) != ERROR_SUCCESS ) {
        printf("Error while GetAdaptersInfo\n");
        ::free(ptr_adapter_info);
        return res;
    }

	char buf[128] = "\0";
    while (ptr_adapter_info) {
        if (ptr_adapter_info->Index == index) {
			char *bptr = buf;
            for (unsigned i = 0; i < ptr_adapter_info->AddressLength; ++i) {
				int s = std::snprintf(
					 bptr, 4
					,"%02x%s"
					,(unsigned char)ptr_adapter_info->Address[i], (i+1 < ptr_adapter_info->AddressLength ? ":" : "")
				);
				bptr += s;
			}
            break;

        }
        ptr_adapter_info = ptr_adapter_info->Next;

    }
    ::free(ptr_adapter_info_first);
	
	res = buf;
	return res;
}

/**************************************************************************/

std::string get_ip() {
    boost::asio::ip::tcp::iostream stream;
    stream.expires_from_now(boost::posix_time::seconds(2));
    stream.connect("ipinfo.io", "http");
    stream << "GET /ip HTTP/1.0\r\n";
    stream << "Host: ipinfo.io\r\n";
    stream << "Accept: */*\r\n";
    stream << "Connection: close\r\n\r\n";
    stream.flush();

    std::string res;
    while( std::getline(stream, res) ) {
        if ( !res.empty() && std::isdigit(res[0]) )
            break;
    }

    return res;
}

/**************************************************************************/

} // ns gbp
