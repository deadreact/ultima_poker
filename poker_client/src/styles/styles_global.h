#pragma once

#include <QtCore/qglobal.h>

#if defined(STATIC_LIB) || defined(NO_DLL_IMPORT)
    #define STYLES_DLL
#else
    #if defined(STYLES_LIB)
        #define STYLES_DLL Q_DECL_EXPORT
    #else
        #define STYLES_DLL Q_DECL_IMPORT
    #endif
#endif // STATIC_LIB
