#pragma once

#include "api_models_global.h"
#include <qiodevice.h>
#include "../../common/api/gbp_int.hpp"

namespace ns_model
{
    class API_MODEL_DLL IOModel
    {
    protected:
        static gbp_i64 sm_userId;
        QString m_destroyedMsg;
        IOModel(QObject *object = nullptr);
    public:
        virtual ~IOModel();
        constexpr static const uint serialize_key = 0x0C1a551D;
        virtual bool read(QIODevice *iodevice = nullptr) = 0;
        virtual bool write(QIODevice *iodevice = nullptr) const = 0;

        static QString getPath();
        static void setUserId(gbp_i64 id);
        static gbp_i64 userId();
    };
}
