#pragma once

#include <iostream>
#include <stdio.h>

namespace gbp
{
 namespace ns_filesystem
 {
  inline void file_append(const std::string& filename, const std::string& info)
  {
   FILE* pFile = fopen(filename.c_str(), "a+");
   if (pFile != NULL)
   {
    std::ostringstream os;
    os << info << std::endl;
    fputs(os.str().c_str(), pFile);
    fclose(pFile);
   }
  }
 } // ns_filesystem
} // ns gbp