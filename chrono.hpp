/* time.hpp - (c) 2016 James S Renwick
   -----------------------------------
   Authors: James S Renwick
*/
#pragma once
#include <type_traits>
#include <cstddef>

extern "C++" {
namespace std
{
	namespace chrono
	{
		struct _period { };
		struct nanoseconds : _period { };
		struct microseconds : _period { };
		struct milliseconds : _period { };
		struct seconds : _period { };

		template<typename Period>
		struct duration
		{
		private:
			long long units;

		public:
			constexpr inline explicit duration(long long units) noexcept :
				units(units) { }

			constexpr inline long long count() const noexcept { return units; }

			template<typename TargetPeriod>
			constexpr duration<TargetPeriod> as() const noexcept;
		};


		template<typename TargetPeriod, typename InputPeriod>
		constexpr duration<TargetPeriod> duration_cast(
			const duration<InputPeriod> duration) noexcept;

#define _DURATION_CAST(from, to, expr) \
		template<> constexpr inline duration<to> duration_cast( \
			const duration<from> duration) noexcept { \
			return chrono::duration<to>(duration.count() expr); \
		}

		_DURATION_CAST(nanoseconds, nanoseconds, );
		_DURATION_CAST(nanoseconds, microseconds, / 1000);
		_DURATION_CAST(nanoseconds, milliseconds, / 1000000);
		_DURATION_CAST(nanoseconds, seconds, / 1000000000);

		_DURATION_CAST(microseconds, microseconds, );
		_DURATION_CAST(microseconds, nanoseconds, *1000);
		_DURATION_CAST(microseconds, milliseconds, / 1000);
		_DURATION_CAST(microseconds, seconds, / 1000000);

		_DURATION_CAST(milliseconds, milliseconds, );
		_DURATION_CAST(milliseconds, nanoseconds, *1000000);
		_DURATION_CAST(milliseconds, microseconds, *1000);
		_DURATION_CAST(milliseconds, seconds, / 1000);

		_DURATION_CAST(seconds, seconds, );
		_DURATION_CAST(seconds, nanoseconds, *1000000000);
		_DURATION_CAST(seconds, microseconds, *1000000);
		_DURATION_CAST(seconds, milliseconds, *1000);

#undef _DURATION_CAST

		template<typename Period>
		template<typename TargetPeriod>
		constexpr duration<TargetPeriod> duration<Period>::as() const noexcept
		{
			return duration_cast<TargetPeriod>(*this);
		}
	}

	/* Structure representing a fixed point in time. */
	struct timestamp
	{
	private:
		uint64_t ticks;

	public:
		constexpr inline uint64_t getTickCount() const noexcept {
			return this->ticks;
		}

		constexpr inline uint64_t getMillisecond() const noexcept {
			return (uint64_t)this->ticks % 60;
		}

		constexpr inline uint64_t getSecond() const noexcept {
			return ((uint64_t)this->ticks / 1000) % 60;
		}

		constexpr inline uint64_t getMinute() const noexcept {
			return ((uint64_t)this->ticks / (1000 * 60)) % 60;
		}

		constexpr inline uint64_t getHour() const noexcept {
			return ((uint64_t)this->ticks / (1000 * 60 * 60)) % 24;
		}

	public:
		constexpr inline bool operator >  (timestamp ts) const noexcept { return ticks > ts.ticks; }
		constexpr inline bool operator <  (timestamp ts) const noexcept { return ticks < ts.ticks; }
		constexpr inline bool operator <= (timestamp ts) const noexcept { return ticks <= ts.ticks; }
		constexpr inline bool operator >= (timestamp ts) const noexcept { return ticks >= ts.ticks; }
		constexpr inline bool operator == (timestamp ts) const noexcept { return ticks == ts.ticks; }
		constexpr inline bool operator != (timestamp ts) const noexcept { return ticks != ts.ticks; }

	public:
		constexpr inline timestamp(uint64_t ticks) noexcept : ticks(0) { }

		constexpr timestamp(uint8_t hour, uint8_t minute, uint8_t second) noexcept :
			ticks(hour * 60*60*1000 + minute * 60*1000 + second * 1000) { }
	};
}
}