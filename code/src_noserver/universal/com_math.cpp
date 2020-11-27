#include "com_math.h"

#include <cmath>
#include <universal/assertive.h>
#include <universal/com_math_anglevectors.h>
#include <universal/com_vector.h>

#include <defs.h>

vec3_t axisDefault[3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

vec3_t bytedirs[NUMVERTEXNORMALS] =
{
{-0.525731f, 0.000000f, 0.850651f}, {-0.442863f, 0.238856f, 0.864188f},
{-0.295242f, 0.000000f, 0.955423f}, {-0.309017f, 0.500000f, 0.809017f},
{-0.162460f, 0.262866f, 0.951056f}, {0.000000f, 0.000000f, 1.000000f},
{0.000000f, 0.850651f, 0.525731f}, {-0.147621f, 0.716567f, 0.681718f},
{0.147621f, 0.716567f, 0.681718f}, {0.000000f, 0.525731f, 0.850651f},
{0.309017f, 0.500000f, 0.809017f}, {0.525731f, 0.000000f, 0.850651f},
{0.295242f, 0.000000f, 0.955423f}, {0.442863f, 0.238856f, 0.864188f},
{0.162460f, 0.262866f, 0.951056f}, {-0.681718f, 0.147621f, 0.716567f},
{-0.809017f, 0.309017f, 0.500000f},{-0.587785f, 0.425325f, 0.688191f},
{-0.850651f, 0.525731f, 0.000000f},{-0.864188f, 0.442863f, 0.238856f},
{-0.716567f, 0.681718f, 0.147621f},{-0.688191f, 0.587785f, 0.425325f},
{-0.500000f, 0.809017f, 0.309017f}, {-0.238856f, 0.864188f, 0.442863f},
{-0.425325f, 0.688191f, 0.587785f}, {-0.716567f, 0.681718f, -0.147621f},
{-0.500000f, 0.809017f, -0.309017f}, {-0.525731f, 0.850651f, 0.000000f},
{0.000000f, 0.850651f, -0.525731f}, {-0.238856f, 0.864188f, -0.442863f},
{0.000000f, 0.955423f, -0.295242f}, {-0.262866f, 0.951056f, -0.162460f},
{0.000000f, 1.000000f, 0.000000f}, {0.000000f, 0.955423f, 0.295242f},
{-0.262866f, 0.951056f, 0.162460f}, {0.238856f, 0.864188f, 0.442863f},
{0.262866f, 0.951056f, 0.162460f}, {0.500000f, 0.809017f, 0.309017f},
{0.238856f, 0.864188f, -0.442863f},{0.262866f, 0.951056f, -0.162460f},
{0.500000f, 0.809017f, -0.309017f},{0.850651f, 0.525731f, 0.000000f},
{0.716567f, 0.681718f, 0.147621f}, {0.716567f, 0.681718f, -0.147621f},
{0.525731f, 0.850651f, 0.000000f}, {0.425325f, 0.688191f, 0.587785f},
{0.864188f, 0.442863f, 0.238856f}, {0.688191f, 0.587785f, 0.425325f},
{0.809017f, 0.309017f, 0.500000f}, {0.681718f, 0.147621f, 0.716567f},
{0.587785f, 0.425325f, 0.688191f}, {0.955423f, 0.295242f, 0.000000f},
{1.000000f, 0.000000f, 0.000000f}, {0.951056f, 0.162460f, 0.262866f},
{0.850651f, -0.525731f, 0.000000f},{0.955423f, -0.295242f, 0.000000f},
{0.864188f, -0.442863f, 0.238856f}, {0.951056f, -0.162460f, 0.262866f},
{0.809017f, -0.309017f, 0.500000f}, {0.681718f, -0.147621f, 0.716567f},
{0.850651f, 0.000000f, 0.525731f}, {0.864188f, 0.442863f, -0.238856f},
{0.809017f, 0.309017f, -0.500000f}, {0.951056f, 0.162460f, -0.262866f},
{0.525731f, 0.000000f, -0.850651f}, {0.681718f, 0.147621f, -0.716567f},
{0.681718f, -0.147621f, -0.716567f},{0.850651f, 0.000000f, -0.525731f},
{0.809017f, -0.309017f, -0.500000f}, {0.864188f, -0.442863f, -0.238856f},
{0.951056f, -0.162460f, -0.262866f}, {0.147621f, 0.716567f, -0.681718f},
{0.309017f, 0.500000f, -0.809017f}, {0.425325f, 0.688191f, -0.587785f},
{0.442863f, 0.238856f, -0.864188f}, {0.587785f, 0.425325f, -0.688191f},
{0.688191f, 0.587785f, -0.425325f}, {-0.147621f, 0.716567f, -0.681718f},
{-0.309017f, 0.500000f, -0.809017f}, {0.000000f, 0.525731f, -0.850651f},
{-0.525731f, 0.000000f, -0.850651f}, {-0.442863f, 0.238856f, -0.864188f},
{-0.295242f, 0.000000f, -0.955423f}, {-0.162460f, 0.262866f, -0.951056f},
{0.000000f, 0.000000f, -1.000000f}, {0.295242f, 0.000000f, -0.955423f},
{0.162460f, 0.262866f, -0.951056f}, {-0.442863f, -0.238856f, -0.864188f},
{-0.309017f, -0.500000f, -0.809017f}, {-0.162460f, -0.262866f, -0.951056f},
{0.000000f, -0.850651f, -0.525731f}, {-0.147621f, -0.716567f, -0.681718f},
{0.147621f, -0.716567f, -0.681718f}, {0.000000f, -0.525731f, -0.850651f},
{0.309017f, -0.500000f, -0.809017f}, {0.442863f, -0.238856f, -0.864188f},
{0.162460f, -0.262866f, -0.951056f}, {0.238856f, -0.864188f, -0.442863f},
{0.500000f, -0.809017f, -0.309017f}, {0.425325f, -0.688191f, -0.587785f},
{0.716567f, -0.681718f, -0.147621f}, {0.688191f, -0.587785f, -0.425325f},
{0.587785f, -0.425325f, -0.688191f}, {0.000000f, -0.955423f, -0.295242f},
{0.000000f, -1.000000f, 0.000000f}, {0.262866f, -0.951056f, -0.162460f},
{0.000000f, -0.850651f, 0.525731f}, {0.000000f, -0.955423f, 0.295242f},
{0.238856f, -0.864188f, 0.442863f}, {0.262866f, -0.951056f, 0.162460f},
{0.500000f, -0.809017f, 0.309017f}, {0.716567f, -0.681718f, 0.147621f},
{0.525731f, -0.850651f, 0.000000f}, {-0.238856f, -0.864188f, -0.442863f},
{-0.500000f, -0.809017f, -0.309017f}, {-0.262866f, -0.951056f, -0.162460f},
{-0.850651f, -0.525731f, 0.000000f}, {-0.716567f, -0.681718f, -0.147621f},
{-0.716567f, -0.681718f, 0.147621f}, {-0.525731f, -0.850651f, 0.000000f},
{-0.500000f, -0.809017f, 0.309017f}, {-0.238856f, -0.864188f, 0.442863f},
{-0.262866f, -0.951056f, 0.162460f}, {-0.864188f, -0.442863f, 0.238856f},
{-0.809017f, -0.309017f, 0.500000f}, {-0.688191f, -0.587785f, 0.425325f},
{-0.681718f, -0.147621f, 0.716567f}, {-0.442863f, -0.238856f, 0.864188f},
{-0.587785f, -0.425325f, 0.688191f}, {-0.309017f, -0.500000f, 0.809017f},
{-0.147621f, -0.716567f, 0.681718f}, {-0.425325f, -0.688191f, 0.587785f},
{-0.162460f, -0.262866f, 0.951056f}, {0.442863f, -0.238856f, 0.864188f},
{0.162460f, -0.262866f, 0.951056f}, {0.309017f, -0.500000f, 0.809017f},
{0.147621f, -0.716567f, 0.681718f}, {0.000000f, -0.525731f, 0.850651f},
{0.425325f, -0.688191f, 0.587785f}, {0.587785f, -0.425325f, 0.688191f},
{0.688191f, -0.587785f, 0.425325f}, {-0.955423f, 0.295242f, 0.000000f},
{-0.951056f, 0.162460f, 0.262866f}, {-1.000000f, 0.000000f, 0.000000f},
{-0.850651f, 0.000000f, 0.525731f}, {-0.955423f, -0.295242f, 0.000000f},
{-0.951056f, -0.162460f, 0.262866f}, {-0.864188f, 0.442863f, -0.238856f},
{-0.951056f, 0.162460f, -0.262866f}, {-0.809017f, 0.309017f, -0.500000f},
{-0.864188f, -0.442863f, -0.238856f}, {-0.951056f, -0.162460f, -0.262866f},
{-0.809017f, -0.309017f, -0.500000f}, {-0.681718f, 0.147621f, -0.716567f},
{-0.681718f, -0.147621f, -0.716567f}, {-0.850651f, 0.000000f, -0.525731f},
{-0.688191f, 0.587785f, -0.425325f}, {-0.587785f, 0.425325f, -0.688191f},
{-0.425325f, 0.688191f, -0.587785f}, {-0.425325f, -0.688191f, -0.587785f},
{-0.587785f, -0.425325f, -0.688191f}, {-0.688191f, -0.587785f, -0.425325f}
};

float identityMatrix44[] =
{ 1.0, 0.0, 0.0, 0.0,
   0.0, 1.0, 0.0, 0.0,
   0.0, 0.0, 1.0, 0.0,
   0.0, 0.0, 0.0, 1.0
};

float identityMatrix33[] =
{ 1.0, 0.0, 0.0,
   0.0, 1.0, 0.0,
   0.0, 0.0, 1.0,
};

unsigned int holdrand_0 = 2309737967;
const float DCONSTcg_hudSplitscreenCompassScale = 1.0;

/*
=================
AngleDelta
returns the normalized delta from a1 to a2
=================
*/
double AngleDelta(const float a1, const float a2)
{
	double v2; // st7
	float a1a; // [esp+10h] [ebp+8h]

	v2 = (a1 - a2) * 0.0027777778;
	a1a = v2;
	return (a1a - floor(v2 + 0.5)) * 360.0;
}

double I_normCDF(double x)
{
	long double v1; // st7
	long double t; // [esp+0h] [ebp-Ch]
	int sign; // [esp+8h] [ebp-4h]
	long double xa; // [esp+14h] [ebp+8h]

	sign = 1;
	if (x < 0.0)
		sign = -1;
	xa = x / sqrtf(2.0);
	t = 1.0 / (xa * 0.3275911 + 1.0);
	v1 = -(xa * xa) * 1.442695040888963387;
	return ((1.0
		- ((2 ^ (int)v1) - v1 + 1.0)
		* (t
			* ((((1.061405429 * t - 1.453152027) * t + 1.421413741) * t - 0.284496736) * t + 0.254829592)))
		* sign
		+ 1.0)
		* 0.5;
}

float random(void)
{
	return (double)rand() * 0.000030517578;
}

float crandom(void)
{
	int randInt;

	randInt = rand();
	return (double)randInt * 0.000030517578 + (double)randInt * 0.000030517578 - 1.0;
}

int RandWithSeed(int* seed)
{
	int v1; // eax

	v1 = 0x41C64E6D * *seed + 12345;
	*seed = v1;
	return (v1 / 0x10000) & 0x7FFF;
}

float LinearTrack(float tgt, float cur, float rate, float deltaTime)
{
	float fVar1;
	float fVar2;

	fVar1 = rate * deltaTime;
	if (tgt - cur <= 0.0) {
		fVar1 = (float)((unsigned int)fVar1 ^ 0x80000000);
	}
	fVar2 = (float)((unsigned int)(tgt - cur) & 0x7fffffff);
	if ((0.001 < fVar2) && ((float)((unsigned int)fVar1 & 0x7fffffff) <= fVar2)) {
		return fVar1 + cur;
	}
	return tgt;
}

float LinearTrackAngle(float tgt, float cur, float rate, float deltaTime)
{
	float fVar1;
	float fVar2;
	double dVar3;

	fVar1 = tgt - cur;
	while (180.0 < fVar1) {
		tgt = tgt - 360.0;
		fVar1 = tgt - cur;
	}
	while (tgt - cur < -180.0) {
		tgt = tgt + 360.0;
	}
	fVar1 = rate * deltaTime;
	if (tgt - cur <= 0.0) {
		fVar1 = (float)((unsigned int)fVar1 ^ 0x80000000);
	}
	fVar2 = (float)((unsigned int)(tgt - cur) & 0x7fffffff);
	if ((0.001 < fVar2) && ((float)((unsigned int)fVar1 & 0x7fffffff) <= fVar2)) {
		tgt = fVar1 + cur;
	}
	dVar3 = floor((double)(tgt * 0.002777778 + 0.5));
	return (tgt * 0.002777778 - (float)dVar3) * 360.0;
}

float DiffTrack(float tgt, float cur, float rate, float deltaTime)
{
	float fVar1;
	float fVar2;

	fVar1 = (tgt - cur) * rate * deltaTime;
	fVar2 = (float)((unsigned int)(tgt - cur) & 0x7fffffff);
	if ((0.001 < fVar2) && ((float)((unsigned int)fVar1 & 0x7fffffff) <= fVar2)) {
		return fVar1 + cur;
	}
	return tgt;
}

float DiffTrackAngle(float tgt, float cur, float rate, float deltaTime)
{
	float fVar1;
	float fVar2;
	double dVar3;

	fVar1 = tgt - cur;
	while (180.0 < fVar1) {
		tgt = tgt - 360.0;
		fVar1 = tgt - cur;
	}
	while (tgt - cur < -180.0) {
		tgt = tgt + 360.0;
	}
	fVar2 = (tgt - cur) * rate * deltaTime;
	fVar1 = (float)((unsigned int)(tgt - cur) & 0x7fffffff);
	if ((0.001 < fVar1) && ((float)((unsigned int)fVar2 & 0x7fffffff) <= fVar1)) {
		tgt = fVar2 + cur;
	}
	dVar3 = floor((double)(tgt * 0.002777778 + 0.5));
	return (tgt * 0.002777778 - (float)dVar3) * 360.0;
}

float GraphGetValueFromFraction(int knotCount, vec2_t const* knots, float fraction)
{
	float fVar1;
	bool bVar3;
	int iVar4;
	float fVar5;
	float fVar6;
	float result;

	result = -1.0;
	if ((knots == (vec2_t*)0x0) &&
		(bVar3 = Assert_MyHandler(__FILE__, __LINE__, 0,
			"(knots)", ""), bVar3 == false)) {
		__debugbreak();
	}
	if ((knotCount < 2) &&
		(bVar3 = Assert_MyHandler(__FILE__, __LINE__, 0,
			"((knotCount >= 2))", "(knotCount) = %i"), bVar3 == false)) {
		__debugbreak();
	}
	if (((fraction < 0.0) || (1.0 < fraction)) &&
		(bVar3 = Assert_MyHandler(__FILE__, __LINE__, 0,
			"((fraction >= 0.0f && fraction <= 1.0f))", "(fraction) = %g"),
			bVar3 == false)) {
		__debugbreak();
	}
	if ((*(float*)(knots + knotCount + -1) != 1.0) &&
		(bVar3 = Assert_MyHandler(__FILE__, __LINE__, 0,
			"((knots[knotCount - 1][0] == 1.0f))",
			"(knots[knotCount - 1][0]) = %g"), bVar3 == false)) {
		__debugbreak();
	}
	iVar4 = 1;
	if (1 < knotCount) {
		do {
			if (fraction <= *(float*)(knots + iVar4)) {
				fVar6 = (fraction - *(float*)(knots + iVar4 + -1)) /
					(*(float*)(knots + iVar4) - *(float*)(knots + iVar4 + -1));
				if (((fVar6 < 0.0) || (1.0 < fVar6)) &&
					(bVar3 = Assert_MyHandler(__FILE__, __LINE__, 0,
						"((adjustedFrac >= 0.0f && adjustedFrac <= 1.0f))",
						"(adjustedFrac) = %g"), bVar3 == false)) {
					__debugbreak();
				}
				fVar1 = *(float*)((int)knots + iVar4 * 8 + -4);
				if (((fVar1 < 0.0) || (1.0 < fVar1)) &&
					(bVar3 = Assert_MyHandler(__FILE__, __LINE__, 0,

						"((knots[knotIndex - 1][1] >= 0.0f && knots[knotIndex - 1][1]<= 1.0f))"
						, "(knots[knotIndex - 1][1]) = %g"), bVar3 == false)) {
					__debugbreak();
				}
				fVar1 = *(float*)((int)knots + iVar4 * 8 + 4);
				if (((fVar1 < 0.0) || (1.0 < fVar1)) &&
					(bVar3 = Assert_MyHandler(__FILE__, __LINE__, 0,

						"((knots[knotIndex][1] >= 0.0f && knots[knotIndex][1] <=1.0f))"
						, "(knots[knotIndex][1]) = %g"), bVar3 == false)) {
					__debugbreak();
				}
				result = (*(float*)((int)knots + iVar4 * 8 + 4) - *(float*)((int)knots + iVar4 * 8 + -4))
					* fVar6 + *(float*)((int)knots + iVar4 * 8 + -4);
				if ((0.0 <= result) && (result <= 1.0)) {
					return result;
				}
				break;
			}
			iVar4 = iVar4 + 1;
		} while (iVar4 < knotCount);
	}
	bVar3 = Assert_MyHandler(__FILE__, __LINE__, 0,
		"((result >= 0.0f && result <= 1.0f))", "(result) = %g");
	if (bVar3 != false) {
		return result;
	}
	__debugbreak();
}

float Q_acos(float c)
{
	float angle;

	angle = acos(c);

	if (angle > M_PI) {
		return (float)M_PI;
	}
	if (angle < -M_PI) {
		return (float)M_PI;
	}
	return angle;
}

signed char ClampChar(int i)
{
	char result;

	result = i;
	if (i < -128)
		return 0x80;
	if (i > 127)
		result = 127;
	return result;
}

short ClampShort(int i)
{
	int result;

	result = i;
	if (i < 4294934528)
		return -32768;
	if (i > 0x7FFF)
		result = 0x7FFF;
	return result;
}

char DirToByte(vec3_t const* dir)
{
	int	i, best;
	float d, bestd;

	if ( !dir ) {
		return 0;
	}

	bestd = 0;
	best = 0;
	for (i=0 ; i<NUMVERTEXNORMALS ; i++)
	{
		d = (dir->x * bytedirs[i].x) + (dir->y * bytedirs[i].y) + (dir->z * bytedirs[i].z);	//DotProduct(dir, bytedirs[i]);
		if (d > bestd)
		{
			bestd = d;
			best = i;
		}
	}
	return best;
}

void ByteToDir(int b, vec3_t* dir)
{
	if (b > 0xA1)
	{
		*dir = vec3_origin;
	}
	else
	{
		*dir = bytedirs[b];
	}
}

int VecNCompareCustomEpsilon(const vec3_t* v0, const vec3_t* v1, float epsilon, int coordCount)
{
	int i;
	vec3_t* pvVar1;
	float fVar2;

	i = 0;
	if (0 < coordCount) {
		pvVar1 = (vec3_t*)((int)v0 - (int)v1);
		do {
			fVar2 = *(float*)((int)v1 + (int)pvVar1) - *(float*)v1;
			if (epsilon * epsilon < fVar2 * fVar2) {
				return 0;
			}
			i = i + 1;
			v1 = (vec3_t*)((int)v1 + 4);
		} while (i < coordCount);
	}
	return 1;
}

void Vec3ProjectionCoords(const vec3_t* dir, int* xCoord, int* yCoord)
{
	float v3; // xmm1_4
	float v4; // xmm0_4
	float v5; // xmm2_4

	v3 = dir->x * dir->x;
	v4 = dir->z * dir->z;
	v5 = dir->y * dir->y;
	if (v4 < v3 || v4 < v5)
	{
		if (v5 < v3 || v5 < v4)
		{
			if (dir->x <= 0.0)
			{
				*xCoord = 2;
				*yCoord = 1;
			}
			else
			{
				*xCoord = 1;
				*yCoord = 2;
			}
		}
		else if (dir->y <= 0.0)
		{
			*xCoord = 0;
			*yCoord = 2;
		}
		else
		{
			*xCoord = 2;
			*yCoord = 0;
		}
	}
	else if (dir->z <= 0.0)
	{
		*xCoord = 1;
		*yCoord = 0;
	}
	else
	{
		*xCoord = 0;
		*yCoord = 1;
	}
}

float vectoyaw(vec2_t const* vec)
{
	float yaw;

	if (vec->v[YAW] == 0 && vec->v[PITCH] == 0) {
		yaw = 0;
	}
	else {
		if (vec->v[PITCH]) {
			yaw = (atan2(vec->v[YAW], vec->v[PITCH]) * 180 / M_PI);
		}
		else if (vec->v[YAW] > 0) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if (yaw < 0) {
			yaw += 360;
		}
	}

	return yaw;
}

double vectosignedyaw(vec2_t const* vec)
{
	double yaw;

	if (vec->v[YAW] == 0 && vec->v[PITCH] == 0) {
		yaw = 0;
	}
	else {
		if (vec->v[PITCH]) {
			yaw = (atan2(vec->v[YAW], vec->v[PITCH]) * 180 / M_PI);
		}
		else if (vec->v[YAW] > 0) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if (yaw < 0) {
			yaw += 360;
		}
	}

	return yaw;
}

float vectopitch(vec3_t const* vec)
{
	float pitch;

	if (vec->y == 0 && vec->x == 0) {
		pitch = 0;
	}
	else {
		if (vec->x) {
			pitch = (atan2(vec->y, vec->x) * 180 / M_PI);
		}
		else if (vec->y > 0) {
			pitch = 90;
		}
		else {
			pitch = 270;
		}
		if (pitch < 0) {
			pitch += 360;
		}
	}

	return pitch;
}

void vectoangles(vec3_t const* vec, vec3_t* angles)
{
	float forward;
	float yaw, pitch;

	if (vec->v[1] == 0 && vec->v[0] == 0) {
		yaw = 0;
		if (vec->v[2] > 0) {
			pitch = 90;
		}
		else {
			pitch = 270;
		}
	}
	else {
		if (vec->v[0]) {
			yaw = (atan2(vec->v[1], vec->v[0]) * 180 / M_PI);
		}
		else if (vec->v[1] > 0) {
			yaw = 90;
		}
		else {
			yaw = 270;
		}
		if (yaw < 0) {
			yaw += 360;
		}

		forward = sqrt(vec->v[0] * vec->v[0] + vec->v[1] * vec->v[1]);
		pitch = (atan2(vec->v[2], forward) * 180 / M_PI);
		if (pitch < 0) {
			pitch += 360;
		}
	}

	angles->v[PITCH] = -pitch;
	angles->v[YAW] = yaw;
	angles->v[ROLL] = 0;
}

void YawVectors(float yaw, vec3_t* forward, vec3_t* right)
{
	float sy, cy;

	cy = cos(yaw * 0.017453292);
	sy = sin(yaw * 0.017453292);
	if (forward) {
		forward->x = cy;
		forward->v[YAW] = sy;
	}
	if (right) {
		right->x = sy;
		right->v[YAW] = -cy;
	}
}

void YawVectors2D(float yaw, vec2_t* forward, vec2_t* right)
{
	float sy, cy;
	cy = cos(yaw * 0.017453292);
	sy = sin(yaw * 0.017453292);
	if (forward) {
		forward->v[PITCH] = cy;
		forward->v[YAW] = sy;
	}
	if (right) {
		right->v[PITCH] = sy;
		right->v[YAW] = -cy;
	}
}

void PerpendicularVector(vec3_t const* src, vec3_t* dst)
{
	const char* vecDbg;

	float v4, v5;

	vec3_t srcSq;

	if (sqrtf((((src->x * src->x) + (src->y * src->y)) + (src->z * src->z)) - 1.0) >= 0.0020000001) {
		vecDbg = va("(%g %g %g) len %g",
			src->x,
			src->y,
			src->z,
			sqrtf(((src->x * src->x) + (src->y * src->y)) + (src->z * src->z)));
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(Vec3IsNormalized( src ))",
			"%s\n\t%s",
			"Vec3IsNormalized( src )",
			vecDbg))
			__debugbreak();
	}
	v4 = src->y * src->y;
	v5 = src->z * src->z;
	srcSq.x = src->x * src->x;
	srcSq.v[YAW] = __PAIR64__(LODWORD(v5), LODWORD(v4));
}

