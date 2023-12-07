#pragma once

#ifndef Q_MOC_RUN

#endif //!Q_MOC_RUN

#include "utils/logger.h"
#include <QMetaEnum>
#include <iostream>
#include <api_models/api_util.hpp>


class GameErrorHandler : public QObject
{
    Q_OBJECT

public:
    enum class ErrorType
    {
        ReplyStatusNotOk,
        ValidTournamentId,
        InvalidTournamentId,
        InvalidCashId,
    };
    Q_ENUM(ErrorType)
public:
    GameErrorHandler(QObject* parent): QObject(parent) {}

    template <typename ...Args>
    void handleError(ErrorType type, const QString& methodName, gbp_i64 gameId, Args... args)
    {
        const char* typeName = QMetaEnum::fromType<ErrorType>().valueToKey(underlying_cast(type));
        Logger::error(QString("%0 in %1, game id: %2").arg(typeName, methodName, QString::number(gameId)), __FILE__, __LINE__);

        handleErrorArgs("args:", args...);
    }

    template <typename T, typename ...Args>
    void handleErrorArgs(const T& arg, Args... args)
    {
        std::cout << arg << " ";
        handleErrorArgs(args...);
    }

    template <typename T>
    void handleErrorArgs(const T& arg)
    {
        std::cout << arg << std::endl;
    }
};
