#define _VECTORZ_H

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)

#define rad(a) a * 0.01745329251
#define deg(a) a * 57.295779513082

#include <Windows.h>
#include <math.h>
#include <emmintrin.h>
#include <xmmintrin.h>

inline void SinCosX(const float rad, float &sin, float &cos)
{
	const __m128 _ps_fopi = _mm_set_ss(1.27323954473516f);

	const __m128 _ps_0p5 = _mm_set_ss(0.5f);
	const __m128 _ps_1 = _mm_set_ss(1.0f);

	const __m128 _ps_dp1 = _mm_set_ss(-0.7851562f);
	const __m128 _ps_dp2 = _mm_set_ss(-2.4187564849853515625e-4f);
	const __m128 _ps_dp3 = _mm_set_ss(-3.77489497744594108e-8f);

	const __m128 _ps_sincof_p0 = _mm_set_ss(2.443315711809948e-5f);
	const __m128 _ps_sincof_p1 = _mm_set_ss(8.3321608736e-3f);
	const __m128 _ps_sincof_p2 = _mm_set_ss(-1.6666654611e-1f);
	const __m128 _ps_coscof_p0 = _mm_set_ss(2.443315711809948e-5f);
	const __m128 _ps_coscof_p1 = _mm_set_ss(-1.388731625493765e-3f);
	const __m128 _ps_coscof_p2 = _mm_set_ss(4.166664568298827e-2f);

	const __m128i _pi32_1 = _mm_set1_epi32(1);
	const __m128i _pi32_i1 = _mm_set1_epi32(~1);
	const __m128i _pi32_2 = _mm_set1_epi32(2);
	const __m128i _pi32_4 = _mm_set1_epi32(4);

	const __m128 _mask_sign_raw = *(__m128*)&_mm_set1_epi32(0x80000000);
	const __m128 _mask_sign_inv = *(__m128*)&_mm_set1_epi32(~0x80000000);


	__m128  xmm3 = _mm_setzero_ps();
	__m128i emm0, emm2, emm4;

	__m128 sign_bit_cos, sign_bit_sin;

	__m128 x, y, z;
	__m128 y1, y2;

	__m128 a = _mm_set_ss(rad);

	x = _mm_and_ps(a, _mask_sign_inv);
	y = _mm_mul_ps(x, _ps_fopi);

	emm2 = _mm_cvtps_epi32(y);
	emm2 = _mm_add_epi32(emm2, _pi32_1);
	emm2 = _mm_and_si128(emm2, _pi32_i1);
	y = _mm_cvtepi32_ps(emm2);

	emm4 = emm2;

	emm0 = _mm_and_si128(emm2, _pi32_4);
	emm0 = _mm_slli_epi32(emm0, 29);
	__m128 swap_sign_bit_sin = _mm_castsi128_ps(emm0);

	emm2 = _mm_and_si128(emm2, _pi32_2);
	emm2 = _mm_cmpeq_epi32(emm2, _mm_setzero_si128());
	__m128 poly_mask = _mm_castsi128_ps(emm2);

	x = _mm_add_ps(x, _mm_mul_ps(y, _ps_dp1));
	x = _mm_add_ps(x, _mm_mul_ps(y, _ps_dp2));
	x = _mm_add_ps(x, _mm_mul_ps(y, _ps_dp3));

	emm4 = _mm_sub_epi32(emm4, _pi32_2);
	emm4 = _mm_andnot_si128(emm4, _pi32_4);
	emm4 = _mm_slli_epi32(emm4, 29);

	sign_bit_cos = _mm_castsi128_ps(emm4);
	sign_bit_sin = _mm_xor_ps(_mm_and_ps(a, _mask_sign_raw), swap_sign_bit_sin);

	z = _mm_mul_ps(x, x);

	y1 = _mm_mul_ps(_ps_coscof_p0, z);
	y1 = _mm_add_ps(y1, _ps_coscof_p1);
	y1 = _mm_mul_ps(y1, z);
	y1 = _mm_add_ps(y1, _ps_coscof_p2);
	y1 = _mm_mul_ps(y1, z);
	y1 = _mm_mul_ps(y1, z);
	y1 = _mm_sub_ps(y1, _mm_mul_ps(z, _ps_0p5));
	y1 = _mm_add_ps(y1, _ps_1);

	y2 = _mm_mul_ps(_ps_sincof_p0, z);
	y2 = _mm_add_ps(y2, _ps_sincof_p1);
	y2 = _mm_mul_ps(y2, z);
	y2 = _mm_add_ps(y2, _ps_sincof_p2);
	y2 = _mm_mul_ps(y2, z);
	y2 = _mm_mul_ps(y2, x);
	y2 = _mm_add_ps(y2, x);

	xmm3 = poly_mask;

	__m128 ysin2 = _mm_and_ps(xmm3, y2);
	__m128 ysin1 = _mm_andnot_ps(xmm3, y1);

	sin = _mm_cvtss_f32(_mm_xor_ps(_mm_add_ps(ysin1, ysin2), sign_bit_sin));
	cos = _mm_cvtss_f32(_mm_xor_ps(_mm_add_ps(_mm_sub_ps(y1, ysin1), _mm_sub_ps(y2, ysin2)), sign_bit_cos));
}

