#pragma once

#include <xmmintrin.h>
#include <emmintrin.h>
#include <Directxmath.h>

using namespace DirectX;

#define NUMVERTEXNORMALS 162
#define M_PI 3.14159265358979323846

const float _mask__NegFloat_ = 0x80000000;

#define	PITCH 0	// up / down
#define	YAW	1	// left / right
#define	ROLL 2	// fall over

union vec2_t
{
	float v[2];
	struct {
		float x;
		float y;
	};
};

union vec3_t
{
	struct {
		float x;
		float y;
		float z;
	};
	float v[3];
};

struct cplane_s
{
	vec3_t normal;
	float dist;
	char type;
	char signbits;
	char pad[2];
};

union vec4_t
{
	float v[4];
	struct {
		float x;
		float y;
		float z;
		float w;
	};
	struct {
		float r;
		float g;
		float b;
		float a;
	};
};

inline vec4_t operator +(const vec4_t vec1, const vec4_t vec2) {
	vec4_t out;

	out.x = vec1.x + vec2.x;
	out.y = vec1.y + vec2.y;
	out.z = vec1.z + vec2.z;
	out.w = vec1.w + vec2.w;

	return out;
}

inline vec4_t operator *(const vec4_t vec1, const vec4_t vec2) {
	vec4_t out;

	out.x = vec1.x * vec2.x; 
	out.y = vec1.y * vec2.y;
	out.z = vec1.z * vec2.z;
	out.w = vec1.w * vec2.w;

	return out;
}

bool bops_initialized;

