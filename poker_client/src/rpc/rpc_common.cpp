#include <precompiled.h>
#include "rpc_common.h"
#include "network_client.h"

#include "request_lobby.h"
#include "request_statistic.h"
#include "table/request.h"

#include "server_request_lobby.h"
#include "table/server_request.h"
#include "table/server_reply.h"

#include "reply_lobby.h"
#include "reply_statistic.h"
#include "table/reply.h"

#include "rpc_lobby.h"
#include "table/rpc_table.h"
#include "rpc_statistic.h"


template <>
reply_lobby* rpc_object<reply_lobby>() {
    return netClient().reply<reply_lobby>();
}

template <>
reply_statistic* rpc_object<reply_statistic>() {
    return netClient().reply<reply_statistic>();
}

template <>
reply_table_cash* rpc_object<reply_table_cash>() {
    return netClient().reply<reply_table_cash>();
}
template <>
reply_table_tournament* rpc_object<reply_table_tournament>() {
    return netClient().reply<reply_table_tournament>();
}

template <>
request_lobby* rpc_object<request_lobby>() {
    return netClient().request<request_lobby>();
}

template <>
request_statistic* rpc_object<request_statistic>() {
    return netClient().request<request_statistic>();
}

template <>
request_table_cash* rpc_object<request_table_cash>() {
    return netClient().request<request_table_cash>();
}
template <>
request_table_tournament* rpc_object<request_table_tournament>() {
    return netClient().request<request_table_tournament>();
}

template <>
rpc_lobby* rpc_object<rpc_lobby>() {
    return netClient().rpc<rpc_lobby>();
}

template <>
rpc_table* rpc_object<rpc_table>() {
    return netClient().rpc<rpc_table>();
}

template <>
rpc_statistic* rpc_object<rpc_statistic>() {
    return netClient().rpc<rpc_statistic>();
}

template <>
const reply_statistic::emitter_t* rpc_object() {
    return netClient().emitter<reply_statistic>();
}

template <>
const ns_server_request::table_cash::emitter_t* rpc_object() {
    return netClient().server_request<ns_server_request::table_cash>()->emitter();
}
