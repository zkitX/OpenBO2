#pragma once

#define NUMVERTEXNORMALS 162
#define M_PI 3.14159265358979323846

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
void PerpendicularVector(union vec3_t const&, union vec3_t&);
float PointToLineSegmentDistSq2D(union vec2_t const&, union vec2_t const&, union vec2_t const&);
float PointToLineSegmentDistSq(union vec3_t const&, union vec3_t const&, union vec3_t const&);
int BoxInPlanes(union vec4_t const*, unsigned int, union vec3_t const&, union vec3_t const&);
int SphereInPlanes(union vec4_t const*, unsigned int, union vec3_t const&, float);

// ----------------------------------------
// Matrix manipulation
// ----------------------------------------

void MatrixIdentity33(union vec3_t* const);
void MatrixIdentity44(union vec4_t* const);
void MatrixMultiply(union vec3_t const* const, union vec3_t const* const, union vec3_t* const);
void MatrixVecMultiply(union vec3_t const* const, union vec3_t const&, union vec3_t&);
void MatrixVecMultiplyProject(union vec4_t const* const, union vec3_t const&, union vec3_t&);
void MatrixMultiply43(union vec3_t const* const, union vec3_t const* const, union vec3_t* const);
void MatrixMultiply44(union vec4_t const* const, union vec4_t const* const, union vec4_t* const);
void MatrixTranspose(union vec3_t const* const, union vec3_t* const);
void MatrixTranspose44(union vec4_t const* const, union vec4_t* const);
void MatrixInverse(union vec3_t const* const, union vec3_t* const);
void MatrixInverseOrthogonal43(union vec3_t const* const, union vec3_t* const);
void MatrixInverse44(union vec4_t const* const, union vec4_t* const);
void MatrixTransformVector44(union vec4_t const&, union vec4_t const* const, union vec4_t&);
void MatrixTransformVector43(union vec3_t const&, union vec3_t const* const, union vec3_t&);
void MatrixTransposeTransformVector43(union vec3_t const&, union vec3_t const* const, union vec3_t&);
void MatrixTransformVector43Equals(union vec3_t&, union vec3_t const* const);

void VectorAngleMultiply(union vec2_t&, float);
void UnitQuatToAxis(union vec4_t const&, union vec3_t* const);
void UnitQuatToForward(union vec4_t const&, union vec3_t&);
void QuatSlerp(union vec4_t const&, union vec4_t const&, float, union vec4_t&);
float RotationToYaw(union vec2_t const&);
void FinitePerspectiveMatrix(float, float, float, float, union vec4_t* const);
void SpotLightViewMatrix(union vec3_t const&, float, union vec4_t* const);
void SpotLightViewMatrixDir3(union vec3_t const&, union vec3_t const&, union vec3_t const&, union vec4_t* const);
void SpotLightProjectionMatrix(float, float, float, union vec4_t* const);
void InfinitePerspectiveMatrix(float, float, float, union vec4_t* const);
void MatrixForViewer(union vec3_t const&, union vec3_t const* const, union vec4_t* const);
void AnglesSubtract(union vec3_t const&, union vec3_t const&, union vec3_t&);
void AnglesSubtract(union vec2_t const&, union vec2_t const&, union vec2_t&);
float AngleNormalize360(float);
float RadiusFromBounds2DSq(union vec2_t const&, union vec2_t const&);
void ExtendBounds(union vec3_t&, union vec3_t&, union vec3_t const&);
void ExpandBoundsToWidth(union vec3_t&, union vec3_t&);
void ClearBounds(vec3_t* mins, vec3_t* maxs);
void AddPointToBounds(union vec3_t const&, union vec3_t&, union vec3_t&);
void AddPointToBounds2D(union vec2_t const&, union vec2_t&, union vec2_t&);
int BoundsOverlap(union vec3_t const&, union vec3_t const&, union vec3_t const&, union vec3_t const&);
void ExpandBounds(union vec3_t const&, union vec3_t const&, union vec3_t&, union vec3_t&);
void AxisClear(union vec3_t* const);
void AxisCopy(union vec3_t const* const, union vec3_t* const);
void AxisTranspose(union vec3_t const* const, union vec3_t* const);
void AxisTransformVec3(union vec3_t const* const, union vec3_t const&, union vec3_t&);
void YawToAxis(float, union vec3_t* const);
void AxisToAngles(union vec3_t const* const, union vec3_t&);
void Axis4ToAngles(union vec4_t const* const, union vec3_t&);
int IntersectPlanes(float const** const, union vec3_t&);
int ProjectedWindingContainsCoplanarPoint(union vec3_t const* const, int, int, int, union vec3_t const&);
int PlaneFromPoints(union vec4_t&, union vec3_t const&, union vec3_t const&, union vec3_t const&);
void ProjectPointOnPlane(union vec3_t const&, union vec3_t const&, union vec3_t&);
void SetPlaneSignbits(struct cplane_s*);
int BoxOnPlaneSide(union vec3_t const&, union vec3_t const&, struct cplane_s const*);
int IsPosInsideArc(union vec3_t const&, float, union vec3_t const&, float, float, float, float);
float Q_rint(float);
float ColorNormalize(union vec3_t const&, union vec3_t&);
void ColorSRGBtoLinear(union vec3_t const&, union vec3_t&);
float PitchForYawOnNormal(float, union vec3_t const&);
void NearestPitchAndYawOnPlane(union vec3_t const&, union vec3_t const&, union vec3_t&);
void Rand_Init(int);
unsigned int* GetRandSeed(void);
float flrand(float, float);
int irand(int, int);
void AxisToQuat(union vec3_t const* const, union vec4_t&);
void QuatLerp(union vec4_t const&, union vec4_t const&, float, union vec4_t&);
bool CullBoxFromCone(union vec3_t const&, union vec3_t const&, float, union vec3_t const&, union vec3_t const&);
bool CullBoxFromSphere(union vec3_t const&, float, union vec3_t const&, union vec3_t const&);
bool CullBoxFromConicSectionOfSphere(union vec3_t const&, union vec3_t const&, float, float, union vec3_t const&, union vec3_t const&);
bool CullSphereFromCone(union vec3_t const&, union vec3_t const&, float, union vec3_t const&, float);
void colorTempToXYZ(float, union vec4_t&);
void colorTempMatrix(union vec4_t* const, float);
void colorHueMatrix(union vec4_t* const, float);
void colorSaturationMatrix(union vec4_t* const, float);
float I_fnormPDF(float);
float I_fnormCDF(float);
void RotatePointAroundVector(union vec3_t&, union vec3_t const&, union vec3_t const&, float);
void Vec3Basis_RightHanded(union vec3_t const&, union vec3_t&, union vec3_t&);
void UnitQuatToAngles(union vec4_t const&, union vec3_t&);
float RadiusFromBounds(union vec3_t const&, union vec3_t const&);
float RadiusFromBounds2D(union vec2_t const&, union vec2_t const&);
void SnapPointToIntersectingPlanes(float const** const, union vec3_t&, float, float);
void MatrixSet44(union vec4_t* const, union vec3_t const&, union vec3_t const* const, float);