double AngleDelta(const float a1, const float a2);
double I_normCDF(double);
float random(void);
float crandom(void);
int RandWithSeed(int*);
float LinearTrack(float, float, float, float);
float LinearTrackAngle(float, float, float, float);
float DiffTrack(float, float, float, float);
float DiffTrackAngle(float, float, float, float);
float GraphGetValueFromFraction(int, union vec2_t const*, float);
float Q_acos(float);
signed char ClampChar(int);
short ClampShort(int);
char DirToByte(union vec3_t const*);
void ByteToDir(int, union vec3_t*);
int VecNCompareCustomEpsilon(const vec3_t*, const vec3_t*, float, int);
void Vec3ProjectionCoords(const vec3_t*, int*, int*);
float vectoyaw(union vec2_t const*);
double vectosignedyaw(union vec2_t const*);
float vectopitch(union vec3_t const*);
void vectoangles(union vec3_t const*, union vec3_t*);
void YawVectors(float, union vec3_t*, union vec3_t*);
void YawVectors2D(float, union vec2_t*, union vec2_t*);
void PerpendicularVector(union vec3_t const*, union vec3_t*);
float PointToLineSegmentDistSq2D(union vec2_t const*, union vec2_t const*, union vec2_t const*);
float PointToLineSegmentDistSq(union vec3_t const&, union vec3_t const&, union vec3_t const&);
int BoxInPlanes(union vec4_t const*, unsigned int, union vec3_t const*, union vec3_t const*);
int SphereInPlanes(const vec4_t*, unsigned int, const vec3_t*, const float);
void MatrixIdentity33(union vec3_t*);
void MatrixIdentity44(union vec4_t*);
void MatrixMultiply(const vec3_t*, const vec3_t*, vec3_t*);
void MatrixVecMultiply(const vec3_t*, const vec3_t*, vec3_t*);
void MatrixVecMultiplyProject(const vec4_t*, const vec3_t*, vec3_t*);
void MatrixMultiply43(const vec3_t*, const vec3_t*, vec3_t*);
void MatrixMultiply44(const vec4_t*, const vec4_t*, vec4_t*);
void MatrixTranspose(const vec3_t*, vec3_t*);
void MatrixTranspose44(const vec4_t*, vec4_t*);
void MatrixInverse(const vec3_t*, vec3_t*);
void MatrixInverseOrthogonal43(const vec3_t*, vec3_t*);
void MatrixInverse44(const vec4_t*, vec4_t*);
void MatrixTransformVector44(union vec4_t const&, union vec4_t const* const, union vec4_t&);
void MatrixTransformVector43(union vec3_t const&, union vec3_t const* const, union vec3_t&);
void MatrixTransposeTransformVector43(union vec3_t const&, union vec3_t const* const, union vec3_t&);
void MatrixTransformVector43Equals(union vec3_t&, union vec3_t const* const);
void VectorAngleMultiply(union vec2_t&, float);
void UnitQuatToAxis(const vec4_t*, vec3_t*);
void UnitQuatToForward(const vec4_t*, vec3_t*);
void QuatSlerp(union vec4_t const&, union vec4_t const&, float, union vec4_t&);
float RotationToYaw(const vec2_t*);
void FinitePerspectiveMatrix(float, float, float, float, vec4_t*);
void SpotLightViewMatrix(union vec3_t const&, float, union vec4_t* const);
void SpotLightViewMatrixDir3(const vec3_t*, const vec3_t*, const vec3_t*, vec4_t*);
void SpotLightProjectionMatrix(float, float, float, union vec4_t* const);
void InfinitePerspectiveMatrix(float, float, float, union vec4_t* const);
void MatrixForViewer(union vec3_t const&, union vec3_t const* const, union vec4_t* const);
float AngleSubtract(float, float);
void AnglesSubtract(const vec3_t*, const vec3_t*, vec3_t*);
void AnglesSubtract(const vec2_t*, const vec2_t*, vec2_t*);
float AngleNormalize360(float);
float RadiusFromBounds2DSq(const vec2_t*, const vec2_t*);
void ExtendBounds(vec3_t*, vec3_t*, const vec3_t*);
void ExpandBoundsToWidth(vec3_t*, vec3_t*);
void ClearBounds(vec3_t* mins, vec3_t* maxs);
void AddPointToBounds(const vec3_t*, vec3_t*, vec3_t*);
void AddPointToBounds2D(const vec2_t*, vec2_t*, vec2_t*);
int BoundsOverlap(const vec3_t*, const vec3_t*, const vec3_t*, const vec3_t*);
void ExpandBounds(const vec3_t*, const vec3_t*, vec3_t*, vec3_t*);
void AxisClear(vec3_t*);
void AxisCopy(const vec3_t*, vec3_t*);
void AxisTranspose(const vec3_t*, vec3_t*);
void AxisTransformVec3(const vec3_t*, const vec3_t*, vec3_t*);
void YawToAxis(float yaw, vec3_t* axis);
void AxisToAngles(const vec3_t*, vec3_t*);
void Axis4ToAngles(const vec4_t*, vec3_t*);
int IntersectPlanes(const float**, vec3_t*);
int ProjectedWindingContainsCoplanarPoint(const vec3_t*, int, int, int, const vec3_t*);
int PlaneFromPoints(vec4_t*, const vec3_t*, const vec3_t*, const vec3_t*);
void ProjectPointOnPlane(const vec3_t*, const vec3_t*, vec3_t*);
void SetPlaneSignbits(struct cplane_s*);
int BoxOnPlaneSide(const vec3_t*, const vec3_t*, const cplane_s*);
int IsPosInsideArc(const vec3_t* pos, const vec3_t* arcOrigin, float posRadius, float arcRadius, float arcAngle0, float arcAngle1);
float Q_rint(float);
float ColorNormalize(const vec3_t*, vec3_t*);
void ColorSRGBtoLinear(union vec3_t const&, union vec3_t&);
float PitchForYawOnNormal(const float, const vec3_t*);
void NearestPitchAndYawOnPlane(const vec3_t*, const vec3_t*, vec3_t*);
void Rand_Init(int);
unsigned int* GetRandSeed();
float flrand(float, float);
int irand(int, int);
void AxisToQuat(const vec3_t*, vec4_t*);
void QuatLerp(const vec4_t*, const vec4_t*, float, vec4_t*);
bool CullBoxFromCone(const vec3_t*, const vec3_t*, float, const vec3_t*, const vec3_t*);
bool CullBoxFromSphere(const vec3_t*, float, const vec3_t*, const vec3_t*);
bool CullBoxFromConicSectionOfSphere(const vec3_t*, const vec3_t*, float, float, const vec3_t*, const vec3_t*);
bool CullSphereFromCone(const vec3_t*, const vec3_t*, float, const vec3_t*, float);
void colorTempToXYZ(float, vec4_t*);
void colorTempMatrix(union vec4_t* const, float);
void colorHueMatrix(union vec4_t* const, float);
void colorSaturationMatrix(vec4_t*, float);
float I_fnormPDF(float);
float I_fnormCDF(float);
void RotatePointAroundVector(vec3_t*, const vec3_t*, const vec3_t*, const float);
void Vec3Basis_RightHanded(const vec4_t*, vec3_t*);
void UnitQuatToAngles(const vec4_t*, vec3_t*);
float RadiusFromBounds(const vec3_t*, const vec3_t*);
float RadiusFromBounds2D(const vec2_t*, const vec2_t*);
void SnapPointToIntersectingPlanes(const float**, vec3_t*, float, float);
void MatrixSet44(vec4_t*, const vec3_t*, const vec3_t*, float);