float PointToLineSegmentDistSq2D(vec2_t const* point, vec2_t const* start, vec2_t const* end)
{
	float v3; // xmm2_4
	float v4; // xmm1_4
	float v5; // xmm0_4
	float v6; // xmm0_4
	float v7; // xmm0_4
	float v8; // st6
	float v9; // st5
	float result; // st7
	float v11; // xmm3_4
	float v12; // st7
	float v13; // st6
	float v14; // [esp+0h] [ebp-20h]
	float ptToLine; // [esp+Ch] [ebp-14h]
	float ptToLine_4; // [esp+10h] [ebp-10h]

	v3 = end->v[0] - start->v[0];
	v4 = end->v[1] - start->v[1];
	v5 = point->v[1];
	v14 = v5;
	v6 = v5 - start->v[1];
	ptToLine_4 = v6;
	ptToLine = point->v[0] - start->v[0];
	v7 = (v6 * v4) + (ptToLine * v3);
	if (v7 >= 0.0)
	{
		v11 = (v4 * v4) + (v3 * v3);
		if (v11 == 0.0)
		{
			if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(segDot)", false))
				__debugbreak();
			v11 = (v4 * v4) + (v3 * v3);
		}
		if (v7 <= v11)
		{
			result = ((v4 * -(v7 / v11)) + ptToLine_4) * ((v4 * -(v7 / v11)) + ptToLine_4)
				+ ((-(v7 / v11) * v3) + ptToLine) * ((-(v7 / v11) * v3) + ptToLine);
		}
		else
		{
			v12 = point->v[0] - end->v[0];
			v13 = point->v[1] - end->v[1];
			result = v12 * v12 + v13 * v13;
		}
	}
	else
	{
		v8 = v14 - start->v[1];
		v9 = point->v[0] - start->v[0];
		result = v9 * v9 + v8 * v8;
	}
	return result;
}

float PointToLineSegmentDistSq(vec3_t const&, vec3_t const&, vec3_t const&)
{
	return 0.0f;
}

