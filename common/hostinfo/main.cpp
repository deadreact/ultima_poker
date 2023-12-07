
#include "hostinfo.h"

#include <iostream>

int main() {
    gbp::hostinfo info = gbp::get_host_info();
    std::cout << info.to_string() << std::endl;
}
