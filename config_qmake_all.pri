# общие параметры для каждого проекта
CONFIG += c++17
CONFIG += static_build
DEFINES += GBP_DECLARE_TYPE_GEN_ADDITIONALS
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -Wall -Wextra -Winvalid-pch
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-unused-variable -Wno-sign-compare
