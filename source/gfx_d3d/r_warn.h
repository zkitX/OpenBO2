#pragma once
#include "r_shared.h"

#include <universal/dvar.h>

enum GfxWarningType
{
	R_WARN_FRONTEND_ENT_LIMIT = 0x0,
	R_WARN_KNOWN_MODELS = 0x1,
	R_WARN_KNOWN_SPECIAL_MODELS = 0x2,
	R_WARN_TEXTURE_OVERRIDES = 0x3,
	R_WARN_MODEL_LIGHT_CACHE = 0x4,
	R_WARN_MODEL_LIGHTING_HANDLE_ALLOC_FAILED = 0x5,
	R_WARN_SCENE_ENTITIES = 0x6,
	R_WARN_TEMP_SKIN_BUF_SIZE = 0x7,
	R_WARN_MAX_SKINNED_CACHE_VERTICES = 0x8,
	R_WARN_MAX_SCENE_SURFS_SIZE = 0x9,
	R_WARN_MAX_SCENE_CULLINFOS_SIZE = 0xA,
	R_WARN_PORTAL_PLANES = 0xB,
	R_WARN_MAX_CLOUDS = 0xC,
	R_WARN_MAX_DLIGHTS = 0xD,
	R_WARN_SMODEL_LIGHTING = 0xE,
	R_WARN_MAX_DRAWSURFS = 0xF,
	R_WARN_GFX_CODE_MESH_LIMIT = 0x10,
	R_WARN_GFX_MARK_MESH_LIMIT = 0x11,
	R_WARN_MAX_SCENE_DRAWSURFS = 0x12,
	R_WARN_MAX_FX_DRAWSURFS = 0x13,
	R_WARN_NONEMISSIVE_FX_MATERIAL = 0x14,
	R_WARN_NONLIGHTMAP_MARK_MATERIAL = 0x15,
	R_WARN_PRIM_DRAW_SURF_BUFFER_SIZE = 0x16,
	R_WARN_CMDBUF_OVERFLOW = 0x17,
	R_WARN_MISSING_DECL_NONDEBUG = 0x18,
	R_WARN_MAX_DYNENT_REFS = 0x19,
	R_WARN_MAX_SCENE_DOBJ_REFS = 0x1A,
	R_WARN_MAX_SCENE_MODEL_REFS = 0x1B,
	R_WARN_MAX_SCENE_BRUSH_REFS = 0x1C,
	R_WARN_MAX_CODE_INDS = 0x1D,
	R_WARN_MAX_CODE_VERTS = 0x1E,
	R_WARN_MAX_CODE_ARGS = 0x1F,
	R_WARN_MAX_MARK_INDS = 0x20,
	R_WARN_MAX_MARK_VERTS = 0x21,
	R_WARN_DEBUG_ALLOC = 0x22,
	R_WARN_SPOT_LIGHT_LIMIT = 0x23,
	R_WARN_FX_ELEM_LIMIT = 0x24,
	R_WARN_VTX_DATA_OVERFLOW = 0x25,
	R_WARN_WORKER_CMD_SIZE = 0x26,
	R_WARN_UNKNOWN_STATICMODEL_SHADER = 0x27,
	R_WARN_UNKNOWN_XMODEL_SHADER = 0x28,
	R_WARN_DYNAMIC_INDEX_BUFFER_SIZE = 0x29,
	R_WARN_MICROCODE_BUFFER_SIZE = 0x2A,
	R_WARN_TOO_MANY_LIGHT_GRID_POINTS = 0x2B,
	R_WARN_MAX_TRANSFER_REQUESTS = 0x2C,
	R_WARN_MAX_SHADER_CONSTANT_SETS = 0x2D,
	R_WARN_SHADER_CONSTANT_SET_ALLOCATE = 0x2E,
	R_WARN_MISSING_TECHNIQUE = 0x2F,
	R_WARN_DEPTH_HACK_NOT_IN_DRAWLIST = 0x30,
	R_WARN_WORLD_IN_DEPTH_HACK_DRAWLIST = 0x31,
	R_WARN_VIEWMODEL_MISSING_STENCIL = 0x32,
	R_WARN_MARKS_ERR_PROBE_MISMATCH = 0x33,
	R_WARN_MARKS_ERR_LMAP_MISMATCH = 0x34,
	R_WARN_OCCLUSION_QUERY = 0x35,
	R_WARN_MAX_OCCLUSION_QUERIES = 0x36,
	R_WARN_FX_ACTIONS_LIMIT = 0x37,
	R_WARN_FX_UNIQUE_HANDLES_LIMIT = 0x38,
	R_WARN_MAX_QUEUED_FX_SPAWN_WORKERS = 0x39,
	R_WARN_TENSION_VERT_COUNT = 0x3A,
	R_WARN_TENSION_PATCH_COUNT = 0x3B,
	R_WARN_COUNT = 0x3C,
};


