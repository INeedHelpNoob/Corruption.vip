
#pragma once

#include "Vector.h"
#include "SDK.h"
#include <stdint.h>

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define RADPI 57.295779513082f
#define rad(a) a * 0.01745329251

void AngleVectors(const Vector &angles, Vector *forward);
void SinCos(float a, float* s, float*c);
void VectorTransform(Vector &, matrix3x4a_t &, Vector &);

//void SinCos1(const float rad, float &sin, float &cos);
void VectorAngles(Vector forward, Vector &angles);
void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up);
void Normalize(Vector &vIn, Vector &vOut);
float NormalizeYaw(float value);
void CalcAngle(Vector src, Vector dst, Vector &angles);
bool IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID);
bool PointVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID, Vector Point);
void CalcAngleYawOnly(Vector src, Vector dst, Vector &angles);
void NormalizeVector(Vector& vec);
float DistanceTo(Vector vecDst, Vector vecSrc);


inline Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}
inline float distance_point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}