int BoxInPlanes(const vec4_t* planes, unsigned int numPlanes, vec3_t const* mins, vec3_t const* maxs)
{
	unsigned int v5; // edx
	float* i; // ecx
	float v7; // xmm4_4 MAPDST
	float v8; // xmm5_4
	float v9; // xmm6_4
	float v10; // xmm4_4
	float v11; // xmm1_4
	float v12; // xmm3_4
	float v13; // xmm2_4
	float v14; // xmm7_4
	float v15; // xmm1_4
	float v16; // xmm7_4
	float v17; // xmm6_4
	float v18; // xmm1_4
	float v19; // xmm4_4
	float v20; // xmm4_4
	float v21; // xmm1_4
	float v22; // xmm1_4
	float v23; // xmm4_4
	float v24; // xmm5_4
	float v25; // xmm1_4
	float v26; // xmm7_4
	float v27; // xmm7_4
	float v28; // xmm1_4
	float v29; // xmm3_4
	float v30; // xmm3_4
	float v31; // xmm1_4
	float v33; // [esp+4h] [ebp-24h]
	float v34; // [esp+8h] [ebp-20h]
	float v36; // [esp+10h] [ebp-18h]
	float v37; // [esp+18h] [ebp-10h]
	float v38; // [esp+24h] [ebp-4h]
	float numPlanesa; // [esp+34h] [ebp+Ch]

	v5 = 0;
	if (!numPlanes)
		return 1;
	v38 = mins->y;
	v36 = maxs->y;
	v34 = maxs->x;
	v37 = maxs->z;
	for (i = (float*)&planes->v[2]; ; i += 4)
	{
		v7 = *(i - 2);
		v8 = *(i - 1);
		v9 = *i;
		v10 = v7 * mins->x;
		v11 = v10 + (v8 * v38);
		v12 = *i * mins->z;
		v33 = v8 * v38;
		v13 = i[1];
		v14 = ((v11 + v12) + v13) < 0.0 ? 1.0 : 0.0;
		numPlanesa = v9 * v37;
		v15 = ((v11 + (v9 * v37)) + v13) < 0.0 ? 1.0 : 0.0;
		v16 = v15 + v14;
		v17 = v8 * v36;
		v18 = (v8 * v36) + v10;
		v19 = ((v18 + v12) + v13) < 0.0 ? 1.0 : 0.0;
		v20 = v19 + v16;
		v21 = ((v18 + numPlanesa) + v13) < 0.0 ? 1.0 : 0.0;
		v22 = v21 + v20;
		v23 = v7 * v34;
		v24 = v22;
		v25 = (v7 * v34) + v33;
		v26 = ((v25 + v12) + v13) < 0.0 ? 1.0 : 0.0;
		v27 = v26 + v24;
		v28 = ((v25 + numPlanesa) + v13) < 0.0 ? 1.0 : 0.0;
		v29 = (((v23 + v17) + v12) + v13) < 0.0 ? 1.0 : 0.0;
		v30 = v29 + (v28 + v27);
		v31 = (((v23 + v17) + numPlanesa) + v13) < 0.0 ? 1.0 : 0.0;
		if ((v31 + v30) == 0.0)
			break;
		if (++v5 >= numPlanes)
			return 1;
	}
	return 0;
}

int SphereInPlanes(const vec4_t* planes, unsigned int numPlanes, const vec3_t* center, const float radius)
{
	int v4; // eax
	float* i; // ecx

	v4 = 0;
	if (!numPlanes)
		return 1;
	for (i = (float*)&planes->v[2];
		(((((*(i - 2) * center->x) + (*(i - 1) * center->y)) + (center->z * *i)) + i[1]) - radius) <= 0.0;
		i += 4)
	{
		if (++v4 >= numPlanes)
			return 1;
	}
	return 0;
}

void MatrixIdentity33(vec3_t* out)
{
	if (!out && !Assert_MyHandler(__FILE__, __LINE__, 0, "(out)", false))
		__debugbreak();
	qmemcpy(out, identityMatrix33, 0x24u);
}

void MatrixIdentity44(vec4_t* out)
{
	if (!out && !Assert_MyHandler(__FILE__, __LINE__, 0, "(out)", false))
		__debugbreak();
	qmemcpy(out, identityMatrix44, 0x40u);
}

void MatrixMultiply(const vec3_t* in1, const vec3_t* in2, vec3_t* out)
{
	out->x = ((in1->x * in2->x) + (in2[1].x * in1->y)) + (in2[2].x * in1->z);
	out->y = ((in2[1].y * in1->y) + (in2->y * in1->x)) + (in2[2].y * in1->z);
	out->z = ((in2[1].z * in1->y) + (in2->z * in1->x)) + (in2[2].z * in1->z);
	out[1].x = ((in1[1].y * in2[1].x) + (in1[1].x * in2->x)) + (in1[1].z * in2[2].x);
	out[1].y = ((in1[1].y * in2[1].y) + (in1[1].x * in2->y)) + (in1[1].z * in2[2].y);
	out[1].z = ((in1[1].y * in2[1].z) + (in1[1].x * in2->z)) + (in1[1].z * in2[2].z);
	out[2].x = ((in2[1].x * in1[2].y) + (in2->x * in1[2].x)) + (in2[2].x * in1[2].z);
	out[2].y = ((in2[1].y * in1[2].y) + (in2->y * in1[2].x)) + (in2[2].y * in1[2].z);
	out[2].z = ((in2[1].z * in1[2].y) + (in2->z * in1[2].x)) + (in2[2].z * in1[2].z);
}

void MatrixVecMultiply(const vec3_t* mulMat, const vec3_t* mulVec, vec3_t* solution)
{
	solution->x = ((mulMat->y * mulVec->y) + (mulMat->x * mulVec->x)) + (mulMat->z * mulVec->z);
	solution->y = ((mulMat[1].y * mulVec->y) + (mulMat[1].x * mulVec->x)) + (mulMat[1].z * mulVec->z);
	solution->z = ((mulMat[2].y * mulVec->y) + (mulMat[2].x * mulVec->x)) + (mulMat[2].z * mulVec->z);
}

void MatrixVecMultiplyProject(const vec4_t* mulMat, const vec3_t* mulVec, vec3_t* solution)
{
	float v3; // xmm5_4
	float v4; // xmm6_4
	float v5; // xmm1_4
	float v6; // xmm2_4
	float v7; // xmm4_4

	v3 = mulVec->y;
	v4 = mulVec->z;
	v5 = (((mulMat[1].v[1] * v3) + (mulMat[1].v[0] * mulVec->x)) + (mulMat[1].v[2] * v4)) + mulMat[1].v[3];
	v6 = (((mulMat[2].v[1] * v3) + (mulMat[2].v[0] * mulVec->x)) + (mulMat[2].v[2] * v4)) + mulMat[2].v[3];
	v7 = 1.0 / ((((mulMat[3].v[1] * v3) + (mulMat[3].v[0] * mulVec->x)) + (mulMat[3].v[2] * v4)) + mulMat[3].v[3]);
	solution->x = ((((mulMat->v[1] * v3) + (mulMat->v[0] * mulVec->x)) + (mulMat->v[2] * v4)) + mulMat->v[3]) * v7;
	solution->y = v5 * v7;
	solution->z = v6 * v7;
}

void MatrixMultiply43(const vec3_t* in1, const vec3_t* in2, vec3_t* out)
{
	if (in1 == out
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(in1 != out)", false))
	{
		__debugbreak();
	}
	if (in2 == out
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(in2 != out)", false))
	{
		__debugbreak();
	}
	out->x = ((in1->x * in2->x) + (in2[1].x * in1->y)) + (in2[2].x * in1->z);
	out[1].x = ((in1[1].y * in2[1].x) + (in1[1].x * in2->x)) + (in1[1].z * in2[2].x);
	out[2].x = ((in1[2].y * in2[1].x) + (in1[2].x * in2->x)) + (in1[2].z * in2[2].x);
	out->y = ((in2[1].y * in1->y) + (in2->y * in1->x)) + (in2[2].y * in1->z);
	out[1].y = ((in1[1].y * in2[1].y) + (in1[1].x * in2->y)) + (in1[1].z * in2[2].y);
	out[2].y = ((in1[2].y * in2[1].y) + (in1[2].x * in2->y)) + (in1[2].z * in2[2].y);
	out->z = ((in1->x * in2->z) + (in1->y * in2[1].z)) + (in2[2].z * in1->z);
	out[1].z = ((in1[1].y * in2[1].z) + (in1[1].x * in2->z)) + (in1[1].z * in2[2].z);
	out[2].z = ((in1[2].y * in2[1].z) + (in1[2].x * in2->z)) + (in1[2].z * in2[2].z);
	out[3].x = (((in1[3].y * in2[1].x) + (in1[3].x * in2->x)) + (in1[3].z * in2[2].x)) + in2[3].x;
	out[3].y = (((in1[3].y * in2[1].y) + (in1[3].x * in2->y)) + (in1[3].z * in2[2].y)) + in2[3].y;
	out[3].z = (((in1[3].y * in2[1].z) + (in1[3].x * in2->z)) + (in1[3].z * in2[2].z)) + in2[3].z;
}

void MatrixMultiply44(const vec4_t* in1, const vec4_t* in2, vec4_t* out)
{
	out[0] = in1[0] * in2[0] + in1[1] * in2[4] + in1[2] * in2[8] + in1[3] * in2[12];
	out[1] = in1[0] * in2[1] + in1[1] * in2[5] + in1[2] * in2[9] + in1[3] * in2[13];
	out[2] = in1[0] * in2[2] + in1[1] * in2[6] + in1[2] * in2[10] + in1[3] * in2[14];
	out[3] = in1[0] * in2[3] + in1[1] * in2[7] + in1[2] * in2[11] + in1[3] * in2[15];
	out[4] = in1[4] * in2[0] + in1[5] * in2[4] + in1[6] * in2[8] + in1[7] * in2[12];
	out[5] = in1[4] * in2[1] + in1[5] * in2[5] + in1[6] * in2[9] + in1[7] * in2[13];
	out[6] = in1[4] * in2[2] + in1[5] * in2[6] + in1[6] * in2[10] + in1[7] * in2[14];
	out[7] = in1[4] * in2[3] + in1[5] * in2[7] + in1[6] * in2[11] + in1[7] * in2[15];
	out[8] = in1[8] * in2[0] + in1[9] * in2[4] + in1[10] * in2[8] + in1[11] * in2[12];
	out[9] = in1[8] * in2[1] + in1[9] * in2[5] + in1[10] * in2[9] + in1[11] * in2[13];
	out[10] = in1[8] * in2[2] + in1[9] * in2[6] + in1[10] * in2[10] + in1[11] * in2[14];
	out[11] = in1[8] * in2[3] + in1[9] * in2[7] + in1[10] * in2[11] + in1[11] * in2[15];
	out[12] = in1[12] * in2[0] + in1[13] * in2[4] + in1[14] * in2[8] + in1[15] * in2[12];
	out[13] = in1[12] * in2[1] + in1[13] * in2[5] + in1[14] * in2[9] + in1[15] * in2[13];
	out[14] = in1[12] * in2[2] + in1[13] * in2[6] + in1[14] * in2[10] + in1[15] * in2[14];
	out[15] = in1[12] * in2[3] + in1[13] * in2[7] + in1[14] * in2[11] + in1[15] * in2[15];
}

void MatrixTranspose(const vec3_t* in, vec3_t* out) //43
{
	if (in == out
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(in != out)", false))
	{
		__debugbreak();
	}
	out->x = in->x;
	out->y = in[1].x;
	out->z = in[2].x;
	out[1].x = in->y;
	out[1].y = in[1].y;
	out[1].z = in[2].y;
	out[2].x = in->z;
	out[2].y = in[1].z;
	out[2].z = in[2].z;
}

void MatrixTranspose44(const vec4_t* in, vec4_t* out)
{
	if (in == out
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "%s", "in != out"))
	{
		__debugbreak();
	}
	*out = *in;
	out[1] = in[4];
	out[2] = in[8];
	out[3] = in[12];
	out[4] = in[1];
	out[5] = in[5];
	out[6] = in[9];
	out[7] = in[13];
	out[8] = in[2];
	out[9] = in[6];
	out[10] = in[10];
	out[11] = in[14];
	out[12] = in[3];
	out[13] = in[7];
	out[14] = in[11];
	out[15] = in[15];
}

void MatrixInverse(const vec3_t* in, vec3_t* out)
{
	const vec3_t* v2; // esi
	float det; // [esp+10h] [ebp+8h]

	v2 = in;
	if (in == out
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(in != out)", false))
	{
		__debugbreak();
	}
	det = ((((in[1].y * in[2].z) - (in[1].z * in[2].y)) * in->x) - (((in->y * in[2].z) - (in->z * in[2].y)) * in[1].x)) + (((in->y * in[1].z) - (in->z * in[1].y)) * in[2].x);
	if (det == 0.0)
	{
		if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(det)", false))
			__debugbreak();
	}
	out->x = ((v2[1].y * v2[2].z) - (v2[1].z * v2[2].y)) * (1.0 / det);
	out->y = -(((v2[2].z * v2->y) - (v2[2].y * v2->z)) * (1.0 / det));
	out->z = ((v2[1].z * v2->y) - (v2[1].y * v2->z)) * (1.0 / det);
	out[1].x = -(((v2[1].x * v2[2].z) - (v2[1].z * v2[2].x)) * (1.0 / det));
	out[1].y = ((v2[2].z * v2->x) - (v2->z * v2[2].x)) * (1.0 / det);
	out[1].z = -(((v2[1].z * v2->x) - (v2[1].x * v2->z)) * (1.0 / det));
	out[2].x = ((v2[1].x * v2[2].y) - (v2[1].y * v2[2].x)) * (1.0 / det);
	out[2].y = -(((v2[2].y * v2->x) - (v2->y * v2[2].x)) * (1.0 / det));
	out[2].z = ((v2[1].y * v2->x) - (v2[1].x * v2->y)) * (1.0 / det);
}

void MatrixInverseOrthogonal43(const vec3_t* in, vec3_t* out)
{
	float v2; // xmm0_4
	float v3; // xmm1_4
	float v4; // xmm2_4
	vec3_t origin; // [esp+8h] [ebp-10h]

	MatrixTranspose(in, out);
	v2 = 0.0 - in[3].x;
	v3 = 0.0 - in[3].y;
	v4 = 0.0 - in[3].z;
	origin.x = 0.0 - in[3].x;
	*&origin.v[1] = __PAIR64__(LODWORD(v4), LODWORD(v3));
	if (&origin == &out[3])
	{
		if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(&in1 != &out)", false))
			__debugbreak();
		v4 = origin.z;
		v3 = origin.y;
		v2 = origin.x;
	}
	out[3].x = ((out[1].x * v3) + (out->x * v2)) + (out[2].x * v4);
	out[3].y = ((out->y * v2) + (out[1].y * v3)) + (out[2].y * v4);
	out[3].z = ((out->z * v2) + (out[1].z * v3)) + (out[2].z * v4);
}

void MatrixInverse44(const vec4_t* mat, vec4_t* dst)
{
	unsigned int i;
	float det;
	vec4_t row1;
	vec4_t row2;
	vec4_t row3;
	vec4_t row4;

	row1 = mat[0]; // |  x  y  z  w  |
	row2 = mat[1]; // |  x  y  z  w  |
	row3 = mat[2]; // |  x  y  z  w  |
	row4 = mat[3]; // |  x  y  z  w  |

	det = row1.w * row2.z * row3.y * row4.x - row1.z * row2.w * row3.y * row4.x -
		row1.w * row2.y * row3.z * row4.x + row1.y * row2.w * row3.z * row4.x +
		row1.z * row2.y * row3.w * row4.x - row1.y * row2.z * row3.w * row4.x -
		row1.w * row2.z * row3.x * row4.y + row1.z * row2.w * row3.x * row4.y +
		row1.w * row2.x * row3.z * row4.y - row1.x * row2.w * row3.z * row4.y -
		row1.z * row2.x * row3.w * row4.y + row1.x * row2.z * row3.w * row4.y +
		row1.w * row2.y * row3.x * row4.z - row1.y * row2.w * row3.x * row4.z -
		row1.w * row2.x * row3.y * row4.z + row1.x * row2.w * row3.y * row4.z +
		row1.y * row2.x * row3.w * row4.z - row1.x * row2.y * row3.w * row4.z -
		row1.z * row2.y * row3.x * row4.w + row1.y * row2.z * row3.x * row4.w +
		row1.z * row2.x * row3.y * row4.w - row1.x * row2.z * row3.y * row4.w -
		row1.y * row2.x * row3.z * row4.w + row1.x * row2.y * row3.z * row4.w;

	dst[0].x = 0;
}

void MatrixTransformVector44(vec4_t const&, vec4_t const* const, vec4_t&)
{
}

void MatrixTransformVector43(vec3_t const&, vec3_t const* const, vec3_t&)
{
}

void MatrixTransposeTransformVector43(vec3_t const&, vec3_t const* const, vec3_t&)
{
}

void MatrixTransformVector43Equals(vec3_t&, vec3_t const* const)
{
}

void VectorAngleMultiply(vec2_t&, float)
{
}

void UnitQuatToAxis(const vec4_t* quat, vec3_t* axis)
{
	float v2; // xmm6_4
	const char* v3; // eax
	float v4; // xmm3_4
	float v5; // xmm0_4
	float v6; // xmm4_4
	float v7; // xmm5_4
	float v8; // xmm1_4
	float v9; // xmm3_4
	float v10; // xmm0_4
	float v11; // xmm4_4
	float xw; // [esp+24h] [ebp-10h]
	float xx; // [esp+28h] [ebp-Ch]
	float xz; // [esp+2Ch] [ebp-8h]
	float yy; // [esp+30h] [ebp-4h]
	float zw; // [esp+3Ch] [ebp+8h]

	if ((((((quat->v[0] * quat->v[0]) + (quat->v[1] * quat->v[1])) + (quat->v[2] * quat->v[2])) + (quat->v[3] * quat->v[3])) - 1.0) >= 0.0020000001) {
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(Vec4IsNormalized( quat ))",
			"%s\n\t%s",
			"Vec4IsNormalized( quat )",
			va("%g %g %g %g", quat->v[0], quat->v[1], quat->v[2], quat->v[3])))
			__debugbreak();
	}
	v4 = quat->v[1];
	v5 = quat->v[0] * 2.0;
	xz = quat->v[2] * v5;
	v6 = quat->v[3];
	xw = quat->v[3] * v5;
	xx = quat->v[0] * v5;
	v7 = v4 * 2.0;
	v8 = v4 * v5;
	yy = v4 * (v4 * 2.0);
	v9 = v6 * (v4 * 2.0);
	v10 = quat->v[2];
	zw = v6 * (v10 * 2.0);
	v11 = v10 * (v10 * 2.0);
	axis->x = v2 - (v11 + yy);
	axis->z = xz - v9;
	axis[1].y = v2 - (v11 + xx);
	axis->y = zw + v8;
	axis[1].x = v8 - zw;
	axis[1].z = (v10 * v7) + xw;
	axis[2].x = v9 + xz;
	axis[2].y = (v10 * v7) - xw;
	axis[2].z = v2 - (yy + xx);
}

void UnitQuatToForward(const vec4_t* quat, vec3_t* forward)
{
	if (
		((((quat->v[0] * quat->v[0]) + (quat->v[1] * quat->v[1])) + (quat->v[2] * quat->v[2])) + (quat->v[3] * quat->v[3])) - 1.0 >= 0.0020000001)
	{
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(Vec4IsNormalized( quat ))",
			"%s\n\t%s",
			"Vec4IsNormalized( quat )",
			va("%g %g %g %g", quat->v[0], quat->v[1], quat->v[2], quat->v[3])))
			__debugbreak();
	}

	forward->x = 1.0 - (((quat->v[1] * quat->v[1]) + (quat->v[2] * quat->v[2])) * 2.0);
	forward->y = ((quat->v[0] * quat->v[1]) + (quat->v[3] * quat->v[2])) * 2.0;
	forward->z = ((quat->v[0] * quat->v[2]) - (quat->v[3] * quat->v[1])) * 2.0;
}

