#pragma once

#include "types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_bots
  {
   namespace ns_botserver_request
   { // bot_server -> game_server

    /***************************** MISCELLANEOUS ********************************/

    struct miscellaneous
    {
     virtual ~miscellaneous() { }

     // добавление контекстов инфы ботов в рантайм
     virtual void add_runtime_contexts(const std::vector<gbp::ns_api::ns_bots::bot_runtime_info>& bots_info) = 0;
     // удаление ботов из рантайма игрового
     virtual void remove_runtime_contexts(const std::vector<gbp_i64>& bot_ids) = 0;
     // запрос чат-столов
     virtual void get_chat_cashes() = 0;
     // запрос инфы по отдельному кешу
     virtual void get_single_cash_info(gbp_i64 tournament_id, gbp_i64 game_id, gbp_i64 admin_id) = 0;
     // запрос фин. плана за сутки
     virtual void get_day_fin_plan(gbp_i64 admin_id, gbp_i32 day_number) = 0;
     // взять список стран
     virtual void get_countries_list(gbp_i64 admin_id) = 0;
     // высадить игрока (бота или человека) со стола
     virtual void player_close_table(gbp_i64 admin_id, gbp_i64 user_id, gbp_i64 tournament_id, gbp_i64 game_id) = 0;

    };

    /******************************** CASHES ************************************/

    struct cashes
    {
     virtual ~cashes() { }

     // запросить список кешей
     virtual void get_cashes() = 0;

    };
   
    /****************************** TOURNAMENTS ********************************/
    
    struct tournaments
    {
     virtual ~tournaments() { }

     // запросить список турниров
     virtual void get_tournaments() = 0;

     // запросить доп. инфу по турниру     
     virtual void get_additional_tournament_stat(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , const gbp::ns_api::ns_bots::tourn_stat_cashes_filter& cashes_filter) = 0;

    };
    
    /************************* PLANNED DISTRIBUTIONS ****************************/

    struct planned_distributions
    {
     virtual ~planned_distributions() { }

     // взять все запланированные раздачи игры
     virtual void get_planned_distributions(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::planned_distributions_filter& filter) = 0;

     // добавление запланированной раздачи
     virtual void add_planned_distribution(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::planned_distribution& new_planned_distribution) = 0;

     // удаление запланированной раздачи
     virtual void remove_planned_distribution(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 distribution_number) = 0;

     // получить список по истории шаблонных запл. раздач
     virtual void get_planned_distr_history_items(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , bool is_sit_n_go
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     virtual void remove_planned_distr_history_item(
      gbp_i64 admin_id
      , gbp_i64 tournament_id
      , bool is_sit_n_go
      , gbp_i64 game_id
      , gbp_i32 distr_number) = 0;

    };

    /**************************** PLAY INSTEAD BOTS ****************************/

    struct play_instead_bots
    {
     virtual ~play_instead_bots() { }

     virtual void play_instead_bots_start(gbp_i64 admin_id, gbp_i64 tournament_id, gbp_i64 game_id) = 0;
     virtual void play_instead_bots_stop(gbp_i64 admin_id, gbp_i64 tournament_id, gbp_i64 game_id) = 0;
     virtual void get_table_state_instead_bots(gbp_i64 admin_id, gbp_i64 tournament_id, gbp_i64 game_id) = 0;

    };

    /********************************** CHATS **********************************/

    struct chat_table_play
    {
     virtual ~chat_table_play() { }

     virtual void chat_table_play_start(
      gbp_i64 chat_admin_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     virtual void chats_table_play_stop(const std::vector<gbp_i64>& chat_cashes_ids) = 0;

    };

    /********************************** CHASE **********************************/

    struct chase
    {
     virtual ~chase() { }

     virtual void get_full_chase_stat() = 0;
     virtual void update_user_notes(gbp_i64 admin_id, const std::string& nickname, gbp_i64 user_id, const std::string& new_notes) = 0;

     virtual void update_user_task(
      gbp_i64 admin_id
      , gbp_i64 user_id
      , gbp_i64 tournament_id
      , gbp_i32 task_number
      , const std::string& task_text) = 0;

     virtual void get_tournament_prizes(gbp_i64 tournament_id, gbp_i64 admin_id, bool is_sit_n_go) = 0;

    };

   } // ns ns_botserver_request
  } // ns ns_bots
 } // ns ns_api
} // ns gbp
