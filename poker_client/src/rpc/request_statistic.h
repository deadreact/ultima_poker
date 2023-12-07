#pragma once

#ifndef Q_MOC_RUN
    #include <api/lobby_stat/request.h>
#endif // Q_MOC_RUN

class NetworkClient;

struct request_statistic : gbp::ns_api::ns_statistic::ns_request::stat_common, gbp::ns_api::ns_statistic::ns_request::stat_desktop
{
private:
    NetworkClient& m_client;
    static const std::string classname_common() { return "gbp::ns_api::ns_statistic::ns_request::stat_common"; }
    static const std::string classname_desktop() { return "gbp::ns_api::ns_statistic::ns_request::stat_desktop"; }
public:
    explicit request_statistic(NetworkClient* const client);

    // stat interface
public:
    virtual void games_statistic() override;
    virtual void get_additional_tourn_stat(gbp_i64 tourn_id) override;
    virtual void get_tourn_users_stat(gbp_i64 tourn_id) override;
};