// from the leaked sdk

#define  FORCEINLINE			__forceinline


inline float BitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline float sqrt2(float sqr)
{
	float root = 0;

	__asm
	{
		sqrtss xmm0, sqr
		movss root, xmm0
	}

	return root;
}

#define FLOAT32_NAN_BITS     (unsigned long)0x7FC00000
#define FLOAT32_NAN          BitsToFloat( FLOAT32_NAN_BITS )
#define FLT_EPSILON      1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0

#define VEC_T_NAN FLOAT32_NAN
inline float FastSqrt(float x)
{
	auto root = _mm_sqrt_ss(_mm_load_ss(&x));
	return *(reinterpret_cast<float *>(&root));
}
class Vector//instead i switched my vector to floats instead of vec_t because my hitbox pos was fucked and made me crash? but seems same its similar to yours because yours works lol xDD
{
public:
	float x, y, z;

	Vector(void);
	Vector(float x, float y, float z);
	explicit Vector(float XYZ);
	Vector(float *xyz) { x = xyz[0]; y = xyz[1]; z = xyz[2]; }
	Vector(const Vector &v) { x = v.x; y = v.y; z = v.z; }
	Vector(Vector &&v) : x(0), y(0), z(0)
	{
		x = v.x;
		y = v.y;
		z = v.z;

		v.x = 0;
		v.y = 0;
		v.z = 0;
	}

	~Vector() {};
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);

	bool IsValid() const;
	void Invalidate();

	float   operator[](int i) const;
	float&  operator[](int i);

	float* Base();
	float const* Base() const;

	inline void Zero();

	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;
	bool operator!() { return !x && !y && !z; }

	FORCEINLINE Vector& operator+=(const Vector& v);
	FORCEINLINE Vector& operator-=(const Vector& v);
	FORCEINLINE Vector& operator*=(const Vector& v);
	FORCEINLINE Vector& operator*=(float s);
	FORCEINLINE Vector& operator/=(const Vector& v);
	FORCEINLINE Vector& operator/=(float s);
	FORCEINLINE Vector& operator+=(float fl);
	FORCEINLINE Vector& operator-=(float fl);
	inline float Long() { return sqrt2(x*x + y*y + z*z); }

	void Negate();

	inline float Length() const;

	inline Vector Angle(Vector* up = 0)
	{
		if (!x && !y)
			return Vector(0, 0, 0);

		float roll = 0;

		if (up)
		{
			Vector left = (*up).Cross(*this);

			roll = deg(atan2f(left.z, (left.y * x) - (left.x * y)));
		}

		return Vector(deg(atan2f(-z, sqrtf(x*x + y*y))), deg(atan2f(y, x)), roll);
	}

	inline Vector Forward()
	{
		float cp, cy, sp, sy;

		SinCosX(rad(x), sp, cp);
		SinCosX(rad(y), sy, cy);

		return Vector(cp*cy, cp*sy, -sp);
	}

	FORCEINLINE float LengthSqr(void) const
	{
		return (x * x + y * y + z * z);
	}

	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}
	inline void clearVekt()
	{
		this->x = this->y = this->z = 0.0f;
	}

	float Vector::Size()
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}


	float NormalizeInPlace();
	Vector Normalized() const
	{
		Vector norm = *this;

		float radius = FastSqrt(norm.x * norm.x + norm.y * norm.y + norm.z * norm.z);

		// FLT_EPSILON is added to the radius to eliminate the possibility of divide by zero.
		float iradius = 1.f / (radius + FLT_EPSILON);

		norm.x *= iradius;
		norm.y *= iradius;
		norm.z *= iradius;

		return norm;
	}
	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float val) const;

	FORCEINLINE bool WithinAABox(Vector const &boxmin, Vector const &boxmax);

	float DistTo(const Vector& vOther) const;

	FORCEINLINE float DistToSqr(const Vector& vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	void CopyToArray(float* rgfl) const;
	void MulAdd(const Vector& a, const Vector& b, float scalar);

	float Dot(const Vector& vOther) const;
	float	Dot(const float* fOther) const;

	Vector& operator=(const Vector& vOther);

	float Length2D(void) const;
	float Length2DSqr(void) const;

	Vector operator-(void) const;
	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;
	Vector operator*(const Vector& v) const;
	Vector operator/(const Vector& v) const;
	Vector operator*(float fl) const;
	Vector operator/(float fl) const;

	Vector Cross(const Vector& vOther) const;

	Vector Min(const Vector& vOther) const;
	Vector Max(const Vector& vOther) const;
};

