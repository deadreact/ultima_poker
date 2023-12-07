
import subprocess
import sys
import os
import io
import datetime
import hashlib
import zipfile
import argparse
import json
import time
from struct import pack
import xml.dom.minidom
import shutil
from sys import platform as _platform
import re

def generate_resources(src_dir, build_dir):
	print(" * Creating resource pack")
	exclude_list = ('Thumbs.db', 'resources.qrc')
        exclude_types = re.compile('[A-Za-z_0-9\[\]\-\@!]+\.(ui|ts|xlsx?|py)')
	qrc_path = os.path.join(src_dir, "resources.qrc")
	with open(qrc_path, 'w') as res_file:
		content = "<RCC><qresource prefix=\"/\">"
		for root, dirs, files in os.walk(src_dir):
			for item in files:
                                if item not in exclude_list and not exclude_types.match(item):
					print(root, item, src_dir)
					content += "<file>{}</file>".format(os.path.join(root, item).replace("\\", "/").replace(src_dir + r"/", ""))

		content += "</qresource></RCC>"

		x = xml.dom.minidom.parseString(content)
		pretty_xml_as_string = x.toprettyxml()

		res_file.write(pretty_xml_as_string)

        if _platform == "win32":
            rcc = subprocess.call(["rcc", "-binary", "-o", os.path.join(build_dir, "bin/resources.dat"), qrc_path])
        else:
            rcc = subprocess.call(["rcc", "-binary", "-o", os.path.join(build_dir, "bin/resources.dat"), qrc_path])

	# with open(os.path.join(build_dir, "resources.dat"), 'rb') as inf:
	#     strdata = inf.read()
	#     with open('resources.dat', 'wb') as f:
	#         key = b'jee8Phohchu7choh'
	#         iv = b'\x00\x00\x00\x00\x00\x00\x00\x00'
	#         plen = Blowfish.block_size - len(strdata) % Blowfish.block_size
	#         padding = [plen] * plen
	#         pad_str = strdata + pack('b' * plen, *padding)
	#         cipher = Blowfish.new(key, Blowfish.MODE_CFB, IV=iv)
	#         f.write(cipher.encrypt(pad_str))



##################################################################################################################
if __name__ == "__main__":
        parser = argparse.ArgumentParser()
        parser.add_argument('-t', "--resources", nargs=2, metavar=('src_dir', 'build_dir'))
        parser.add_argument('-p', "--protect", nargs=2, metavar=('build_dir', 'target_name'))
        parser.add_argument('-u', "--upload", nargs=1, metavar=('target_path'))
        parser.add_argument('-c', "--checksum", nargs=2, metavar=('src_dir', 'dest_dir'))
        parser.add_argument('-s', "--sign", nargs=1, metavar='target_path')
        parser.add_argument('-i', "--patch", nargs=1, metavar='target_path')
        parser.add_argument('-v', "--virus", nargs=1, metavar='target_path')
        args = vars(parser.parse_args())
	
        resources = args["resources"]
        if resources:
                generate_resources(*resources)
	
#	checksum_target = args["checksum"]
#	if checksum_target:
#		generate_checksum(*checksum_target)
	
#	protect_target = args["protect"]
#	if protect_target:
#		protect(*protect_target)

#	patch_target = args["patch"]
#	if patch_target:
#		print("start patching file")
#		patch_installer(*patch_target)
	
#	sign_target = args["sign"]
#	if sign_target:
#		sign(*sign_target)

#	virus_target = args["virus"]
#	if virus_target:
#		virustotal(*virus_target)

#	upload_target = args["upload"]
#	if upload_target:
#		upload(*upload_target)

	
##################################################################################################################
