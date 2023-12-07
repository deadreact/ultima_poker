#include <precompiled.h>
#include "request_statistic.h"
#ifndef Q_MOC_RUN
    #include "network_client.h"
#endif // Q_MOC_RUN


request_statistic::request_statistic(NetworkClient * const client)
    : m_client(*client)
{
}

void request_statistic::games_statistic()
{
//    std::cout << "request.games_statistic" << std::endl;
    m_client.call_server(classname_desktop() + ".games_statistic");
}

void request_statistic::get_additional_tourn_stat(gbp_i64 tourn_id)
{
    m_client.call_server(classname_desktop() + ".get_additional_tourn_stat", tourn_id);
}

void request_statistic::get_tourn_users_stat(gbp_i64 tourn_id)
{
    m_client.call_server(classname_desktop() + ".get_tourn_users_stat", tourn_id);
}
