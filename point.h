/* point.h - (c) James S Renwick 2013
   ----------------------------------
   Version 1.0.4
*/
#pragma once
#include "std.h"
#include "math.h"

namespace std
{
	struct Point
	{
		UInt32 X;
		UInt32 Y;

		Point() noexcept : X(0), Y(0) { }
		Point(UInt32 X, UInt32 Y) noexcept : X(X), Y(Y) { }

		inline Point operator +=(Point& p1) noexcept
		{
			this->X += p1.X;
			this->Y += p1.Y;
			return *this;
		}
		inline Point operator -=(Point& p1) noexcept
		{
			this->X -= p1.X;
			this->Y -= p1.Y;
			return *this;
		}
		inline Point operator *=(UInt32 f) noexcept
		{
			this->X *= f;
			this->Y *= f;
			return *this;
		}
		inline Point operator /=(UInt32 f) noexcept
		{
			this->X /= f;
			this->Y /= f;
			return *this;
		}
	};

	inline const Point operator +(const Point p1, const Point p2) noexcept {
		return Point(p1.X + p2.X, p1.Y + p2.Y);
	}
	inline const Point operator -(const Point p1, const Point p2) noexcept {
		return Point(p1.X - p2.X, p1.Y - p2.Y);
	}
	inline const Point operator *(UInt32 factor, const Point p1) noexcept {
		return Point(p1.X * factor, p1.Y * factor);
	}
	inline const Point operator *(const Point p1, UInt32 factor) noexcept {
		return Point(p1.X * factor, p1.Y * factor);
	}
	inline const Point operator /(UInt32 factor, const Point p1) noexcept {
		return Point(p1.X / factor, p1.Y / factor);
	}
	inline const Point operator /(const Point p1, UInt32 factor) noexcept {
		return Point(p1.X / factor, p1.Y / factor);
	}


	struct Vector2
	{
	public:
		float X;
		float Y;

		Vector2() noexcept : X(0), Y(0) { }
		Vector2(float X, float Y) noexcept : X(X), Y(Y) { }

	public:
		inline float getMagnitude() const noexcept {
			return sqrt(X * X + Y * Y);
		}
		inline float getSqrMagnitude() const noexcept {
			return X * X + Y * Y;
		}

		inline const Vector2 getNormalised() const noexcept
		{
			float div = this->getMagnitude();
			return Vector2(this->X / div,  this->Y / div);
		}

		inline Vector2 operator +=(const Vector2& v1) noexcept
		{
			this->X += v1.X;
			this->Y += v1.Y;
			return *this;
		}
		inline Vector2 operator -=(const Vector2& v1) noexcept
		{
			this->X -= v1.X;
			this->Y -= v1.Y;
			return *this;
		}
		inline Vector2 operator *=(const float f) noexcept
		{
			this->X *= f;
			this->Y *= f;
			return *this;
		}
		inline Vector2 operator /=(const float f) noexcept
		{
			this->X /= f;
			this->Y /= f;
			return *this;
		}

		inline const Vector2 getDotProduct(const Vector2& v1) const noexcept
		{
			return Vector2(this->X * v1.X, this->Y * v1.Y);
		}
	};

	inline const Vector2 operator +(const Vector2 v1, const Vector2 v2) noexcept {
		return Vector2(v1.X + v2.X, v1.Y + v2.Y);
	}
	inline const Vector2 operator -(const Vector2 v1, const Vector2 v2) noexcept {
		return Vector2(v1.X - v2.X, v1.Y - v2.Y);
	}
	inline const Vector2 operator *(float factor, const Vector2 v1) noexcept {
		return Vector2(v1.X * factor, v1.Y * factor);
	}
	inline const Vector2 operator *(const Vector2 v1, float factor) noexcept {
		return Vector2(v1.X * factor, v1.Y * factor);
	}
	inline const Vector2 operator /(float factor, const Vector2 v1) noexcept {
		return Vector2(v1.X / factor, v1.Y / factor);
	}
	inline const Vector2 operator /(const Vector2 v1, float factor) noexcept {
		return Vector2(v1.X / factor, v1.Y / factor);
	}

	struct Vector3
	{
	public:
		float X;
		float Y;
		float Z;

		Vector3() noexcept : X(0), Y(0), Z(0) { }
		Vector3(float X, float Y, float Z) noexcept : X(X), Y(Y), Z(Z) { }

	public:
		inline float getMagnitude() const noexcept {
			return sqrt(X * X + Y * Y + Z * Z);
		}
		inline float getSqrMagnitude() const noexcept {
			return X * X + Y * Y + Z * Z;
		}

		inline const Vector3 getNormalised() const noexcept
		{
			float div = this->getMagnitude();
			return Vector3(this->X / div, this->Y / div, this->Z / div);
		}

		Vector3 operator +=(const Vector3& v1) noexcept
		{
			this->X += v1.X;
			this->Y += v1.Y;
			this->Z += v1.Z;
			return *this;
		}
		Vector3 operator -=(const Vector3& v1) noexcept
		{
			this->X -= v1.X;
			this->Y -= v1.Y;
			this->Z -= v1.Z;
			return *this;
		}
		Vector3 operator *=(float f) noexcept
		{
			this->X *= f;
			this->Y *= f;
			this->Z *= f;
			return *this;
		}
		Vector3 operator /=(float f) noexcept
		{
			this->X /= f;
			this->Y /= f;
			this->Z /= f;
			return *this;
		}

		inline const Vector3 getDotProduct(const Vector3& v1) const noexcept
		{
			return Vector3(this->X * v1.X, this->Y * v1.Y, this->Z * v1.Z);
		}

		inline const Vector3 getCrossProduct(const Vector3& v1) const noexcept
		{
			return Vector3(
				this->Y * v1.Z - this->Z * v1.Y,
				this->Z * v1.X - this->X * v1.Z,
				this->X * v1.Y - this->Y * v1.X
			);
		}
	};

	inline const Vector3 operator +(const Vector3 v1, const Vector3 v2) noexcept {
		return Vector3(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);
	}
	inline const Vector3 operator -(const Vector3 v1, const Vector3 v2) noexcept {
		return Vector3(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z);
	}
	inline const Vector3 operator *(float factor, const Vector3 v1) noexcept {
		return Vector3(v1.X * factor, v1.Y * factor, v1.Z * factor);
	}
	inline const Vector3 operator *(const Vector3 v1, float factor) noexcept {
		return Vector3(v1.X * factor, v1.Y * factor, v1.Z * factor);
	}
	inline const Vector3 operator /(float factor, const Vector3 v1) noexcept {
		return Vector3(v1.X / factor, v1.Y / factor, v1.Z / factor);
	}
	inline const Vector3 operator /(const Vector3 v1, float factor) noexcept {
		return Vector3(v1.X / factor, v1.Y / factor, v1.Z / factor);
	}

}
