#pragma once

class NetworkClient;

class IReplyHandler
{
protected:
    virtual void registerNetworkReplyHandlers(const NetworkClient* client) = 0;
};