void QuatSlerp(vec4_t const&, vec4_t const&, float, vec4_t&)
{
}

float RotationToYaw(const vec2_t* rot)
{
	float zz; // [esp+4h] [ebp-4h]
	float r; // [esp+10h] [ebp+8h]

	zz = rot->v[0] * rot->v[0];
	r = (rot->v[1] * rot->v[1]) + zz;
	if (r == 0.0 && !Assert_MyHandler(__FILE__, __LINE__, 0, "(r)", false))
		__debugbreak();
	return atan2(rot->v[1] * rot->v[0] * (2.0 / r), 1.0 - 2.0 / r * zz) * 57.295776;
}

void FinitePerspectiveMatrix(float tanHalfFovX, float tanHalfFovY, float zNear, float zFar, vec4_t* mtx)
{
	if (!mtx && !Assert_MyHandler(__FILE__, __LINE__, 0, "(mtx)", false))
		__debugbreak();
	if (zNear <= 0.0)
	{
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(zNear) > (0.0f)",
			"zNear > 0.0f\n\t%g, %g",
			zNear,
			0.0))
			__debugbreak();
	}
	if (zFar <= zNear
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(zFar) > (zNear)",
			"zFar > zNear\n\t%g, %g",
			zFar,
			zNear))
	{
		__debugbreak();
	}
	memset(mtx, 0, 0x40u);
	mtx->v[0] = 1.0 / tanHalfFovX;
	mtx[1].v[1] = 1.0 / tanHalfFovY;
	mtx[2].v[2] = -(zNear / (zFar - zNear));
	mtx[2].v[3] = 1.0;
	mtx[3].v[2] = (zNear * zFar) / (zFar - zNear);
}

void SpotLightViewMatrix(vec3_t const&, float, vec4_t* const)
{
}

void SpotLightViewMatrixDir3(const vec3_t* dirx, const vec3_t* diry, const vec3_t* dirz, vec4_t* mtx)
{
	float v15;
	float v20;

	vec3_t axis[3];
	vec4_t lookAtMatrix[4];

	v20 = diry->x;
	lookAtMatrix[0].g = __PAIR64__(LODWORD(dirz->y), LODWORD(diry->y));
	lookAtMatrix[2].b = 0;
	lookAtMatrix[0].v[0] = 0.0;
	lookAtMatrix[1].v[0] = 0.0;
	*lookAtMatrix[2].v = 0;
	lookAtMatrix[1].g = __PAIR64__(LODWORD(dirz->z), LODWORD(diry->z));
	lookAtMatrix[0].v[3] = dirx->y;
	lookAtMatrix[1].v[3] = dirx->z;
	lookAtMatrix[3].v[0] = 1.0;
	memset(&v15, 0, 0x40u);
	v15 = 3.141592741012573;
	axis[0].z = 3.141592741012573;
	axis[0].y = 3.141592741012573;
	axis[2].y = 1.0;
	MatrixMultiply44((const vec4_t * )&v20, (const vec4_t *)&v15, mtx);
}

void SpotLightProjectionMatrix(float, float, float, vec4_t* const)
{
}

void InfinitePerspectiveMatrix(float, float, float, vec4_t* const)
{
}

void MatrixForViewer(vec3_t const&, vec3_t const* const, vec4_t* const)
{
}

float AngleSubtract(float a1, float a2)
{
	float a;

	a = a1 - a2;
	while (a > 180) {
		a -= 360;
	}
	while (a < -180) {
		a += 360;
	}
	return a;
}

void AnglesSubtract(const vec3_t* v1, const vec3_t* v2, vec3_t* out)
{
	out->x = AngleSubtract(v1->x, v2->x);
	out->y = AngleSubtract(v1->y, v2->y);
	out->z = AngleSubtract(v1->z, v2->z);
}

void AnglesSubtract(const vec2_t* v1, const vec2_t* v2, vec2_t* out)
{
	out->x = AngleSubtract(v1->x, v2->x);
	out->y = AngleSubtract(v1->y, v2->y);
}

float AngleNormalize360(float angle)
{
	return (360.0 / 65536) * ((int)(angle * (65536 / 360.0)) & 65535);
}

float RadiusFromBounds2DSq(const vec2_t* mins, const vec2_t* maxs)
{
	float v2; // xmm0_4
	float v4; // [esp+0h] [ebp-10h]

	v2 = mins->v[0];
	if (v2 <= maxs->v[0])
		v2 = maxs->v[0];
	if (mins->v[1] <= maxs->v[1])
		v4 = maxs->v[1];
	else
		v4 = mins->v[1];
	return v4 * v4 + v2 * v2;
}

void ExtendBounds(vec3_t* mins, vec3_t* maxs, const vec3_t* offset)
{
	float v3; // xmm0_4
	float v4; // xmm0_4
	float v5; // xmm0_4

	v3 = offset->x;
	if (offset->x <= 0.0)
		mins->x = mins->x + v3;
	else
		maxs->x = maxs->x + v3;
	v4 = offset->y;
	if (v4 <= 0.0)
		mins->y = mins->y + v4;
	else
		maxs->y = maxs->y + v4;
	v5 = offset->z;
	if (v5 <= 0.0)
		mins->z = mins->z + v5;
	else
		maxs->z = maxs->z + v5;
}

void ExpandBoundsToWidth(vec3_t* mins, vec3_t* maxs)
{
	float v2; // xmm0_4
	float v3; // xmm2_4
	float v4; // xmm0_4

	if (maxs->x < mins->x
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(maxs[0] >= mins[0])",
			false))
	{
		__debugbreak();
	}
	if (maxs->y < mins->y
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(maxs[1] >= mins[1])",
			false))
	{
		__debugbreak();
	}
	if (maxs->z < mins->z
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(maxs[2] >= mins[2])",
			false))
	{
		__debugbreak();
	}
	v2 = maxs->x - mins->x;
	v3 = maxs->z - mins->z;
	if ((v2 - (maxs->y - mins->y)) < 0.0)
		v2 = maxs->y - mins->y;
	if (v2 > v3)
	{
		v4 = (v2 - v3) * 0.5;
		mins->z = mins->z - v4;
		maxs->z = maxs->z + v4;
	}
}

void ClearBounds(vec3_t* mins, vec3_t* maxs)
{
	mins->x = 131072.0;
	mins->y = 131072.0;
	mins->z = 131072.0;
	maxs->x = -131072.0;
	maxs->y = -131072.0;
	maxs->z = -131072.0;
}

void AddPointToBounds(const vec3_t* v, vec3_t* mins, vec3_t* maxs)
{
	float v3; // xmm0_4
	float v4; // xmm0_4
	float v5; // xmm0_4
	float v6; // xmm0_4

	if (mins->x > v->x)
		mins->x = v->x;
	if (v->x > maxs->x)
		maxs->x = v->x;
	v3 = v->y;
	if (mins->y > v3)
		mins->y = v3;
	v4 = v->y;
	if (v4 > maxs->y)
		maxs->y = v4;
	v5 = v->z;
	if (mins->z > v5)
		mins->z = v5;
	v6 = v->z;
	if (v6 > maxs->z)
		maxs->z = v6;
}

void AddPointToBounds2D(const vec2_t* v, vec2_t* mins, vec2_t* maxs)
{
	float v3; // xmm0_4
	float v4; // xmm0_4

	if (mins->v[0] > v->v[0])
		mins->v[0] = v->v[0];
	if (v->v[0] > maxs->v[0])
		maxs->v[0] = v->v[0];
	v3 = v->v[1];
	if (mins->v[1] > v3)
		mins->v[1] = v3;
	v4 = v->v[1];
	if (v4 > maxs->v[1])
		maxs->v[1] = v4;
}

