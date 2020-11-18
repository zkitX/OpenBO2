#include "com_math.h"

#include <cmath>
#include <universal/assertive.h>

#include <defs.h>

vec3_t vec3_origin = { 0,0,0 };
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
	float fVar1;
	float fVar2;
	double dVar3;
	int local_8;

	local_8 = 1;
	if (x < 0.0) {
		local_8 = -1;
	}
	dVar3 = (double)((unsigned long long)x & 0x7fffffffffffffff) / 1.414213562373095;
	fVar1 = (float)dVar3;
	fVar2 = (float)1.442695040888963 * -(fVar1 * fVar1);
	fVar1 = round(fVar2);
	fVar2 = (powf(2, fVar2 - fVar1)) - 1;
	fVar1 = (float)scale((float)1 + fVar2, fVar1);
	fVar2 = (float)(1.0 / (dVar3 * 0.3275911 + 1.0));
	return (double)(((float)1 +
		(float)local_8 *
		((float)1 -
			(((((float)1.061405429 * fVar2 - (float)1.453152027) * fVar2 +
				(float)1.421413741) * fVar2 - (float)0.284496736) * fVar2 +
				(float)0.254829592) * fVar2 * fVar1)) * (float)0.5);
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

void AnglesSubtract(vec3_t const&, vec3_t const&, vec3_t&)
{
}

void AnglesSubtract(vec2_t const&, vec2_t const&, vec2_t&)
{
}

float AngleNormalize360(float)
{
	return 0.0f;
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

void AxisClear(vec3_t* const)
{
}

void AxisCopy(vec3_t const* const, vec3_t* const)
{
}

void AxisTranspose(vec3_t const* const, vec3_t* const)
{
}

void AxisTransformVec3(vec3_t const* const, vec3_t const&, vec3_t&)
{
}

void YawToAxis(float, vec3_t* const)
{
}

void AxisToAngles(vec3_t const* const, vec3_t&)
{
}

void Axis4ToAngles(vec4_t const* const, vec3_t&)
{
}

int IntersectPlanes(float const** const, vec3_t&)
{
	return 0;
}

int ProjectedWindingContainsCoplanarPoint(vec3_t const* const, int, int, int, vec3_t const&)
{
	return 0;
}
