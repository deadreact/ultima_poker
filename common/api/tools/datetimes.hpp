#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>
#include "../declare_type.h"
#include "../api_common.h"

namespace gbp
{

 namespace ns_datetimes
 {

  inline std::string get_current_utc_time_str()
  {
   char buf[32];
   std::time_t raw_time;
   std::tm* time;

   std::time(&raw_time);
   time = gmtime(&raw_time);

   //strftime(buf, sizeof(buf), "%d.%m.%Y-%H:%M:%S", time);
   strftime(buf, sizeof(buf), "%d.%m-%H:%M:%S", time);

   std::string result = buf;
   return result;;
  }

  inline ns_api::birth_date birth_date_from_str(const std::string& birth_date_str)
  {
   ns_api::birth_date result;

   std::vector<std::string> strs;
   boost::split(strs, birth_date_str, boost::is_any_of("-"));
   if (strs.size() == 3)
   {
    result._year = std::stoi(strs[0]);
    result._month = std::stoi(strs[1]);
    result._day = std::stoi(strs[2]);
   }

   return result;
  }

  inline std::string birth_date_to_str(const ns_api::birth_date& birth_date)
  {
   std::ostringstream os;
   os << birth_date._year << "-" << birth_date._month << "-" << birth_date._day;

   return os.str();
  }

  // от tm к time
  inline ns_api::time_s tmtime_to_time(const std::tm& time_)
  {
   boost::posix_time::ptime pt = boost::posix_time::ptime_from_tm(time_);
   boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
   boost::posix_time::time_duration::sec_type sec = (pt - epoch).total_seconds();

   return sec;
  }

  // от time к tm
  inline std::tm time_t_to_tm(std::time_t t) {
   return *gmtime(&t);
  }
 
  inline std::string get_str_time(ns_api::time_s time_, const std::string& t_format)
  {
   /**
    * formats:
    * "%d.%m-%H:%M:%S" = 01.06-12:08:42
    * "%Y-%m-%d" = 2017-06-01
    */

   char buf[32];
   std::time_t raw_time = time_;
   std::tm* time = gmtime(&raw_time);
   //strftime(buf, sizeof(buf), "%d.%m.%Y-%H:%M:%S", time);
   strftime(buf, sizeof(buf), t_format.c_str(), time);
   std::string result = buf;
   return result;
  }

  // номер текущего часа
  inline int get_current_utc_hour() 
  {
   // 0-23
   return boost::posix_time::second_clock::universal_time().time_of_day().hours();
  }

  // номер месяца
  inline int get_utc_month(int utc_time) {
   return boost::posix_time::from_time_t(utc_time).date().month().as_number();
  }

  // год
  inline int get_utc_year(int utc_time) {
   return boost::posix_time::from_time_t(utc_time).date().year();
  }

  // осталось секунд до конца текущих суток
  inline ns_api::time_s get_utc_day_seconds_left()
  {
   ns_api::time_s current_time = time(0);

   // текущий час (0-23)
   ns_api::time_s current_hour = boost::posix_time::second_clock::universal_time().time_of_day().hours();
   // прошло кол-во секунд с начала текущего часа
   ns_api::time_s current_seconds =
    (boost::posix_time::second_clock::universal_time().time_of_day().minutes() * 60) +
    boost::posix_time::second_clock::universal_time().time_of_day().seconds();
   // время начала текущих суток
   ns_api::time_s daily_start_seconds = current_time - ((current_hour * 3600) + current_seconds);

   /**
    * следующие 00:00
    * 86400 - секунд в сутках */
   ns_api::time_s timeout = daily_start_seconds + /*24 часа*/86400 - current_time;

   return timeout;
  }
  
  inline int get_utc_week_number_from_time(int time_utc) 
  {
   int offset = 3 * 24 * 3600; // начало эпохи не с понедельника
   int result = (time_utc + offset) / (7 * 24 * 60 * 60);
   return result;
  }

  inline int get_utc_day_number_from_time(int time_utc) {
   return time_utc / (24 * 60 * 60);
  }

  inline int get_utc_week_day_from_time(std::time_t time_)
  {
   //return boost::posix_time::from_time_t(time_).date().day_of_week().as_number();
   int result = boost::posix_time::from_time_t(time_).date().day_of_week().as_number();
   if (result == 0) {
    result = 7;
   }
   return result;
  }

 } // ns_datetimes
} // ns gbp
