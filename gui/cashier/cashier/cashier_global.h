#pragma once

#include <QtCore/qglobal.h>

#if defined(STATIC_LIB) || defined (NO_DLL_IMPORT)
#define CASHIER_DLL
#else
    #if defined(CASHIER_LIB)
        #define CASHIER_DLL Q_DECL_EXPORT
    #else
        #define CASHIER_DLL Q_DECL_IMPORT
    #endif
#endif //STATIC_LIB
