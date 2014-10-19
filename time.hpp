/* time.hpp - (c) James S Renwick 2013
   -----------------------------------
   Version 1.0.0
*/
#pragma once
#include "std"

namespace std
{
	struct Timestamp
	{
	private:
		UInt64 ticks;

	public:
		inline UInt64 getTickCount() {
			return this->ticks;
		}

		inline UInt getMillisecond() {
			return (UInt)this->ticks % 60;
		}

		inline UInt getSecond() {
			return ((UInt)this->ticks / 1000) % 60;
		}

		inline UInt getMinute() {
			return ((UInt)this->ticks / (1000 * 60)) % 60;
		}

		inline UInt getHour() {
			return ((UInt)this->ticks / (1000 * 60 * 60)) % 24;
		}

	public:
		/*bool operator > (Datestamp ds);
		bool operator < (Datestamp ds);
		bool operator <= (Datestamp ds);
		bool operator >= (Datestamp ds);
		bool operator == (Datestamp ds);
		bool operator != (Datestamp ds);*/
	};

	struct Datestamp
	{
	public:
		byte   second;
		byte   minute;
		byte   hour;
		byte   day;
		byte   month;
		UInt16 year;

	public:
		/*bool operator > (Datestamp ds);
		bool operator < (Datestamp ds);
		bool operator <= (Datestamp ds);
		bool operator >= (Datestamp ds);
		bool operator == (Datestamp ds);
		bool operator != (Datestamp ds);*/
	};
}