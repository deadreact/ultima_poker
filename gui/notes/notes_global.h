#pragma once

#include <QtCore/qglobal.h>

#if defined(STATIC_LIB) || defined (NO_DLL_IMPORT)
    #define NOTES_DLL
#else
    #if defined(NOTES_LIB)
        #define NOTES_DLL Q_DECL_EXPORT
    #else
        #define NOTES_DLL Q_DECL_IMPORT
    #endif
#endif //STATIC_LIB