int BoundsOverlap(const vec3_t* mins0, const vec3_t* maxs0, const vec3_t* mins1, const vec3_t* maxs1)
{
	return mins0->x <= maxs1->x
		&& mins1->x <= maxs0->x
		&& mins0->y <= maxs1->y
		&& mins1->y <= maxs0->y
		&& mins0->z <= maxs1->z
		&& mins1->z <= maxs0->z;
}

void ExpandBounds(const vec3_t* addedmins, const vec3_t* addedmaxs, vec3_t* mins, vec3_t* maxs)
{
	float v4; // xmm0_4
	float v5; // xmm0_4
	float v6; // xmm0_4
	float v7; // xmm0_4

	if (mins->x > addedmins->x)
		mins->x = addedmins->x;
	if (addedmaxs->x > maxs->x)
		maxs->x = addedmaxs->x;
	v4 = addedmins->y;
	if (mins->y > v4)
		mins->y = v4;
	v5 = addedmaxs->y;
	if (v5 > maxs->y)
		maxs->y = v5;
	v6 = addedmins->z;
	if (mins->z > v6)
		mins->z = v6;
	v7 = addedmaxs->z;
	if (v7 > maxs->z)
		maxs->z = v7;
}

void AxisClear(vec3_t* axis)
{
	axis->x = 1.0;
	axis->v[2] = 0.0;
	axis[1].v[1] = 1.0;
	axis[2].x = 0.0;
	axis[2].z = 1.0;
}

void AxisCopy(const vec3_t* in, vec3_t* out)
{
	*out = *in;
	out[1] = in[1];
	out[2] = in[2];
}

void AxisTranspose(const vec3_t* in, vec3_t* out)
{
	if (in == out
		&& !Assert_MyHandler(__FILE__, __LINE__, 0, "(in != out)", nullptr))
	{
		__debugbreak();
	}
	out->x = in->x;
	out->y = in[1].x;
	out->z = in[2].x;
	out[1].x = in->y;
	out[1].y = in[1].y;
	out[1].z = in[2].y;
	out[2].x = in->z;
	out[2].y = in[1].z;
	out[2].z = in[2].z;
}

void AxisTransformVec3(const vec3_t* axes, const vec3_t* vec, vec3_t* out)
{
	out->x = ((axes[1].x * vec->y) + (axes->x * vec->x)) + (axes[2].x * vec->z);
	out->y = ((axes[1].y * vec->y) + (axes->y * vec->x)) + (axes[2].y * vec->z);
	out->z = ((axes[1].z * vec->y) + (axes->z * vec->x)) + (axes[2].z * vec->z);
}

void YawToAxis(float yaw, vec3_t* axis)
{
	float v2;

	v2 = (yaw * 0.017453292);
	if (axis) {
		axis->x = cosf(v2);
		axis->v[1] = sinf(v2);
	}
	axis[2].x = 0;
	axis[2].z = 1.0;
	axis[1].x = 0.0 - sinf(v2);
	axis[1].y = 0.0 - -cosf(v2);
	axis[1].z = vec3_origin.z;
}

void AxisToAngles(const vec3_t* axis, vec3_t* angles)
{
	vec3_t* v2; // edi
	const vec3_t* v3; // esi
	int v4; // xmm4_4
	float v5; // xmm1_4
	float v6; // xmm3_4
	float v7; // xmm2_4
	float v8; // xmm7_4
	float v9; // xmm0_4
	float v10; // xmm1_4
	float v11; // xmm3_4
	float v12; // xmm0_4
	float v13; // xmm3_4
	float v14; // xmm2_4
	float v15; // xmm3_4
	float v16; // xmm2_4
	double v17; // st7
	float _X; // [esp+4h] [ebp-30h]
	float fCos; // [esp+14h] [ebp-20h]
	float fSin; // [esp+18h] [ebp-1Ch]
	float rad; // [esp+1Ch] [ebp-18h]
	float v22; // [esp+20h] [ebp-14h]
	float v23; // [esp+24h] [ebp-10h]
	vec3_t right; // [esp+28h] [ebp-Ch]

	v2 = angles;
	v3 = axis;
	vectoangles(axis, angles);
	v4 = _mask__NegFloat_;
	v5 = v3[1].x;
	v6 = v3[1].y;
	v7 = v3[1].z;
	rad = -v2->y * 0.017453292;
	v23 = fCos;
	v22 = fSin;
	fCos = cos(rad);
	fSin = sin(rad);
	v8 = v6;
	v9 = v5;
	v10 = (v5 * fSin) + (v6 * fCos);
	v11 = -v2->x * 0.017453292;
	v12 = (v9 * fCos) - (v8 * fSin);
	right.y = v10;
	rad = v11;
	v22 = fCos;
	v23 = fSin;
	fCos = cos(v11);
	fSin = sin(v11);
	v13 = v7;
	v14 = (v7 * fCos) - (v12 * fSin);
	v15 = (v13 * fSin) + (v12 * fCos);
	right.z = v14;
	if (v10 != 0.0 || v15 != 0.0)
	{
		_X = sqrtf((v15 * v15) + (v10 * v10));
		v17 = atan2f(right.z, _X);
		v10 = right.y;
		v23 = v17 * -180.0 * 0.31830987;
		v16 = v23;
	}
	else if ((-v14) < 0.0)
	{
		v16 = -90.0;
	}
	else
	{
		v16 = 90.0;
	}
	if (v10 >= 0.0)
	{
		v2->z = -v16;
	}
	else if (v16 >= 0.0)
	{
		v2->z = v16 + -180.0;
	}
	else
	{
		v2->z = v16 + 180.0;
	}
}

void Axis4ToAngles(const vec4_t* axis, vec3_t* angles)
{
	vec3_t* v2; // edi
	const vec4_t* v3; // esi
	int v4; // xmm4_4
	float v5; // xmm1_4
	float v6; // xmm3_4
	float v7; // xmm2_4
	float v8; // xmm7_4
	float v9; // xmm0_4
	float v10; // xmm1_4
	float v11; // xmm3_4
	float v12; // xmm0_4
	float v13; // xmm3_4
	float v14; // xmm2_4
	float v15; // xmm3_4
	float v16; // xmm2_4
	double v17; // st7
	float _X; // [esp+4h] [ebp-30h]
	float fCos; // [esp+10h] [ebp-24h]
	float fSin; // [esp+14h] [ebp-20h]
	float rad; // [esp+18h] [ebp-1Ch]
	float v22; // [esp+1Ch] [ebp-18h]
	float v23; // [esp+20h] [ebp-14h]
	vec4_t right; // [esp+24h] [ebp-10h]

	v2 = angles;
	v3 = axis;
	vectoangles((const vec3_t *)axis, angles);
	v5 = v3[1].v[0];
	v6 = v3[1].v[1];
	v7 = v3[1].v[2];
	rad = -v2->y * 0.017453292;
	v23 = fCos;
	v22 = fSin;
	fCos = cos(rad);
	fSin = sin(rad);
	v8 = v6;
	v9 = v5;
	v10 = (v5 * fSin) + (v6 * fCos);
	v11 = -v2->x * 0.017453292;
	v12 = (v9 * fCos) - (v8 * fSin);
	right.v[1] = v10;
	rad = v11;
	v22 = fCos;
	v23 = fSin;
	fCos = cos(v11);
	fSin = sin(v11);
	v13 = v7;
	v14 = (v7 * fCos) - (v12 * fSin);
	v15 = (v13 * fSin) + (v12 * fCos);
	right.v[2] = v14;
	if (v10 != 0.0 || v15 != 0.0)
	{
		_X = sqrtf((v15 * v15) + (v10 * v10));
		v17 = atan2f(right.v[2], _X);
		v10 = right.v[1];
		v23 = v17 * -180.0 * 0.31830987;
		v16 = v23;
	}
	else if ((-v14) < 0.0)
	{
		v16 = -90.0;
	}
	else
	{
		v16 = 90.0;
	}
	if (v10 >= 0.0)
	{
		v2->z = -v16;
	}
	else if (v16 >= 0.0)
	{
		v2->z = v16 + -180.0;
	}
	else
	{
		v2->z = v16 + 180.0;
	}
}

int IntersectPlanes(const float** planein, vec3_t* xyz)
{
	double v2; // xmm6_8
	const float* v3; // eax
	double v4; // xmm3_8
	double v5; // xmm1_8
	double v6; // xmm5_8
	float v7; // xmm0_4
	const float* v8; // eax
	double v9; // xmm2_8
	double v10; // xmm4_8
	double v11; // xmm0_8
	float v13; // xmm4_4
	float v14; // xmm2_4
	double plane; // [esp+0h] [ebp-88h]
	double plane_8; // [esp+8h] [ebp-80h]
	double plane_16; // [esp+10h] [ebp-78h]
	double plane_24; // [esp+18h] [ebp-70h]
	double plane_56; // [esp+38h] [ebp-50h]
	double plane_72; // [esp+48h] [ebp-40h]
	double plane_88; // [esp+58h] [ebp-30h]
	double determinant; // [esp+78h] [ebp-10h]
	double v23; // [esp+80h] [ebp-8h]

	plane = **planein;
	v2 = (*planein)[1];
	plane_16 = (*planein)[2];
	v3 = planein[1];
	plane_24 = (*planein)[3];
	v4 = *v3;
	v5 = v3[1];
	v6 = v3[2];
	v7 = v3[3];
	v8 = planein[2];
	plane_72 = v8[1];
	plane_88 = v8[3];
	v9 = v8[2];
	v10 = v9 * v5 - plane_72 * v6;
	plane_8 = (*planein)[1];
	plane_56 = v7;
	v23 = v6 * v2 - v5 * plane_16;
	v11 = *v8;
	determinant = (plane_72 * plane_16 - v9 * v2) * v4 + v10 * plane + v23 * v11;
	if (determinant < 0.001000000047497451)
		return 0;
	v13 = ((plane_72 * plane_16 - v9 * v2) * plane_56 + v10 * plane_24 + v23 * plane_88) * (1.0 / determinant);
	xyz->x = v13;
	v14 = ((v9 * plane - v11 * plane_16) * plane_56
		+ (v11 * v6 - v9 * v4) * plane_24
		+ (v4 * plane_16 - v6 * plane) * plane_88)
		* (1.0
			/ determinant);
	xyz->y = v14;
	xyz->z = ((v11 * plane_8 - plane_72 * plane) * plane_56
		+ (plane_72 * v4 - v11 * v5) * plane_24
		+ (v5 * plane - v4 * plane_8) * plane_88)
		* (1.0
			/ determinant);
	return 1;
}

int ProjectedWindingContainsCoplanarPoint(const vec3_t* verts, int vertCount, int x, int y, const vec3_t* point)
{
	int v5; // eax
	float v6; // ebx
	float i; // edi
	int v9; // [esp+Ch] [ebp-8h]
	int vertIndex; // [esp+10h] [ebp-4h]

	vertIndex = 0;
	if (vertCount <= 0)
		return 1;
	v9 = 0;
	v5 = 4 * (3 * vertCount - 3);
	v6 = verts->x + x;
	for (i = verts->x + y;
		(((*(&point->x + y) - *(&verts->x + 4 * y + v5)) * (*(&verts->x + 4 * x + v5) - v6))
			+ ((*(&point->x + x) - *(&verts->x + 4 * x + v5)) * (i - *(&verts->x + 4 * y + v5)))) >= 0.0;
		i += 3)
	{
		v5 = v9;
		++vertIndex;
		v9 += 12;
		v6 += 3;
		if (vertIndex >= vertCount)
			return 1;
	}
	return 0;
}

int PlaneFromPoints(vec4_t* plane, const vec3_t* v0, const vec3_t* v1, const vec3_t* v2)
{
	float v4; // xmm0_4
	unsigned int v5; // xmm1_4
	unsigned int v6; // xmm2_4
	float v7; // xmm3_4
	float v8; // xmm0_4
	float v9; // xmm0_4
	float v11; // xmm1_4
	vec3_t v1_v0; // [esp+8h] [ebp-1Ch]
	vec3_t v2_v0; // [esp+14h] [ebp-10h]

	v4 = v2->y - v0->y;
	*&v5 = v1->x - v0->x;
	*&v6 = v1->y - v0->y;
	v1_v0.z = v1->z - v0->z;
	v7 = v2->x - v0->x;
	v2_v0.y = v4;
	v8 = v2->z - v0->z;
	*&v1_v0.x = __PAIR64__(v6, v5);
	v2_v0.x = v7;
	v2_v0.z = v8;
	Vec3Cross(&v2_v0, &v1_v0, (vec3_t*)plane);
	v9 = ((plane->v[0] * plane->v[0]) + (plane->v[1] * plane->v[1])) + (plane->v[2] * plane->v[2]);
	if (v9 < 2.0)
	{
		if (v9 == 0.0)
			return 0;
		if ((((((v2_v0.y * v2_v0.y) + (v2_v0.x * v2_v0.x)) + (v2_v0.z * v2_v0.z))
			* (((v1_v0.y * v1_v0.y) + (v1_v0.x * v1_v0.x)) + (v1_v0.z * v1_v0.z)))
			* 0.0000010000001) >= v9)
		{
			v1_v0.x = v2->x - v1->x;
			v1_v0.y = v2->y - v1->y;
			v1_v0.z = v2->z - v1->z;
			v2_v0.x = v0->x - v1->x;
			v2_v0.y = v0->y - v1->y;
			v2_v0.z = v0->z - v1->z;
			Vec3Cross(&v2_v0, &v1_v0, (vec3_t*)plane);
			if ((((((v2_v0.y * v2_v0.y) + (v2_v0.x * v2_v0.x)) + (v2_v0.z * v2_v0.z))
				* (((v1_v0.y * v1_v0.y) + (v1_v0.x * v1_v0.x)) + (v1_v0.z * v1_v0.z)))
				* 0.0000010000001) >= v9)
				return 0;
		}
	}
	v11 = sqrtf(v9);
	plane->v[0] = (1.0 / v11) * plane->v[0];
	plane->v[1] = plane->v[1] * (1.0 / v11);
	plane->v[2] = plane->v[2] * (1.0 / v11);
	plane->v[3] = ((plane->v[1] * v0->y) + (v0->x * plane->v[0])) + (plane->v[2] * v0->z);
	return 1;
}

