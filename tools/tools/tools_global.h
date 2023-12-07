#ifndef TOOLS_GLOBAL_H
#define TOOLS_GLOBAL_H

#include <QtCore/qglobal.h>
#if defined(STATIC_LIB) || defined(NO_DLL_IMPORT)
    #define TOOLS_DLL
#else
    #if defined(TOOLS_LIB)
        #define TOOLS_DLL Q_DECL_EXPORT
    #else
        #define TOOLS_DLL Q_DECL_IMPORT
    #endif
#endif //STATIC_LIB

#ifdef Q_OS_WIN
    #define FUNCTION_TOOLS_DLL TOOLS_DLL
#else
    #define FUNCTION_TOOLS_DLL
#endif

#endif // TOOLS_GLOBAL_H
