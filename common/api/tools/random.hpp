#pragma once

#include <random>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "../gbp_int.hpp"

namespace gbp 
{

 struct randomgen
 {
  randomgen(): 
   _gen(_rd())
   ,_def_re(_rd())
   ,_uniform_dist(1, RAND_MAX) { }

 private:
  std::random_device _rd;
  std::mt19937 _gen;
  std::default_random_engine _def_re;
  std::uniform_int_distribution<int> _uniform_dist;

 public:
  int get_rand(int from, int to)
  {
   if (from > to)
   {
    int t = from;
    from = to;
    to = t;
   }

   std::uniform_int_distribution<> dis(from, to);
   return dis(_gen);
  }
  
  double get_r_rand(double from, double to)
  {
   if (from > to)
   {
    double t = from;
    from = to;
    to = t;
   }

   std::uniform_real_distribution<> dis(from, to);
   return dis(_gen);
  }

  std::string get_item(const std::unordered_map<std::string, double>& data)
  {
   double summ = std::accumulate(data.begin(), data.end(), 0.0, [&](double prev, const std::pair<std::string, double>& pair) {
     return prev + pair.second;
    });

   double r = (double)_uniform_dist(_def_re);
   double rand = r / RAND_MAX;
   bool set = true;
   double result = 0.0;

   for (const std::pair<std::string, double>& item: data)
   {
    double value = item.second;

    if (set) 
    {
     result = rand - (value / summ); 
     set = false; 
    }
    else {
     result = result - (value / summ);
    }

    if (result <= 0) {
     return item.first;
    }
   }

   return "";
  }

  static std::string get_random_string(int len)
  {
   static std::random_device rd;
   static std::mt19937 gen(rd());
   
   static const char alphanum[] = 
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

   static std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

   std::string result_str;

   for (int i = 0; i < len; ++i)
   {
    const char sym = alphanum[dis(gen)];
    //result_str.append(1, sym);
    result_str.push_back(sym);
   }

   return result_str;
  }

  gbp_i64 get_random_id(gbp_i64 from, gbp_i64 to, const std::unordered_set<gbp_i64>& exclusions)
  {
   gbp_i64 rand_val = get_rand(from, to);
   if (exclusions.find(rand_val) == exclusions.end()) {
    return rand_val;
   }
   else {
    return get_random_id(from, to, exclusions);
   }
  }

 }; // struct random

} // ns gbp