void ProjectPointOnPlane(const vec3_t* p, const vec3_t* normal, vec3_t* dst)
{
	const char* v3; // eax
	float v4; // xmm0_4

	if (sqrtf(((normal->x * normal->x) + (normal->y * normal->y)) + (normal->z * normal->z)) - 1.0 >= 0.0020000001)
	{
		v3 = va(
			"(%g %g %g) len %g",
			normal->x,
			normal->y,
			normal->z,
			sqrtf(((normal->x * normal->x) + (normal->y * normal->y)) + (normal->z * normal->z)));
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(Vec3IsNormalized( normal ))",
			"%s\n\t%s",
			"Vec3IsNormalized( normal )",
			v3))
			__debugbreak();
	}
	v4 = -(((p->y * normal->y) + (normal->x * p->x)) + (p->z * normal->z));
	dst->x = (normal->x * v4) + p->x;
	dst->y = (v4 * normal->y) + p->y;
	dst->z = (v4 * normal->z) + p->z;
}

void SetPlaneSignbits(cplane_s* out)
{
	char v1; // al

	v1 = out->normal.x < 0.0;
	if (out->normal.y < 0.0)
		v1 |= 2u;
	if (out->normal.z < 0.0)
		v1 |= 4u;
	out->signbits = v1;
}

int BoxOnPlaneSide(const vec3_t* emins, const vec3_t* emaxs, const cplane_s* p)
{
	float	dist1, dist2;
	int		sides;

	if (!bops_initialized) {
		bops_initialized = true;
		switch (p->signbits)
		{
		case 0:
			dist1 = p->normal.x * emaxs->x + p->normal.y * emaxs->y + p->normal.z * emaxs->y;
			dist2 = p->normal.x * emins->x + p->normal.y * emins->y + p->normal.z * emins->y;
			break;
		case 1:
			dist1 = p->normal.x * emins->x + p->normal.y * emaxs->y + p->normal.z * emaxs->y;
			dist2 = p->normal.x * emaxs->x + p->normal.y * emins->y + p->normal.z * emins->y;
			break;
		case 2:
			dist1 = p->normal.x * emaxs->x + p->normal.y * emins->y + p->normal.z * emaxs->y;
			dist2 = p->normal.x * emins->x + p->normal.y * emaxs->y + p->normal.z * emins->y;
			break;
		case 3:
			dist1 = p->normal.x * emins->x + p->normal.y * emins->y + p->normal.z * emaxs->y;
			dist2 = p->normal.x * emaxs->x + p->normal.y * emaxs->y + p->normal.z * emins->y;
			break;
		case 4:
			dist1 = p->normal.x * emaxs->x + p->normal.y * emaxs->y + p->normal.z * emins->y;
			dist2 = p->normal.x * emins->x + p->normal.y * emins->y + p->normal.z * emaxs->y;
			break;
		case 5:
			dist1 = p->normal.x * emins->x + p->normal.y * emaxs->y + p->normal.z * emins->y;
			dist2 = p->normal.x * emaxs->x + p->normal.y * emins->y + p->normal.z * emaxs->y;
			break;
		case 6:
			dist1 = p->normal.x * emaxs->x + p->normal.y * emins->y + p->normal.z * emins->y;
			dist2 = p->normal.x * emins->x + p->normal.y * emaxs->y + p->normal.z * emaxs->y;
			break;
		case 7:
			dist1 = p->normal.x * emins->x + p->normal.y * emins->y + p->normal.z * emins->y;
			dist2 = p->normal.x * emaxs->x + p->normal.y * emaxs->y + p->normal.z * emaxs->y;
			break;
		default:
			dist1 = dist2 = 0;
			break;
		}

	}

	sides = 0;
	if (dist1 >= p->dist)
		sides = 1;
	if (dist2 < p->dist)
		sides |= 2;

	return sides;
}

int IsPosInsideArc(const vec3_t* pos, const vec3_t* arcOrigin, float posRadius, float arcRadius, float arcAngle0, float arcAngle1)
{
	float v6; // xmm0_4
	float v7; // xmm2_4
	float v8; // xmm3_4
	long double v9; // st7
	float v10; // xmm2_4
	float v11; // xmm0_4
	float v13; // [esp+8h] [ebp-18h]
	float v14; // [esp+10h] [ebp-10h]
	vec3_t originDelta; // [esp+14h] [ebp-Ch]

	v6 = pos->x - arcOrigin->x;
	v7 = pos->z;
	v8 = arcOrigin->z;
	originDelta.y = pos->y - arcOrigin->y;
	originDelta.x = v6;
	v14 = v7;
	originDelta.z = v7 - v8;
	v9 = sqrtf((originDelta.y * originDelta.y) + (v6 * v6));
	if (-v9 < 0.0)
		v10 = v9;
	else
		v10 = DCONSTcg_hudSplitscreenCompassScale;
	originDelta.x = (1.0 / v10) * originDelta.x;
	originDelta.y = originDelta.y * (1.0 / v10);
	v13 = v9;
	if (((v13 - 15.0) * (v13 - 15.0)) <= (posRadius * posRadius) && (v8 - arcAngle1) <= v14 && v14 <= (v8 + arcAngle1))
	{
		v11 = vectoyaw((const vec2_t *)&originDelta);
		v11 = AngleNormalize360(v11);
		if (arcAngle0 <= arcRadius)
		{
			if (arcAngle0 > v11 || v11 > arcRadius)
				return 1;
		}
		else if (arcAngle0 > v11 && v11 > arcRadius)
		{
			return 1;
		}
	}
	return 0;
}

float Q_rint(float in)
{
	return floorf(in + 0.5f);
}

float ColorNormalize(const vec3_t* in, vec3_t* out)
{
	const vec3_t* v2; // ecx
	float v3; // xmm2_4
	float v4; // xmm0_4
	float v5; // xmm1_4
	double result; // st7
	float max; // [esp+4h] [ebp+4h]

	v2 = in;
	v3 = in->x;
	v4 = in->y;
	max = in->x;
	v5 = max;
	if (v4 > max)
	{
		v5 = v4;
		max = v4;
	}
	if (v2->z > v5)
	{
		v5 = v2->z;
		max = v2->z;
	}
	if (v5 == 0.0)
	{
		result = 0.0;
		out->z = 1.0;
		out->y = 1.0;
		out->x = 1.0;
	}
	else
	{
		result = max;
		out->x = v3 * (1.0 / v5);
		out->y = v2->y * (1.0 / v5);
		out->z = v2->z * (1.0 / v5);
	}
	return result;
}

void ColorSRGBtoLinear(vec3_t const&, vec3_t&)
{
}

float PitchForYawOnNormal(const float fYaw, const vec3_t* normal)
{
	long double v2; // st7
	long double result; // st7
	float v4; // [esp+0h] [ebp-18h]
	float v5; // [esp+4h] [ebp-14h]
	float v6; // [esp+8h] [ebp-10h]
	float* v7; // [esp+Ch] [ebp-Ch]
	float* v8; // [esp+10h] [ebp-8h]

	if (normal->x == 0.0
		&& normal->y == 0.0
		&& normal->z == 0.0
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(normal[0] || normal[1] || normal[2])",
			nullptr))
	{
		__debugbreak();
	}
	v5 = fYaw * 0.017453292;
	v8 = &v4;
	v7 = &v6;
	v2 = (float)(fYaw * 0.017453292);
	v4 = cosf(v2);
	v6 = sinf(v2);
	v5 = normal->z;
	if (v5 == 0.0)
		result = 270.0;
	else
		result = atan2f((normal->y * v6 + normal->x * v4) / v5, 1.0) * 180.0 * 0.31830987;
	return result;
}

void NearestPitchAndYawOnPlane(const vec3_t* angles, const vec3_t* normal, vec3_t* result)
{
	vec3_t forward; // [esp+0h] [ebp-1Ch]
	vec3_t projected; // [esp+Ch] [ebp-10h]

	if (normal->x == 0.0
		&& normal->y == 0.0
		&& normal->z == 0.0
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(normal[0] || normal[1] || normal[2])",
			nullptr))
	{
		__debugbreak();
	}
	AngleVectors(angles, &forward, 0, 0);
	ProjectPointOnPlane(&forward, normal, &projected);
	vectoangles(&projected, result);
}

void Rand_Init(int seed)
{
	holdrand_0 = seed;
}

unsigned int* GetRandSeed()
{
	return &holdrand_0;
}

float flrand(float min, float max)
{
	holdrand_0 = 214013 * holdrand_0 + 2531011;
	return min + (max - min) * (double)(holdrand_0 >> 17) * 0.000030517578;
}

int irand(int min, int max)
{
	holdrand_0 = 214013 * holdrand_0 + 2531011;
	return min + (((max - min) * (unsigned __int64)(holdrand_0 >> 17)) >> 15);
}

void AxisToQuat(const vec3_t* mat, vec4_t* out)
{
	float v2; // xmm5_4
	float v3; // xmm0_4
	float v4; // xmm4_4
	float v5; // xmm3_4
	float v6; // xmm2_4
	float v7; // xmm5_4
	float v8; // xmm0_4
	float v9; // xmm0_4
	float v10; // xmm1_4
	float v11; // xmm7_4
	float v12; // xmm4_4
	float v13; // xmm1_4
	float v14; // xmm3_4
	float v15; // xmm6_4
	float v16; // xmm3_4
	float v17; // xmm0_4
	int v18; // esi
	unsigned int v19; // xmm7_4
	float v20; // xmm4_4
	unsigned int v21; // xmm3_4
	float v22; // xmm7_4
	float v23; // xmm3_4
	float v24; // xmm0_4
	float v25; // xmm3_4
	float v26; // xmm2_4
	float v27; // xmm1_4
	vec4_t* v28; // eax
	float testSizeSq; // [esp+10h] [ebp-70h]
	float v30; // [esp+14h] [ebp-6Ch]
	float v31; // [esp+18h] [ebp-68h]
	float v32; // [esp+1Ch] [ebp-64h]
	float v33; // [esp+20h] [ebp-60h]
	float v34; // [esp+24h] [ebp-5Ch]
	float v35; // [esp+28h] [ebp-58h]
	float v36; // [esp+30h] [ebp-50h]
	float v37; // [esp+34h] [ebp-4Ch]
	float v38; // [esp+38h] [ebp-48h]
	vec4_t test[4]; // [esp+3Ch] [ebp-44h]

	v2 = mat[1].z;
	v3 = mat[2].y;
	v4 = mat->y;
	v5 = mat->x;
	v6 = mat[1].y;
	v33 = v3;
	v31 = v2;
	v7 = v2 - v3;
	v8 = mat[2].x;
	v38 = v8;
	v9 = v8 - mat->z;
	v35 = mat->z;
	v10 = mat[1].x;
	v11 = v9 * v9;
	test[0].v[1] = v9;
	v32 = v5;
	v34 = v4;
	v12 = v4 - v10;
	v36 = v10;
	v30 = mat[2].z;
	v13 = 1.0;
	v14 = ((v5 + v6) + mat[2].z) + 1.0;
	test[0].v[3] = v14;
	v15 = v12 * v12;
	v16 = v14 * v14;
	v17 = (((v7 * v7) + (v9 * v9)) + (v12 * v12)) + v16;
	test[0].v[0] = v7;
	test[0].v[2] = v12;
	v37 = v11;
	testSizeSq = (((v7 * v7) + v11) + (v12 * v12)) + v16;
	if (v17 < 1.0)
	{
		v19 = v33 + v31;
		test[1].v[0] = v35 + v38;
		test[1].v[3] = v12;
		v20 = (v35 + v38) * (v35 + v38);
		v21 = ((v30 - v6) - v32) + 1.0;
		test[1].g = __PAIR64__(v21, v19);
		v22 = *&v19 * *&v19;
		v23 = *&v21 * *&v21;
		v17 = ((v20 + v22) + v23) + v15;
		testSizeSq = ((v20 + v22) + v23) + v15;
		if (v17 < 1.0)
		{
			v24 = ((v32 - v6) - v30) + 1.0;
			test[2].v[0] = v24;
			test[2].b = __PAIR64__(LODWORD(v7), LODWORD(test[1].v[0]));
			v25 = (v36 + v34) * (v36 + v34);
			v17 = (((v24 * v24) + v25) + v20) + (v7 * v7);
			test[2].v[1] = v36 + v34;
			testSizeSq = v17;
			if (v17 < 1.0)
			{
				test[3].v[3] = test[0].v[1];
				v26 = ((v6 - v32) - v30) + 1.0;
				v17 = (((v26 * v26) + v25) + v22) + v37;
				test[3].v[0] = v36 + v34;
				test[3].g = __PAIR64__(LODWORD(test[1].v[1]), LODWORD(v26));
				testSizeSq = (((v26 * v26) + v25) + v22) + v37;
				if (v17 < 1.0)
				{
					if (!Assert_MyHandler(
						__FILE__,
						__LINE__,
						0,
						"((testSizeSq >= 1.0f))",
						"(testSizeSq) = %g",
						v26))
						__debugbreak();
					v13 = 1.0;
					v17 = (((v26 * v26) + v25) + v22) + v37;
				}
				v18 = 3;
			}
			else
			{
				v18 = 2;
			}
		}
		else
		{
			v18 = 1;
		}
	}
	else
	{
		v18 = 0;
	}
	if (v17 == 0.0)
	{
		if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(testSizeSq)", nullptr))
			__debugbreak();
		v13 = 1.0;
		v17 = testSizeSq;
	}
	v27 = v13 / sqrtf(v17);
	v28 = &test[v18];
	out->v[0] = v28->v[0] * v27;
	out->v[1] = v28->v[1] * v27;
	out->v[2] = v28->v[2] * v27;
	out->v[3] = v28->v[3] * v27;
}

