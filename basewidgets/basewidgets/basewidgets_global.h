#pragma once

#include <QtCore/qglobal.h>

#if defined(STATIC_LIB) || defined(NO_DLL_IMPORT)
    #define BASEWIDGETS_DLL
#else
    #if defined(BASEWIDGETS_LIB)
        #define BASEWIDGETS_DLL Q_DECL_EXPORT
    #else
        #define BASEWIDGETS_DLL Q_DECL_IMPORT
    #endif
#endif //STATIC_LIB
