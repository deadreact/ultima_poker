#pragma once

#include "../api_common.h"

namespace gbp
{

 namespace ns_cards
 {
 
  inline int compare_high(const ns_api::ns_table::cards_combination_t& cc1, const ns_api::ns_table::cards_combination_t& cc2)
  {
   if (cc1._combination < cc2._combination) return -1;
   if (cc1._combination > cc2._combination) return 1;

   //если выигрышные комбинации равны, то сравниваем по картам
   for (std::size_t i = 0; i < cc1._cards.size(); ++i)
   {
    if (cc1._cards[i] % 13 < cc2._cards[i] % 13) return -1;
    if (cc1._cards[i] % 13 > cc2._cards[i] % 13) return 1;
   }

  // if (_cards < cc._cards) return -1;
  // if (_cards > cc._cards) return 1;

   return 0;
  }

  inline bool comb_high_equal(const ns_api::ns_table::cards_combination_t& cc1, const ns_api::ns_table::cards_combination_t& cc2) {
   return (compare_high(cc1, cc2) == 0);
  }

  inline bool comb_high_greater(const ns_api::ns_table::cards_combination_t& cc1, const ns_api::ns_table::cards_combination_t& cc2) {
   return (compare_high(cc1, cc2) > 0);
  }

  inline bool comb_high_lesser(const ns_api::ns_table::cards_combination_t& cc1, const ns_api::ns_table::cards_combination_t& cc2) {
   return (compare_high(cc1, cc2) < 0);
  }

  inline ns_api::ns_table::cards_combination_t
  get_high_max(const ns_api::ns_table::cards_combination_t& cc1, const ns_api::ns_table::cards_combination_t& cc2) 
  {
   if (comb_high_greater(cc1, cc2)) { return cc1; }
   return cc2;
  }

  inline int compare_low(const ns_api::ns_table::cards_low_combination& cc1, const ns_api::ns_table::cards_low_combination& cc2)
  {
   for (size_t i = 0; i < cc1._cards.size(); ++i)
   {
    if (cc1._cards[i] % 13 > cc2._cards[i] % 13) return -1;
    if (cc1._cards[i] % 13 < cc2._cards[i] % 13) return 1;
   }

   return 0;
  }

  inline bool comb_low_equal(const ns_api::ns_table::cards_low_combination& cc1, const ns_api::ns_table::cards_low_combination& cc2) {
   return (compare_low(cc1, cc2) == 0);
  }

  inline bool comb_low_greater(const ns_api::ns_table::cards_low_combination& cc1, const ns_api::ns_table::cards_low_combination& cc2) {
   return (compare_low(cc1, cc2) > 0);
  }

  inline bool comb_low_lesser(const ns_api::ns_table::cards_low_combination& cc1, const ns_api::ns_table::cards_low_combination& cc2) {
   return (compare_low(cc1, cc2) < 0);
  }

 } // ns_cards
} // ns gbp
