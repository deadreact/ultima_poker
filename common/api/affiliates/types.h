#pragma once

#include "../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_affiliates
  {
   GBP_DECLARE_TYPE(
     user_register
     ,
     (_user_id, (gbp_i64))
     (_country_code, (std::string))
     (_btag, (std::string))
     (_reg_date, (ns_api::time_s))
     (_brand_id, (gbp_i64))
    )

    GBP_DECLARE_TYPE(
     user_activity
     ,
     // user_id
     (_customer_id, (gbp_i64))
     (_activity_date, (ns_api::time_s))
     // 1
     (_product_id, (gbp_i32))
     // rake игрока
     (_gross_revenue, (ns_api::money))
     /**
      * сумма:
      * - стоимость билетов
      * - бездепозитные бонусы (напр., зарегался, прошёл верификацию и ему дали деньги)
      * - отыгранная сумма отыгрываемого бонуса (только то, что зачисляется на счёт) */
     (_bonuses, (ns_api::money))
     /**
      * - сумма возвратов средств (бывает редко);
      * - комиссии плат. систем (за вводы и выводы средств) */
     (_adjustments, (ns_api::money))
     (_adjustment_type_id, (gbp_i64))
     // депозиты игрока
     (_deposits, (ns_api::money))
     // выводы (money out) игрока
     (_withdrawals, (ns_api::money))
     (_transaction, (gbp_i32))
     // 1
     (_brand_id, (gbp_i32))
     (_bonus_type_id, (gbp_i32))
    )

  } // namespace ns_affiliates
 } // namespace ns_api
} // namespace gbp