void QuatLerp(const vec4_t* qa, const vec4_t* qb, float frac, vec4_t* out)
{
	float v4; // xmm0_4
	float v5; // xmm2_4
	float v6; // xmm2_4

	v4 = qb->v[0];
	if (((((qb->v[1] * qa->v[1]) + (qa->v[0] * qb->v[0])) + (qa->v[2] * qb->v[2])) + (qa->v[3] * qb->v[3])) < 0.0)
	{
		out->v[0] = -v4;
		v6 = out->v[0];
		out->v[1] = -qb->v[1];
		out->v[2] = -qb->v[2];
		out->v[3] = -qb->v[3];
		out->v[0] = ((v6 - qa->v[0]) * frac) + qa->v[0];
		out->v[1] = ((out->v[1] - qa->v[1]) * frac) + qa->v[1];
		out->v[2] = ((out->v[2] - qa->v[2]) * frac) + qa->v[2];
		v5 = ((out->v[3] - qa->v[3]) * frac) + qa->v[3];
	}
	else
	{
		out->v[0] = ((v4 - qa->v[0]) * frac) + qa->v[0];
		out->v[1] = ((qb->v[1] - qa->v[1]) * frac) + qa->v[1];
		out->v[2] = ((qb->v[2] - qa->v[2]) * frac) + qa->v[2];
		v5 = ((qb->v[3] - qa->v[3]) * frac) + qa->v[3];
	}
	out->v[3] = v5;
}

bool CullBoxFromCone(const vec3_t* coneOrg, const vec3_t* coneDir, float cosHalfFov, const vec3_t* boxCenter, const vec3_t* boxHalfSize)
{
	float v5; // xmm0_4
	float v6; // xmm4_4
	float v7; // xmm5_4
	float v8; // xmm1_4
	float v9; // xmm5_4
	float v10; // xmm2_4
	float v11; // xmm7_4
	float v12; // xmm2_4
	float v13; // xmm6_4
	float v14; // xmm5_4
	bool result; // al
	float v16; // xmm0_4
	float v17; // xmm1_4
	float v18; // xmm2_4
	float v19; // xmm4_4
	float v20; // xmm5_4
	float v21; // xmm4_4
	float v22; // [esp+0h] [ebp-2Ch]
	float v23; // [esp+14h] [ebp-18h]
	float v24; // [esp+18h] [ebp-14h]

	if (cosHalfFov < 0.0
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(cosHalfFov >= 0.0f)",
			nullptr))
	{
		__debugbreak();
	}
	v5 = coneDir->x;
	v6 = -1.0;
	if (coneDir->x < 0.0)
		v7 = -1.0;
	else
		v7 = 1.0;
	v8 = (boxCenter->x - coneOrg->x) - (boxHalfSize->x * v7);
	v22 = coneDir->y;
	if (coneDir->y < 0.0)
		v9 = -1.0;
	else
		v9 = 1.0;
	v10 = boxHalfSize->y;
	v23 = v10;
	v11 = (boxCenter->y - coneOrg->y) - (v10 * v9);
	v12 = coneDir->z;
	if (v12 >= 0.0)
		v6 = 1.0;
	v13 = (boxCenter->z - coneOrg->z) - (boxHalfSize->z * v6);
	v14 = ((v22 * v11) + (v5 * v8)) + (v12 * v13);
	result = 1;
	if (v14 < 0.0)
	{
		v16 = (v5 * -v14) + v8;
		v17 = (v22 * -v14) + v11;
		v18 = (v12 * -v14) + v13;
		v19 = ((v17 * v17) + (v16 * v16)) + (v18 * v18);
		v24 = v14 * v14;
		v20 = 1.0 - (cosHalfFov * cosHalfFov);
		if ((v24 * v20) >= ((cosHalfFov * cosHalfFov) * v19)
			|| (v21 = cosHalfFov / sqrtf(v20 * v19),
				((((((((v16 * v21) + coneDir->x) * (boxCenter->x - coneOrg->x))
					+ (((v17 * v21) + v22) * (boxCenter->y - coneOrg->y)))
					+ (((v18 * v21) + coneDir->z) * (boxCenter->z - coneOrg->z)))
					- boxHalfSize->x * ((v16 * v21) + coneDir->x))
					- v23 * ((v17 * v21) + v22))
					- boxHalfSize->z * ((v18 * v21) + coneDir->z)) < 0.0))
		{
			result = 0;
		}
	}
	return result;
}

bool CullBoxFromSphere(const vec3_t* sphereOrg, float radius, const vec3_t* boxCenter, const vec3_t* boxHalfSize)
{
	float v4; // xmm3_4
	float v5; // xmm2_4
	float v6; // xmm0_4

	if ((sphereOrg->x - boxCenter->x - boxHalfSize->x) < 0.0)
		v4 = 0.0;
	else
		v4 = sphereOrg->x - boxCenter->x - boxHalfSize->x;
	if ((sphereOrg->y - boxCenter->y - boxHalfSize->y) < 0.0)
		v5 = 0.0;
	else
		v5 = sphereOrg->y - boxCenter->y - boxHalfSize->y;
	v6 = sphereOrg->z - boxCenter->z - boxHalfSize->z;
	if (v6 < 0.0)
		v6 = 0.0;
	return (((v4 * v4) + (v5 * v5)) + (v6 * v6)) > (radius * radius);
}

bool CullBoxFromConicSectionOfSphere(const vec3_t* coneOrg, const vec3_t* coneDir, float cosHalfFov, float radius, const vec3_t* boxCenter, const vec3_t* boxHalfSize)
{
	float v6; // xmm1_4
	float v7; // xmm2_4
	float v8; // xmm7_4
	float v9; // xmm3_4
	float v10; // xmm5_4
	bool result; // al
	float v12; // xmm0_4
	float v13; // xmm5_4
	float v14; // xmm3_4
	float v15; // xmm5_4
	float v16; // xmm1_4
	float v17; // xmm6_4
	float v18; // xmm3_4
	float v19; // xmm3_4
	float v20; // xmm6_4
	float v21; // xmm2_4
	float v22; // xmm5_4
	float v23; // xmm0_4
	float v24; // xmm1_4
	float v25; // xmm2_4
	float v26; // xmm4_4
	float v27; // xmm5_4
	float v28; // xmm4_4
	float influenceRadiusa; // [esp+Ch] [ebp-38h]
	float influenceRadius; // [esp+Ch] [ebp-38h]
	float v31; // [esp+10h] [ebp-34h]
	float sinHalfFovSq; // [esp+14h] [ebp-30h]
	float v33; // [esp+1Ch] [ebp-28h]
	float v34; // [esp+20h] [ebp-24h]
	float distFromBoxToMid; // [esp+28h] [ebp-1Ch]
	float deltaMid; // [esp+34h] [ebp-10h]
	float deltaMid_4; // [esp+38h] [ebp-Ch]
	float deltaMid_8; // [esp+3Ch] [ebp-8h]

	if (cosHalfFov < 0.0
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(cosHalfFov >= 0.0f)",
			nullptr))
	{
		__debugbreak();
	}
	v6 = boxCenter->x - coneOrg->x;
	v7 = boxCenter->y - coneOrg->y;
	deltaMid = boxCenter->x - coneOrg->x;
	deltaMid_4 = boxCenter->y - coneOrg->y;
	deltaMid_8 = boxCenter->z - coneOrg->z;
	if ((v6 - boxHalfSize->x) < 0.0)
		distFromBoxToMid = 0.0;
	else
		distFromBoxToMid = v6 - boxHalfSize->x;
	if ((v7 - boxHalfSize->y) < 0.0)
		v8 = 0.0;
	else
		v8 = v7 - boxHalfSize->y;
	v9 = boxCenter->z - coneOrg->z - boxHalfSize->z;
	if (v9 < 0.0)
		v9 = 0.0;
	v10 = radius;
	if (cosHalfFov > 0.000001)
	{
		influenceRadiusa = ceil((radius / cosHalfFov));
		v10 = influenceRadiusa;
		v6 = deltaMid;
		v7 = deltaMid_4;
	}
	if ((((v8 * v8) + (distFromBoxToMid * distFromBoxToMid)) + (v9 * v9)) > (v10 * v10))
		goto LABEL_30;
	v31 = coneDir->x;
	v12 = -1.0;
	v13 = coneDir->x < 0.0 ? -1.0 : 1.0;
	v14 = boxHalfSize->x * v13;
	v15 = coneDir->y;
	v16 = v6 - v14;
	influenceRadius = coneDir->y;
	v17 = v15 < 0.0 ? -1.0 : 1.0;
	v18 = boxHalfSize->y;
	v33 = v18;
	v19 = v18 * v17;
	v20 = coneDir->z;
	v21 = v7 - v19;
	if (v20 >= 0.0)
		v12 = 1.0;
	sinHalfFovSq = boxHalfSize->z * v12;
	v22 = ((v15 * v21) + (v31 * v16)) + (v20 * (deltaMid_8 - sinHalfFovSq));
	if (v22 >= 0.0
		|| (v23 = (v31 * -v22) + v16,
			v24 = (influenceRadius * -v22) + v21,
			v25 = (v20 * -v22) + (deltaMid_8 - sinHalfFovSq),
			v26 = ((v24 * v24) + (v23 * v23)) + (v25 * v25),
			v34 = v22 * v22,
			v27 = 1.0 - (cosHalfFov * cosHalfFov),
			(v34 * v27) < ((cosHalfFov * cosHalfFov) * v26))
		&& (v28 = cosHalfFov / sqrtf(v27 * v26),
			((((((((v23 * v28) + v31) * deltaMid) + (((v24 * v28) + influenceRadius) * deltaMid_4))
				+ (((v25 * v28) + coneDir->z) * deltaMid_8))
				- boxHalfSize->x * ((v23 * v28) + v31))
				- v33 * ((v24 * v28) + influenceRadius))
				- boxHalfSize->z * ((v25 * v28) + coneDir->z)) >= 0.0))
	{
	LABEL_30:
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}

bool CullSphereFromCone(const vec3_t* coneOrg, const vec3_t* coneDir, float cosHalfFov, const vec3_t* sphereCenter, float radius)
{
	float v5; // xmm5_4
	float v6; // xmm6_4
	float v7; // xmm7_4
	float v8; // xmm1_4
	bool result; // al
	float v10; // xmm0_4

	if (cosHalfFov < 0.0
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(cosHalfFov >= 0.0f)",
			nullptr))
	{
		__debugbreak();
	}
	v5 = sphereCenter->x - coneOrg->x;
	v6 = sphereCenter->y - coneOrg->y;
	v7 = sphereCenter->z - coneOrg->z;
	v8 = ((coneDir->y * v6) + (coneDir->x * v5)) + (coneDir->z * v7);
	result = 1;
	if (v8 < radius)
	{
		v10 = sqrtf(1.0 - (cosHalfFov * cosHalfFov));
		if (((((((coneDir->y * -v8) + v6)
			* ((coneDir->y * -v8) + v6))
			+ (((coneDir->x * -v8) + v5)
				* ((coneDir->x * -v8) + v5)))
			+ (((coneDir->z * -v8) + v7)
				* ((coneDir->z * -v8) + v7)))
			* (cosHalfFov * cosHalfFov)) < (((v10 * v8) - radius) * ((v10 * v8) - radius)))
			result = 0;
	}
	return result;
}

void colorTempToXYZ(float colorTemp, vec4_t* finalVec)
{
}

void colorTempMatrix(vec4_t* const, float)
{
}

void colorHueMatrix(vec4_t* const, float)
{
}

void colorSaturationMatrix(vec4_t* finalMatrix, float saturation)
{
	float v2; // xmm1_4
	unsigned int v3; // xmm0_4
	float v4; // xmm2_4

	v2 = (1.0 - saturation) * 0.25;
	v3 = (1.0 - saturation) * 0.5;
	v4 = (1.0 - saturation) * 0.25;
	finalMatrix->v[0] = v2 + saturation;
	finalMatrix->v[1] = v4;
	finalMatrix->v[2] = v4;
	finalMatrix[2].v[0] = v2;
	finalMatrix[2].v[1] = v2;
	finalMatrix->v[3] = 0.0;
	finalMatrix[1].v[0] = *&v3;
	finalMatrix[1].v[1] = *&v3 + saturation;
	*&finalMatrix[1].b = v3;
	finalMatrix[2].v[2] = v2 + saturation;
	*finalMatrix[3].v = 0i64;
	finalMatrix[3].v[2] = 0.0;
	finalMatrix[3].v[3] = 1.0;
}

float I_fnormPDF(float x)
{
	double v1; // st7

	v1 = x * x * -0.5 * 1.442695040888963387;
	return ((2 ^ (int)v1) - v1 + 1.0) * 0.3989422804014327;
}

float I_fnormCDF(float x)
{
	return I_normCDF(x);
}

