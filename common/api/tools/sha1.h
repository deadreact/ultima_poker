#pragma once

#include <string>
#include <boost/uuid/sha1.hpp>

namespace gbp
{
 namespace ns_crypt
 {

  inline std::string to_sha1(const std::string& str)
  {
   boost::uuids::detail::sha1 sha1;

   auto sha1_to_string = [](char (&hash)[20]) -> std::string {
    char str[128] = {0};
    char *ptr = str;
    char *hptr = hash;
    std::string ret;

    for (int i = 0; i < 20; i++)
    {
     sprintf(ptr, "%02x", (unsigned char)*hptr);
     ptr += 2;
     ++hptr;
    }
    ret = str;

    return ret;
   };

   sha1.process_bytes(str.c_str(), str.length());
   unsigned int digest[5] = {0};
   sha1.get_digest(digest);

   char bin[sizeof (digest)];
   const char* tmp = reinterpret_cast<char*>(&digest[0]);
   for (size_t i = 0; i < sizeof (digest) / sizeof (digest[0]); ++i)
   {
    bin[i * 4 ] = tmp[i * 4 + 3];
    bin[i * 4 + 1] = tmp[i * 4 + 2];
    bin[i * 4 + 2] = tmp[i * 4 + 1];
    bin[i * 4 + 3] = tmp[i * 4 ];
   }

   return sha1_to_string(bin);
  }

 } // ns_crypt
} // ns gbp
