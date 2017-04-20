/* point.hpp - (c) 2016 James S Renwick
   ------------------------------------
   Version 2.0.0
*/
#pragma once
#include "std"
#include "meta.hpp"
#include "math.hpp"
#include "_feats.hpp"
#include "_std.hpp"

namespace std
{
    template<typename T>
	struct point
	{
		T x;
		T y;

        inline __STD_CONSTEXPR_CONST point() noexcept(noexcept(T())) = default;
        inline __STD_CONSTEXPR_CONST point(T&& x, T&& y) noexcept(noexcept(T(std::forward<T>(y)))) :
            x(std::forward<T>(x)), y(std::forward<T>(y)) { }

        __DefaultCopyMove(point);


		inline __STD_CONSTEXPR point operator +=(point& p1) noexcept(noexcept(x += x))
		{
			this->x += p1.x;
			this->y += p1.y;
			return *this;
		}
		inline __STD_CONSTEXPR point operator -=(point& p1) noexcept(noexcept(x -= x))
		{
			this->x -= p1.x;
			this->y -= p1.y;
			return *this;
		}
		inline __STD_CONSTEXPR point operator *=(uint32_t f) noexcept(noexcept(x *= x))
		{
			this->x *= f;
			this->y *= f;
			return *this;
		}
		inline __STD_CONSTEXPR point operator /=(uint32_t f) noexcept(noexcept(x /= x))
		{
			this->x /= f;
			this->y /= f;
			return *this;
		}
        inline __STD_CONSTEXPR_CONST bool operator ==(const point& point) const noexcept(noexcept(x == x))
		{
			return this->x == point.x && this->y == point.y;
		}
	};

    template<typename T>
    __STD_CONSTEXPR_CONST inline point<T> operator +(const point<T>& p1, const point<T>& p2) noexcept {
		return point<T>(p1.x + p2.x, p1.y + p2.y);
	}
    template<typename T>
    __STD_CONSTEXPR_CONST inline point<T> operator -(const point<T>& p1, const point<T>& p2) noexcept {
		return point<T>(p1.x - p2.x, p1.y - p2.y);
	}
    template<typename T>
    __STD_CONSTEXPR_CONST inline point<T> operator *(uint32_t factor, const point<T>& p1) noexcept {
		return point<T>(p1.x * factor, p1.y * factor);
	}
    template<typename T>
    __STD_CONSTEXPR_CONST inline point<T> operator *(const point<T>& p1, uint32_t factor) noexcept {
		return point<T>(p1.x * factor, p1.y * factor);
	}
    template<typename T>
    __STD_CONSTEXPR_CONST inline point<T> operator /(uint32_t factor, const point<T>& p1) noexcept {
		return point<T>(p1.x / factor, p1.y / factor);
	}
    template<typename T>
    inline __STD_CONSTEXPR_CONST point<T> operator /(const point<T>& p1, uint32_t factor) noexcept {
		return point<T>(p1.x / factor, p1.y / factor);
	}
}
