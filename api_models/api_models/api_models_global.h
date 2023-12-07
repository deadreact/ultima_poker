#ifndef API_MODELS_GLOBAL_H
#define API_MODELS_GLOBAL_H

#include <QtCore/qglobal.h>

#if !defined(API_MODELS_SHARED_LIB) || defined(STATIC_LIB) || defined (NO_DLL_IMPORT)
    #define API_MODEL_DLL
#else
    #if defined(API_MODELS_LIB)
        #define API_MODEL_DLL Q_DECL_EXPORT
    #else
        #define API_MODEL_DLL Q_DECL_IMPORT
    #endif
#endif //STATIC_LIB

#endif // API_MODELS_GLOBAL_H
