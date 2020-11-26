#pragma once
#include "r_rendercmds.h"

#include <d3d11.h>
#include <universal/com_workercmds.h>

class GfxWorkerData
{
	int cmdBufValid[33];
	GfxCmdBuf cmdBuf[33];
	jqBatchGroup cmdBufBatchGroup[33];
};

int r_add_bspCallback_Implementation(jqBatch*, void*);
int r_add_bsp_sunCallback_Implementation(jqBatch*, void*);
int r_add_staticmodelCallback_Implementation(jqBatch*, void*);
int r_add_staticmodel_sunCallback_Implementation(jqBatch*, void*);
int r_add_bsp_dlightCallback_Implementation(jqBatch*, void*);
int r_add_smodels_dlightCallback_Implementation(jqBatch*, void*);
int r_model_skinCallback_Implementation(jqBatch*, void*);
int r_draw_litCallback_Implementation(jqBatch*, void*);
int r_draw_lit_bspCallback_Implementation(jqBatch*, void*);
int r_draw_lit_static_modelCallback_Implementation(jqBatch*, void*);
int fx_update_remainingCallback_Implementation(jqBatch*, void*);
int fx_post_updateCallback_Implementation(jqBatch*, void*);
int fx_draw_nonspriteCallback_Implementation(jqBatch*, void*);
int fx_marks_drawCallback_Implementation(jqBatch*, void*);
int fx_spawnCallback_Implementation(jqBatch*, void*);
int r_add_bspCallback(jqBatch*);
int r_add_bsp_sunCallback(jqBatch*);
int r_add_staticmodelCallback(jqBatch*);
int r_add_staticmodel_sunCallback(jqBatch*);
int r_add_bsp_dlightCallback(jqBatch*);
int r_add_smodels_dlightCallback(jqBatch*);
int r_model_skinCallback(jqBatch*);
int r_draw_litCallback(jqBatch*);
int r_draw_lit_bspCallback(jqBatch*);
int r_draw_lit_static_modelCallback(jqBatch*);
int r_draw_depthCallback(jqBatch*);
int r_dpvs_staticCallback(jqBatch*);
int r_dpvs_dynmodelCallback(jqBatch*);
int r_dpvs_sceneentCallback(jqBatch*);
int r_dpvs_dynbrushCallback(jqBatch*);
int fx_updateCallback(jqBatch*);
int fx_update_remainingCallback(jqBatch*);
int fx_update_portalledCallback(jqBatch*);
int fx_post_updateCallback(jqBatch*);
int fx_drawCallback(jqBatch*);
int fx_update_nonspriteCallback(jqBatch*);
int fx_draw_nonspriteCallback(jqBatch*);
int dobj_skelCallback(jqBatch*);
int dobj_skinCallback(jqBatch*);
int model_skelCallback(jqBatch*);
int fx_marks_drawCallback(jqBatch*);
int fx_update_spotCallback(jqBatch*);
int fx_spawnCallback(jqBatch*);