void RotatePointAroundVector(vec3_t* dst, const vec3_t* dir, const vec3_t* point, const float degrees)
{
	float v4; // xmm1_4
	float v5; // xmm2_4
	float v6; // xmm0_4
	const char* v7; // eax
	float v8; // xmm2_4
	unsigned int v9; // xmm3_4
	int v10; // eax
	float v11; // xmm1_4
	float v12; // xmm4_4
	float v13; // xmm0_4
	float v14; // xmm2_4
	float v15; // xmm4_4
	float v16; // xmm0_4
	float v17; // xmm5_4
	float v18; // xmm1_4
	float v19; // xmm2_4
	float v20; // xmm6_4
	float v21; // xmm3_4
	float v22; // xmm4_4
	float v23; // xmm0_4
	float v24; // xmm5_4
	float v25; // xmm2_4
	float rad; // [esp+2Ch] [ebp-9Ch]
	vec3_t vup; // [esp+34h] [ebp-94h]
	vec3_t vf; // [esp+40h] [ebp-88h]
	vec3_t vr; // [esp+4Ch] [ebp-7Ch]
	vec3_t m[3]; // [esp+58h] [ebp-70h]
	vec3_t tmpmat[3]; // [esp+7Ch] [ebp-4Ch]
	vec3_t rot[3]; // [esp+A0h] [ebp-28h]

	if (dir->x == 0.0
		&& dir->y == 0.0
		&& dir->z == 0.0
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(dir[0] || dir[1] || dir[2])",
			nullptr))
	{
		__debugbreak();
	}
	v4 = dir->y;
	v5 = dir->z;
	vf.x = dir->x;
	vf.z = v5;
	v6 = 1.0;
	if (sqrtf(((v4 * v4) + (vf.x * vf.x)) + (v5 * v5)) - 1.0 >= 0.0020000001)
	{
		v7 = va(
			"(%g %g %g) len %g",
			dir->x,
			dir->y,
			dir->z,
			sqrtf(((dir->x * dir->x) + (dir->y * dir->y)) + (dir->z * dir->z)));
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(Vec3IsNormalized( src ))",
			"%s\n\t%s",
			"Vec3IsNormalized( src )",
			v7))
			__debugbreak();
		v6 = 1.0;
	}
	v8 = dir->y * dir->y;
	v9 = dir->z * dir->z;
	vup.x = dir->x * dir->x;
	vup.v[1] = __PAIR64__(v9, v8);
	v10 = vup.x > v8;
	if (*(&vup.x + v10) > * &v9)
		v10 = 2;
	v11 = -(dir->x + v10);
	vr.x = dir->x * v11;
	vr.y = dir->y * v11;
	vr.z = dir->z * v11;
	*(&vr.x + v10) = *(&vr.x + v10) + v6;
	v12 = sqrtf(((vr.y * vr.y) + (vr.x * vr.x)) + (vr.z * vr.z));
	if (-v12 >= 0.0)
		v12 = v6;
	v13 = v6 / v12;
	vr.x = vr.x * v13;
	vr.y = vr.y * v13;
	vr.z = vr.z * v13;
	Vec3Cross(&vr, &vf, &vup);
	m[0].x = vr.x;
	m[1].y = vup.y;
	m[2].z = vf.z;
	rot[0].v[2] = 0;
	rot[2].x = 0;
	rot[2].z = 1.0;
	rot[1].v[1] = 1.0;
	rot[0].x = 1.0;
	rad = degrees * 0.017453292;
	if ((COERCE_UNSIGNED_INT(degrees * 0.017453292) & 0x7F800000) == 2139095040
		&& !Assert_MyHandler("c:\\t6\\code\\src_noserver\\universal\\com_math.cpp", 908, 0, "(!IS_NAN(rad))", nullptr))
	{
		__debugbreak();
	}
	rot[0].x = cosf(rad);
	rot[0].y = sinf(rad);
	if ((LODWORD(rot[0].y) & 0x7F800000) == 2139095040
		&& !Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(!IS_NAN(zrot[0][1]))",
			nullptr))
	{
		__debugbreak();
	}
	v14 = rot[0].x;
	if ((LODWORD(rot[0].x) & 0x7F800000) == 2139095040)
	{
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(!IS_NAN(zrot[0][0]))",
			nullptr))
			__debugbreak();
		v14 = rot[0].x;
	}
	v15 = ((-rot[0].y * vup.x) + (v14 * vr.x)) + (rot[2].x * vf.x);
	v16 = ((v14 * vup.x) + (rot[0].y * vr.x)) + (rot[2].y * vf.x);
	v17 = ((rot[1].z * vup.x) + (rot[0].z * vr.x)) + (rot[2].z * vf.x);
	tmpmat[1].x = ((-rot[0].y * vup.y) + (v14 * vr.y)) + (rot[2].x * vf.y);
	v18 = ((v14 * vup.y) + (rot[0].y * vr.y)) + (rot[2].y * vf.y);
	tmpmat[2].x = ((-rot[0].y * vup.z) + (v14 * vr.z)) + (rot[2].x * vf.z);
	v19 = ((v14 * vup.z) + (rot[0].y * vr.z)) + (rot[2].y * vf.z);
	tmpmat[1].z = ((rot[1].z * vup.y) + (rot[0].z * vr.y)) + (rot[2].z * vf.y);
	tmpmat[2].z = ((rot[1].z * vup.z) + (rot[0].z * vr.z)) + (rot[2].z * vf.z);
	rot[0].x = ((v16 * vup.x) + (v15 * m[0].x)) + (v17 * vf.x);
	rot[0].z = ((v16 * vup.z) + (v15 * vr.z)) + (v17 * m[2].z);
	v20 = (((v16 * m[1].y) + (v15 * vr.y)) + (v17 * vf.y)) * point->y;
	v21 = ((v18 * vup.x) + (tmpmat[1].x * m[0].x)) + (tmpmat[1].z * vf.x);
	rot[1].z = ((v18 * vup.z) + (tmpmat[1].x * vr.z)) + (tmpmat[1].z * m[2].z);
	v22 = ((v18 * m[1].y) + (tmpmat[1].x * vr.y)) + (tmpmat[1].z * vf.y);
	v23 = ((v19 * vup.x) + (tmpmat[2].x * m[0].x)) + (tmpmat[2].z * vf.x);
	v24 = ((v19 * m[1].y) + (tmpmat[2].x * vr.y)) + (tmpmat[2].z * vf.y);
	v25 = ((v19 * vup.z) + (tmpmat[2].x * vr.z)) + (tmpmat[2].z * m[2].z);
	dst->x = (v20 + (rot[0].x * point->x)) + (rot[0].z * point->z);
	dst->y = ((v22 * point->y) + (v21 * point->x)) + (rot[1].z * point->z);
	dst->z = ((v24 * point->y) + (v23 * point->x)) + (v25 * point->z);
}

void Vec3Basis_RightHanded(const vec3_t* forward, vec3_t* left, vec3_t* up)
{
	float v3; // xmm3_4
	const char* v4; // eax
	float v5; // xmm1_4
	float v6; // xmm2_4
	int v7; // eax
	float v8; // xmm0_4
	float v9; // xmm0_4
	float v10; // xmm3_4
	float v11; // [esp+2Ch] [ebp-10h]
	float v12; // [esp+30h] [ebp-Ch]
	float v13; // [esp+34h] [ebp-8h]

	v3 = 1.0;
	if (sqrtf(
		((forward->x * forward->x) + (forward->y * forward->y))
		+ (forward->z * forward->z)) - 1.0 >= 0.0020000001)
	{
		v4 = va(
			"(%g %g %g) len %g",
			forward->x,
			forward->y,
			forward->z,
			sqrtf(((forward->x * forward->x) + (forward->y * forward->y)) + (forward->z * forward->z)));
		if (!Assert_MyHandler(
			__FILE__,
			__LINE__,
			0,
			"(Vec3IsNormalized( src ))",
			"%s\n\t%s",
			"Vec3IsNormalized( src )",
			v4))
			__debugbreak();
		v3 = 1.0;
	}
	v5 = forward->y * forward->y;
	v6 = forward->z * forward->z;
	v11 = forward->x * forward->x;
	v12 = v5;
	v13 = v6;
	v7 = v11 > v5;
	if (*(&v11 + v7) > v6)
		v7 = 2;
	v8 = -(forward->x + v7);
	up->x = forward->x * v8;
	up->y = v8 * forward->y;
	up->z = v8 * forward->z;
	*(&up->x + v7) = *(&up->x + v7) + v3;
	v9 = sqrtf(((up->x * up->x) + (up->y * up->y)) + (up->z * up->z));
	if (-v9 >= 0.0)
		v9 = v3;
	v10 = v3 / v9;
	up->x = up->x * v10;
	up->y = up->y * v10;
	up->z = up->z * v10;
	Vec3Cross(up, forward, left);
}

void UnitQuatToAngles(const vec4_t* quat, vec3_t* angles)
{
	vec3_t axis[3]; // [esp+4h] [ebp-28h]

	UnitQuatToAxis(quat, axis);
	AxisToAngles(axis, angles);
}

float RadiusFromBounds(const vec3_t* mins, const vec3_t* maxs)
{
	float v2; // xmm0_4
	float v3; // xmm0_4
	float v5; // [esp+0h] [ebp-14h]
	float v6; // [esp+4h] [ebp-10h]

	v2 = mins->x;
	if (v2 <= maxs->x)
		v2 = maxs->x;
	v6 = v2;
	v3 = mins->y;
	if (v3 <= maxs->y)
		v3 = maxs->y;
	if (mins->z <= maxs->z)
		v5 = maxs->z;
	else
		v5 = mins->z;
	return sqrtf(v5 * v5 + v3 * v3 + v6 * v6);
}

float RadiusFromBounds2D(const vec2_t* mins, const vec2_t* maxs)
{
	float v2; // xmm0_4
	float v4; // [esp+0h] [ebp-10h]

	v2 = mins->v[0];
	if (v2 <= maxs->v[0])
		v2 =maxs->v[0];
	if (mins->v[1] <= maxs->v[1])
		v4 = maxs->v[1];
	else
		v4 =mins->v[1];
	return sqrtf(v4 * v4 + v2 * v2);
}

void SnapPointToIntersectingPlanes(const float** planes, vec3_t* xyz, float snapGrid, float snapEpsilon)
{
	float v4; // xmm3_4
	vec3_t* v5; // eax
	int v6; // edx
	int v7; // esi
	float v8; // xmm0_4
	const float* v9; // eax
	float v10; // xmm2_4
	float v11; // xmm4_4
	float v12; // xmm5_4
	const float* v13; // eax
	const float* v14; // eax
	float v15; // xmm6_4
	vec3_t snapped; // [esp+10h] [ebp-10h]

	v4 = snapEpsilon;
	v5 = xyz;
	v6 = &snapped - xyz;
	v7 = 3;
	do
	{
		v8 = (((1.0 / snapGrid) * v5->x) + 9.313225746154785e-10) * snapGrid;
		if (snapEpsilon <= (v8 - v5->x))
			*(&v5->x + v6) = v5->x;
		else
			*(&v5->x + v6) = v8;
		v5 = (v5 + 4);
		--v7;
	} while (v7);
	if (snapped.x != xyz->x || snapped.y != xyz->y || snapped.z != xyz->z)
	{
		v10 = 0.0;
		if (
			((((*planes)[1] * snapped.y) + (**planes * snapped.x)) + ((*planes)[2] * snapped.z))
			- (*planes)[3] > 0.0)
			v10 = ((((*planes)[1] * snapped.y) + (**planes * snapped.x)) + ((*planes)[2] * snapped.z)) - (*planes)[3];
		v11 = xyz->y;
		v12 = xyz->x;
		v9 = *planes;
		if ((((v9[1] * v11) + (*v9 * xyz->x)) + (v9[2] * xyz->z)) - v9[3] > snapEpsilon)
			v4 = (((v9[1] * v11) + (*v9 * xyz->x)) + (v9[2] * xyz->z)) - v9[3];
		v13 = planes[1];
		if ((((v13[1] * snapped.y) + (*v13 * snapped.x)) + (v13[2] * snapped.z)) - v13[3] > v10)
			v10 = (((v13[1] * snapped.y) + (*v13 * snapped.x)) + (v13[2] * snapped.z)) - v13[3];
		if ((((v13[1] * v11) + (*v13 * v12)) + (v13[2] * xyz->z)) - v13[3] > v4)
			v4 = (((v13[1] * v11) + (*v13 * v12)) + (v13[2] * xyz->z)) - v13[3];
		v14 = planes[2];
		v15 = snapped.z;
		if ((((v14[1] * snapped.y) + (*v14 * snapped.x)) + (v14[2] * snapped.z)) - v14[3] > v10)
			v10 = (((v14[1] * snapped.y) + (*v14 * snapped.x)) + (v14[2] * snapped.z)) - v14[3];
		if ((((v14[1] * v11) + (*v14 * v12)) + (v14[2] * xyz->z)) - v14[3] > v4)
			v4 = (((v14[1] * v11) + (*v14 * v12)) + (v14[2] * xyz->z)) - v14[3];
		if (v4 > v10)
		{
			*&xyz->x = *&snapped.x;
			xyz->z = v15;
		}
	}
}

void MatrixSet44(vec4_t* out, const vec3_t* origin, const vec3_t* axis, float scale)
{
	double v4; // st7

	out->v[0] = axis->x * scale;
	out->v[1] = axis->y * scale;
	out->v[2] = axis->z * scale;
	out->v[3] = 0.0;
	out[1].v[0] = axis[1].x * scale;
	out[1].v[1] = axis[1].y * scale;
	out[1].v[2] = axis[1].z * scale;
	out[1].v[3] = 0.0;
	out[2].v[0] = axis[2].x * scale;
	out[2].v[1] = axis[2].y * scale;
	out[2].v[2] = axis[2].z * scale;
	out[2].v[3] = 0.0;
	out[3].v[0] = origin->x;
	out[3].v[1] = origin->y;
	v4 = origin->z;
	out[3].v[3] = 1.0;
	out[3].v[2] = v4;
}