//using QAngle = Vector;
typedef Vector QAngle;

FORCEINLINE Vector ReplicateToVector(float x)
{
	return Vector(x, x, x);
}

inline Vector::Vector(void)
{

}

inline Vector::Vector(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
}

inline Vector::Vector(float XYZ)
{
	x = y = z = XYZ;
}

inline void Vector::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
}

inline void Vector::Zero()
{
	x = y = z = 0.0f;
}

inline void VectorClear(Vector& a)
{
	a.x = a.y = a.z = 0.0f;
}

inline Vector& Vector::operator=(const Vector& vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

inline float& Vector::operator[](int i)
{
	return ((float*)this)[i];
}

inline float Vector::operator[](int i) const
{
	return ((float*)this)[i];
}



inline float* Vector::Base()
{
	return (float*)this;
}

inline float const* Vector::Base() const
{
	return (float const*)this;
}

inline bool Vector::IsValid() const
{
	return (x == x && y == y && z == z);
}

inline void Vector::Invalidate()
{
	x = y = z = VEC_T_NAN;
}

inline bool Vector::operator==(const Vector& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool Vector::operator!=(const Vector& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z);
}

FORCEINLINE void VectorCopy(const Vector& src, Vector& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

inline void Vector::CopyToArray(float* rgfl) const
{
	rgfl[0] = x; rgfl[1] = y; rgfl[2] = z;
}

inline void Vector::Negate()
{
	x = -x; y = -y; z = -z;
}

FORCEINLINE Vector& Vector::operator+=(const Vector& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator-=(const Vector& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

FORCEINLINE Vector& Vector::operator*=(const Vector& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator+=(float fl)
{
	x += fl;
	y += fl;
	z += fl;
	return *this;
}

FORCEINLINE Vector& Vector::operator-=(float fl)
{
	x -= fl;
	y -= fl;
	z -= fl;
	return *this;
}

FORCEINLINE Vector& Vector::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}

FORCEINLINE Vector& Vector::operator/=(const Vector& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

FORCEINLINE void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

FORCEINLINE void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

FORCEINLINE void VectorMultiply(const Vector& a, float b, Vector& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

FORCEINLINE void VectorMultiply(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}

inline void VectorScale(const Vector& in, float scale, Vector& result)
{
	VectorMultiply(in, scale, result);
}

FORCEINLINE void VectorDivide(const Vector& a, float b, Vector& c)
{
	float oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
}

FORCEINLINE void VectorDivide(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
}

inline void Vector::MulAdd(const Vector& a, const Vector& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
}

inline void VectorLerp(const Vector& src1, const Vector& src2, float t, Vector& dest)
{
	dest.x = src1.x + (src2.x - src1.x) * t;
	dest.y = src1.y + (src2.y - src1.y) * t;
	dest.z = src1.z + (src2.z - src1.z) * t;
}

FORCEINLINE float DotProduct(const Vector& a, const Vector& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline float Vector::Dot(const Vector& vOther) const
{
	return DotProduct(*this, vOther);
}
inline float Vector::Dot(const float* fOther) const
{
	const Vector& a = *this;

	return(a.x*fOther[0] + a.y*fOther[1] + a.z*fOther[2]);
}

inline void CrossProduct(const Vector& a, const Vector& b, Vector& result)
{
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}

//inline float DotProductAbs(const Vector& v0, const Vector& v1)
//{
//	return abs(v0.x * v1.x) + abs(v0.y * v1.y) + abs(v0.z * v1.z);
//}

inline float VectorLength(const Vector& v)
{
	return (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline float Vector::Length(void) const
{
	return VectorLength(*this);
}

inline float VectorNormalize(Vector& v)
{
	float l = v.Length();

	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f; v.z = 1.0f;
	}

	return l;
}

FORCEINLINE float VectorNormalizer(float * v)
{
	return VectorNormalize(*(reinterpret_cast<Vector *>(v)));
}
inline float Vector::NormalizeInPlace()
{
	return VectorNormalize(*this);
}

bool Vector::WithinAABox(Vector const &boxmin, Vector const &boxmax)
{
	return (
		(x >= boxmin.x) && (x <= boxmax.x) &&
		(y >= boxmin.y) && (y <= boxmax.y) &&
		(z >= boxmin.z) && (z <= boxmax.z)
		);
}

inline float Vector::DistTo(const Vector& vOther) const
{
	Vector delta;
	VectorSubtract(*this, vOther, delta);
	return delta.Length();
}

inline Vector Vector::Min(const Vector& vOther) const
{
	return Vector(x < vOther.x ? x : vOther.x,
		y < vOther.y ? y : vOther.y,
		z < vOther.z ? z : vOther.z);
}

inline Vector Vector::Max(const Vector& vOther) const
{
	return Vector(x > vOther.x ? x : vOther.x,
		y > vOther.y ? y : vOther.y,
		z > vOther.z ? z : vOther.z);
}

inline Vector Vector::operator-(void) const
{
	return Vector(-x, -y, -z);
}

inline Vector Vector::operator+(const Vector& v) const
{
	Vector res;
	VectorAdd(*this, v, res);
	return res;
}

inline Vector Vector::operator-(const Vector& v) const
{
	Vector res;
	VectorSubtract(*this, v, res);
	return res;
}

inline Vector Vector::operator*(float fl) const
{
	Vector res;
	VectorMultiply(*this, fl, res);
	return res;
}

inline Vector Vector::operator*(const Vector& v) const
{
	Vector res;
	VectorMultiply(*this, v, res);
	return res;
}

inline Vector Vector::operator/(float fl) const
{
	Vector res;
	VectorDivide(*this, fl, res);
	return res;
}

inline Vector Vector::operator/(const Vector& v) const
{
	Vector res;
	VectorDivide(*this, v, res);
	return res;
}

inline Vector operator*(float fl, const Vector& v)
{
	return v * fl;
}

inline Vector Vector::Cross(const Vector& vOther) const
{
	Vector res;
	CrossProduct(*this, vOther, res);
	return res;
}

inline float Vector::Length2D(void) const
{
	return (float)::sqrtf(x * x + y * y);
}

inline float Vector::Length2DSqr(void) const
{
	return (x * x + y * y);
}

inline Vector CrossProduct(const Vector& a, const Vector& b)
{
	return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline void VectorMin(const Vector& a, const Vector& b, Vector& result)
{
	result.x = min(a.x, b.x);
	result.y = min(a.y, b.y);
	result.z = min(a.z, b.z);
}

inline void VectorMax(const Vector& a, const Vector& b, Vector& result)
{
	result.x = max(a.x, b.x);
	result.y = max(a.y, b.y);
	result.z = max(a.z, b.z);
}


class VectorAligned : public Vector
{
public:
	VectorAligned()
	{
		x = y = z = 0.0f;
	}

	VectorAligned(const Vector& v)
	{
		x = v.x; y = v.y; z = v.z;
	}

	float w;
};



#pragma once

#define CHECK_VALID(_v) 0
#define Assert(_exp) ((void)0)

class Vector3
{
public:

	float x1, y1, z1;

	Vector3() {};
	Vector3(float x1, float y1, float z1) : x1{ x1 }, y1{ y1 }, z1{ z1 } {}
	Vector3(float *xyz) { x1 = xyz[0]; y1 = xyz[1]; z1 = xyz[2]; }
	Vector3(const Vector3 &v) { x1 = v.x1; y1 = v.y1; z1 = v.z1; }
	Vector3(Vector3 &&v) : x1(0), y1(0), z1(0)
	{
		x1 = v.x1;
		y1 = v.y1;
		z1 = v.z1;

		v.x1 = 0;
		v.y1 = 0;
		v.z1 = 0;
	}

	~Vector3() {};

	inline void init(float x1, float y1, float z1)
	{
		this->x1 = x1; this->y1 = y1; this->z1 = z1;
	}

	inline float *base()
	{
		return (float*)this;
	}

	inline float const *base() const
	{
		return (float const*)this;
	}

	inline float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	inline float &operator[](int i)
	{
		return ((float*)this)[i];
	}

	inline float length()
	{
		float root = 0.0f;

		float sqsr = (this->x1 * this->x1) + (this->y1 * this->y1) + (this->z1 * this->z1);

		_asm
		{
			sqrtss xmm0, sqsr
			movss root, xmm0
		}

		return root;
	}

	inline Vector3 normalize()
	{
		Vector3 vekt;
		float len = this->length();

		if (len != 0)
		{
			vekt.x1 = this->x1 / len;
			vekt.y1 = this->y1 / len;
			vekt.z1 = this->z1 / len;
		}
		else
		{
			vekt.x1 = vekt.y1 = 0.0f;
			vekt.z1 = 1.0f;
		}

		return vekt;
	}

	inline float normalizeInPlace()
	{
		Vector3 &vekt = *this;

		float iradius = 1.f / (this->length() + 1.192092896e-07F);

		vekt.x1 *= iradius;
		vekt.y1 *= iradius;
		vekt.z1 *= iradius;
	}

	inline float length2d()
	{
		float root = 0.0f;
		float sqs = (this->x1 * this->x1) + (this->y1 * this->y1);

		_asm
		{
			sqrtss xmm0, sqs
			movss root, xmm0
		}
		return root;
	}

	inline float lengthSqr()
	{
		return (pow(this->x1, 2) + pow(this->y1, 2) + pow(this->z1, 2));
	}

	inline void clearVekt()
	{
		this->x1 = this->y1 = this->z1 = 0.0f;
	}

	inline bool Vector3::operator==(const Vector3 &src) const
	{
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.x1 == x1) && (src.y1 == y1) && (src.z1 == z1);
	}

	inline Vector3 operator+(const Vector3 &v) const
	{
		return Vector3(this->x1 + v.x1, this->y1 + v.y1, this->z1 + v.z1);
	}

	inline Vector3 operator-(const Vector3 &v) const
	{
		return Vector3(this->x1 - v.x1, this->y1 - v.y1, this->z1 - v.z1);
	}

	inline Vector3 operator*(const Vector3 &v) const
	{
		return Vector3(this->x1 * v.x1, this->y1 * v.y1, this->z1 * v.z1);
	}

	inline Vector3 operator*(float f) const
	{
		return Vector3(this->x1 * f, this->y1 * f, this->z1 * f);
	}

	inline Vector3 operator/(const Vector3 &v) const
	{
		return Vector3(this->x1 / v.x1, this->y1 / v.y1, this->z1 / v.z1);
	}

	inline Vector3 operator/(float f) const
	{
		return Vector3(this->x1 / f, this->y1 / f, this->z1 / f);
	}

	inline Vector3 &operator+=(const Vector3 &v)
	{
		this->x1 += v.x1;
		this->y1 += v.y1;
		this->z1 += v.z1;

		return *this;
	}

	inline Vector3 &operator-=(const Vector3 &v)
	{
		this->x1 -= v.x1;
		this->y1 -= v.y1;
		this->z1 -= v.z1;

		return *this;
	}

	inline Vector3 &operator*=(const Vector3 &v)
	{
		this->x1 *= v.x1;
		this->y1 *= v.y1;
		this->z1 *= v.z1;

		return *this;
	}

	inline Vector3 &operator/=(const Vector3 &v)
	{
		this->x1 /= v.x1;
		this->y1 /= v.y1;
		this->z1 /= v.z1;

		return *this;
	}

	inline Vector3 &operator*=(float f)
	{
		this->x1 *= f;
		this->y1 *= f;
		this->z1 *= f;

		return *this;
	}

	inline Vector3 &operator/=(float f)
	{
		this->x1 /= f;
		this->y1 /= f;
		this->z1 /= f;

		return *this;
	}

	inline bool operator==(const Vector3 &src)
	{
		return (src.x1 == x1) && (src.y1 == y1) && (src.z1 == z1);
	}

	inline bool operator!=(const Vector3 &src)
	{
		return (src.x1 != x1) || (src.y1 != y1) || (src.z1 != z1);
	}

	inline bool operator!()
	{
		return !x1 && !y1 && !z1;
	}

	inline Vector3 &operator=(Vector3 &&v)
	{
		if (this != &v)
		{
			x1 = 0;
			y1 = 0;
			z1 = 0;

			x1 = v.x1;
			y1 = v.y1;
			z1 = v.z1;

			v.x1 = 0;
			v.y1 = 0;
			v.z1 = 0;
		}

		return *this;
	}

	inline Vector3 &operator=(const Vector3 &v)
	{
		if (this != &v)
		{
			x1 = v.x1;
			y1 = v.y1;
			z1 = v.z1;
		}

		return *this;
	}
};

inline Vector3 operator*(float fl, const Vector3& v) { return v * fl; }

class __declspec(align(16)) Vector3Aligned : public Vector3
{
public:

	Vector3Aligned() {}
	Vector3Aligned(float x1, float y1, float z1)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->z1 = z1;
	}

	explicit Vector3Aligned(const Vector3 &v)
	{
		this->x1 = v.x1;
		this->y1 = v.y1;
		this->z1 = v.z1;
	}

	inline Vector3Aligned &operator=(const Vector3 &v)
	{
		this->x1 = v.x1;
		this->y1 = v.y1;
		this->z1 = v.z1;

		return *this;
	}

	inline Vector3Aligned &operator=(const Vector3Aligned &v)
	{
		_mm_store_ps(this->base(), _mm_load_ps(v.base()));

		return *this;
	}

	float w;
};

inline void VectorMA(const Vector3 &start, float scale, const Vector3 &direction, Vector3 &dest)
{
	CHECK_VALID(start);
	CHECK_VALID(direction);

	dest.x1 = start.x1 + scale * direction.x1;
	dest.y1 = start.y1 + scale * direction.y1;
	dest.z1 = start.z1 + scale * direction.z1;
}

typedef unsigned __int32		uint32;

inline uint32 const FloatBits(const float &f)
{
	union Convertor_t
	{
		float f;
		uint32 ul;
	}tmp;
	tmp.f = f;
	return tmp.ul;
}

inline bool IsFinite(const float &f)
{
#if _X360
	return f == f && fabs(f) <= FLT_MAX;
#else
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
#endif
}

inline void VectorMultiply(const Vector3 &a, float b, Vector3 &c)
{
	CHECK_VALID(a);
	Assert(IsFinite(b));
	c.x1 = a.x1 * b;
	c.y1 = a.y1 * b;
	c.z1 = a.z1 * b;
}

inline void CrossProduct(const Vector3 &a, const Vector3 &b, Vector3 &result)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	Assert(&a != &result);
	Assert(&b != &result);
	result.x1 = a.y1*b.z1 - a.z1*b.y1;
	result.y1 = a.z1*b.x1 - a.x1*b.z1;
	result.z1 = a.x1*b.y1 - a.y1*b.x1;
}

inline void VectorAdd(const Vector3 &a, const Vector3 &b, Vector3 &c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x1 = a.x1 + b.x1;
	c.y1 = a.y1 + b.y1;
	c.z1 = a.z1 + b.z1;
}

inline void VectorSubtract(const Vector3 &a, const Vector3 &b, Vector3 &c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x1 = a.x1 - b.x1;
	c.y1 = a.y1 - b.y1;
	c.z1 = a.z1 - b.z1;
}

inline float VectorNormalize(Vector3 &v)
{
	float l = v.length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x1 = v.y1 = v.z1 = 0.0f;
	}
	return l;
}