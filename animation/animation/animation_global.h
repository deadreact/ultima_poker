#ifndef ANIMATION_GLOBAL_H
#define ANIMATION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STATIC_LIB) || defined (NO_DLL_IMPORT)
    #define ANIMATION_DLL
#else
    #if defined(ANIMATION_LIB)
        #define ANIMATION_DLL Q_DECL_EXPORT
    #else
        #define ANIMATION_DLL Q_DECL_IMPORT
    #endif
#endif // STATIC_LIB

#endif // ANIMATION_GLOBAL_H
