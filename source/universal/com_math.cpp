#include "com_math.h"

#include <cmath>
#include <universal/assertive.h>

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

void PerpendicularVector(vec3_t const&, vec3_t&)
{
}

float PointToLineSegmentDistSq2D(vec2_t const&, vec2_t const&, vec2_t const&)
{
	return 0.0f;
}

float PointToLineSegmentDistSq(vec3_t const&, vec3_t const&, vec3_t const&)
{
	return 0.0f;
}

int BoxInPlanes(vec4_t const*, unsigned int, vec3_t const&, vec3_t const&)
{
	return 0;
}

int SphereInPlanes(vec4_t const*, unsigned int, vec3_t const&, float)
{
	return 0;
}