const dvar_t* r_warningRepeatDelay;
unsigned int s_warnCount[60];

const char* s_warnFormat[]{
	"entity buffer exceeded - not drawing model", 
	"too many existing models (more than %i)", 
	"too many existing special models (more than %i)", 
	"too many texture overrides (more than %i)", 
	"model light cache alloc failed - not drawing model", 
	"model lighting handle alloc failed - not drawing model", 
	"too many scene entities (more than %i)", 
	"TEMP_SKIN_BUF_SIZE exceeded - not skinning surface", 
	"Max skinned verts exceeded - not drawing surface", 
	"MAX_SCENE_SURFS_SIZE(1) exceeded - not drawing surface", 
	"MAX_SCENE_CULLINFOS_SIZE(1) exceeded - not culling surface", 
	"Portal plane buffer full - flushing", 
	"GFX_PARTICLE_CLOUD_LIMIT(256) exceeded - not drawing particle cloud", 
	"MAX_ADDED_DLIGHTS(32) exceeded.", 
	"Too many visible static models - not drawing static model", 
	"MAX_DRAWSURFS(1) exceeded - not drawing geometry", 
	"GFX_CODE_MESH_LIMIT(1) exceeded - not drawing code mesh", 
	"GFX_MARK_MESH_LIMIT(1) exceeded - not drawing mark mesh", 
	"Max scene drawsurfs exceed from %s %d - not drawing surface", 
	"Max fx drawsurfs %i exceed for region %i - not drawing surface", 
	"non effect material '%s' used for effect (or code geom) '%s'", 
	"non impact mark material '%s' used for impact mark", 
	"PRIM_DRAW_SURF_BUFFER_SIZE((128 * 512)) exceeded - not drawing surface", 
	"command buffer overflow - not drawing surface",
	"Missing decl '%s' techset '%s' tech '%s' shader '%s' (ignore for debug settings).", 
	"Max dyn ent refs exceeded", 
	"Max scene dobj refs (%i) exceeded", 
	"Max scene model refs (%i) exceeded", 
	"Max scene brush refs (%i) exceeded", 
	"GFX_CODE_MESH_INDEX_LIMIT((1 * 6 * 5)) exceeded", 
	"GFX_CODE_MESH_VERT_LIMIT((1 * 4 * 3)) exceeded", 
	"GFX_CODE_MESH_ARGS_LIMIT(256) exceeded", 
	"GFX_MARK_MESH_INDEX_LIMIT((1 * 6)) exceeded", 
	"GFX_MARK_MESH_VERT_LIMIT((1 * 4)) exceeded", 
	"Out of debug memory for (%s)", 
	"FX_SPOT_LIGHT_LIMIT(1) exceeded - not spawning spot light effect",
	"FX_ELEM_LIMIT(2048) exceeded - not spawning fx elem",
	"Dynamic vertex buffer overflow",
	"worker command size exceeded for type %i",
	"Unknown static model shader (%s)",
	"Unknown xmodel shader",
	"DYNAMIC_INDEX_BUFFER_SIZE exceeded - speed warning",
	"GFX_MICROCODE_BUFFER_SIZE exceeded - things won't look right",
	"Too many light grid points",
	"too many transfer requests (more than %i)",
	"max %i shader constant sets exceeded",
	"failed to allocate shader constant %s - all constants are used",
	"%s is missing %s(%d), using %s(%d)",
	"viewmodel material '%s' not in viewmodel drawlist - change sortkey",
	"world material '%s' in viewmodel drawlist - change sortkey",
	"viewmodel material '%s' does not use stencil - sorting issue",
	"Marks probe mismatch %d,%d mark %s surf %s",
	"Marks lmap mismatch %d,%d mark %s surf %s",
	"drawnSampleCount (%d) > occlusionQuerySamples (%d)",
	"Max occlusion queries (%i) exceeded",
	"Max fx actions (%i) exceeded",
	"Max fx unique handles (%i) exceeded",
	"Max fx_spawn workers (%i) exceeded",
	"Tension allocation failed for %s, vert count (%i) exceeds (%i)",
	"Tension allocation failed for %s, maximum patches exceeded (%i)"
};

float R_UpdateFrameRate();
void R_WarnOncePerFrame(GfxWarningType warnType, ...);
void R_WarnInitDvars();
