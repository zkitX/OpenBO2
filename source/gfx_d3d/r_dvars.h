#pragma once

#include <universal/dvar.h>

#pragma region DVARS
const dvar_t* r_smp_worker_thread[8];
const dvar_t* developer;
const dvar_t* exp_softclip;
const dvar_t* r_aaSamples;
const dvar_t* r_aaSamplesMax;
const dvar_t* r_alphaMap;
const dvar_t* r_anaglyphFX_enable;
const dvar_t* r_aspectRatio;
const dvar_t* r_aspectRatioWindow;
const dvar_t* r_autoLodScale;
const dvar_t* r_autopriority;
const dvar_t* r_blinkCullNone;
const dvar_t* r_blinkLayers;
const dvar_t* r_blinkLitQuasiOpaque;
const dvar_t* r_blinkTrans;
const dvar_t* r_bloomHiQuality;
const dvar_t* r_bloomTweaks;
const dvar_t* r_blur;
const dvar_t* r_brightness;
const dvar_t* r_brushLimit;
const dvar_t* r_cameraInfo;
const dvar_t* r_clearColor;
const dvar_t* r_clearColor2;
const dvar_t* r_clipCodec;
const dvar_t* r_clipFPS;
const dvar_t* r_clipSize;
const dvar_t* r_colorMap;
const dvar_t* r_contrast;
const dvar_t* r_convergence;
const dvar_t* r_cubicUpsampleParam;
const dvar_t* r_currentMonitor;
const dvar_t* r_customMode;
const dvar_t* r_debugMetalStorm;
const dvar_t* r_debugPerformance;
const dvar_t* r_debugShader;
const dvar_t* r_debugShowCoronas;
const dvar_t* r_debugShowDynamicLights;
const dvar_t* r_debugShowPrimaryLightLines;
const dvar_t* r_debugShowPrimaryLights;
const dvar_t* r_defaultMode;
const dvar_t* r_desaturation;
const dvar_t* r_disableGenericFilter;
const dvar_t* r_dlightLimit;
const dvar_t* r_dobjLimit;
const dvar_t* r_dofHDR;
const dvar_t* r_dof_bias;
const dvar_t* r_dof_enable;
const dvar_t* r_dof_farBlur;
const dvar_t* r_dof_farEnd;
const dvar_t* r_dof_farStart;
const dvar_t* r_dof_nearBlur;
const dvar_t* r_dof_nearEnd;
const dvar_t* r_dof_nearStart;
const dvar_t* r_dof_showdebug;
const dvar_t* r_dof_tweak;
const dvar_t* r_dof_viewModelEnd;
const dvar_t* r_dof_viewModelStart;
const dvar_t* r_dpvs_useCellForceInvisibleBits;
const dvar_t* r_drawBModels;
const dvar_t* r_drawDebugFogParams;
const dvar_t* r_drawDynEnts;
const dvar_t* r_drawInfo;
const dvar_t* r_drawLightmapDrawlists;
const dvar_t* r_drawLitDrawlists;
const dvar_t* r_drawModelsGobo;
const dvar_t* r_drawPoly;
const dvar_t* r_drawPrimCap;
const dvar_t* r_drawPrimFloor;
const dvar_t* r_drawPrimHistogram;
const dvar_t* r_drawSModels;
const dvar_t* r_drawSModelsLV;
const dvar_t* r_drawSceneEnts;
const dvar_t* r_drawSceneModels;
const dvar_t* r_drawSkinnedModels;
const dvar_t* r_drawSun;
const dvar_t* r_drawWater;
const dvar_t* r_drawWorld;
const dvar_t* r_drawXModels;
const dvar_t* r_dualPlayActive;
const dvar_t* r_dualPlayEnable;
const dvar_t* r_dualPlayTestCount;
const dvar_t* r_enableCubicUpsample;
const dvar_t* r_enableFlashlight;
const dvar_t* r_enableGlassDpvs;
const dvar_t* r_enableOccluders;
const dvar_t* r_enablePlayerShadow;
const dvar_t* r_exposureTweak;
const dvar_t* r_exposureValue;
const dvar_t* r_extracam_custom_aspectratio;
const dvar_t* r_extracam_lod_scale;
const dvar_t* r_filmLut;
const dvar_t* r_filmTweakBrightness;
const dvar_t* r_filmTweakContrast;
const dvar_t* r_filmTweakDesaturation;
const dvar_t* r_filmTweakEnable;
const dvar_t* r_filmTweakInvert;
const dvar_t* r_filmTweakLut;
const dvar_t* r_filmUseTweaks;
const dvar_t* r_finalShiftX;
const dvar_t* r_finalShiftY;
const dvar_t* r_flameFX_FPS;
const dvar_t* r_flameFX_distortionScaleFactor;
const dvar_t* r_flameFX_fadeDuration;
const dvar_t* r_flameFX_magnitude;
const dvar_t* r_flameScaler;
const dvar_t* r_flame_allowed;
const dvar_t* r_flashLightBobAmount;
const dvar_t* r_flashLightBobRate;
const dvar_t* r_flashLightBrightness;
const dvar_t* r_flashLightColor;
const dvar_t* r_flashLightEndRadius;
const dvar_t* r_flashLightFlickerAmount;
const dvar_t* r_flashLightFlickerRate;
const dvar_t* r_flashLightFovInnerFraction;
const dvar_t* r_flashLightOffset;
const dvar_t* r_flashLightRange;
const dvar_t* r_flashLightShadows;
const dvar_t* r_flashLightSpecularScale;
const dvar_t* r_flashLightStartRadius;
const dvar_t* r_fog;
const dvar_t* r_fogBaseDist;
const dvar_t* r_fogBaseHeight;
const dvar_t* r_fogColor;
const dvar_t* r_fogHalfDist;
const dvar_t* r_fogHalfHeight;
const dvar_t* r_fogOpacity;
const dvar_t* r_fogSunColor;
const dvar_t* r_fogSunInner;
const dvar_t* r_fogSunOpacity;
const dvar_t* r_fogSunOuter;
const dvar_t* r_fogSunPitch;
const dvar_t* r_fogSunYaw;
const dvar_t* r_fogTweak;
const dvar_t* r_fog_disable;
const dvar_t* r_fontResolution;
const dvar_t* r_forceCameraHdg;
const dvar_t* r_forceCameraNode;
const dvar_t* r_forceCameraRecord;
const dvar_t* r_forceLod;
const dvar_t* r_fullPrepass;
const dvar_t* r_fullbright;
const dvar_t* r_fullscreen;
const dvar_t* r_genericSceneVector0;
const dvar_t* r_genericSceneVector1;
const dvar_t* r_genericSceneVector2;
const dvar_t* r_genericSceneVector3;
const dvar_t* r_genericSceneVector_debug;
const dvar_t* r_glossMap;
const dvar_t* r_grassEnable;
const dvar_t* r_grassWindForceEnable;
const dvar_t* r_heroLightScale;
const dvar_t* r_heroLighting;
const dvar_t* r_hidePartTestOn;
const dvar_t* r_highLodDist;
const dvar_t* r_ignoreHwGamma;
const dvar_t* r_lightConflicts;
const dvar_t* r_lightGridContrast;
const dvar_t* r_lightGridEnableTweaks;
const dvar_t* r_lightGridIntensity;
const dvar_t* r_lightMapFilterDisable;
const dvar_t* r_lightMapSecondary;
const dvar_t* r_lightTweakAmbient;
const dvar_t* r_lightTweakAmbientColor;
const dvar_t* r_lightTweakSunColor;
const dvar_t* r_lightTweakSunDiffuseColor;
const dvar_t* r_lightTweakSunDirection;
const dvar_t* r_lightTweakSunLight;
const dvar_t* r_lightmapOnlyBspSurfs;
const dvar_t* r_lightmapOnlyEntSurfs;
const dvar_t* r_lightmapOnlySModelSurfs;
const dvar_t* r_loadForRenderer;
const dvar_t* r_loadingScreen;
const dvar_t* r_lockPvs;
const dvar_t* r_lockPvsInCode;
const dvar_t* r_lodBiasRigid;
const dvar_t* r_lodBiasSkinned;
const dvar_t* r_lodScaleRigid;
const dvar_t* r_lodScaleSkinned;
const dvar_t* r_lowLodDist;
const dvar_t* r_lowestLodDist;
const dvar_t* r_matTwkPreviewSize;
const dvar_t* r_materialXYZ;
const dvar_t* r_mediumLodDist;
const dvar_t* r_missile_cam_debug_display;
const dvar_t* r_mode;
const dvar_t* r_modelLimit;
const dvar_t* r_modelSkelWorker;
const dvar_t* r_modelVertColor;
const dvar_t* r_monitor;
const dvar_t* r_monitorCount;
const dvar_t* r_multiGpu;
const dvar_t* r_multi_extracam_debug;
const dvar_t* r_noipak;
const dvar_t* r_norefresh;
const dvar_t* r_normalMap;
const dvar_t* r_num_viewports;
const dvar_t* r_occQueryDebug;
const dvar_t* r_occlusionMap;
const dvar_t* r_open_automate;
const dvar_t* r_optimize;
const dvar_t* r_optimizeLightmaps;
const dvar_t* r_outdoorAwayBias;
const dvar_t* r_outdoorDownBias;
const dvar_t* r_outdoorFeather;
const dvar_t* r_picmip;
const dvar_t* r_picmip_bump;
const dvar_t* r_picmip_manual;
const dvar_t* r_picmip_spec;
const dvar_t* r_pix_material;
const dvar_t* r_poisonFX_blurMax;
const dvar_t* r_poisonFX_blurMin;
const dvar_t* r_poisonFX_debug_enable;
const dvar_t* r_poisonFX_dvisionX;
const dvar_t* r_poisonFX_pulse;
const dvar_t* r_portalMinRecurseDepth;
const dvar_t* r_portalOccluderTest;
const dvar_t* r_portalWalkLimit;
const dvar_t* r_previousFullscreen;
const dvar_t* r_primaryLightTweakDiffuseStrength;
const dvar_t* r_primaryLightTweakSpecularStrength;
const dvar_t* r_primaryLightUseTweaks;
const dvar_t* r_qrcode_debug_display;
const dvar_t* r_qrcode_debug_display_size;
const dvar_t* r_remotescreen_renderlastframe;
const dvar_t* r_resampleScene;
const dvar_t* r_resolution;
const dvar_t* r_reviveFX_debug;
const dvar_t* r_reviveFX_edgeAmount;
const dvar_t* r_reviveFX_edgeColorTemp;
const dvar_t* r_reviveFX_edgeContrast;
const dvar_t* r_reviveFX_edgeMaskAdjust;
const dvar_t* r_reviveFX_edgeOffset;
const dvar_t* r_reviveFX_edgeSaturation;
const dvar_t* r_reviveFX_edgeScale;
const dvar_t* r_rimIntensity;
const dvar_t* r_rimIntensity_debug;
const dvar_t* r_scaleViewport;
const dvar_t* r_seethru_decal_enable;
const dvar_t* r_shaderDebugA;
const dvar_t* r_shaderDebugB;
const dvar_t* r_shaderDebugC;
const dvar_t* r_shaderWarming;
const dvar_t* r_shaderWarmingPerFrame;
const dvar_t* r_shader_constant_set_debug_range;
const dvar_t* r_shader_constant_set_enable;
const dvar_t* r_shaderbinddebug;
const dvar_t* r_showAabbTrees;
const dvar_t* r_showBounds;
const dvar_t* r_showCollision;
const dvar_t* r_showCollisionDepthTest;
const dvar_t* r_showCollisionDist;
const dvar_t* r_showCollisionGroups;
const dvar_t* r_showCollisionPolyType;
const dvar_t* r_showCullBModels;
const dvar_t* r_showCullDistDebug;
const dvar_t* r_showCullSModels;
const dvar_t* r_showCullXModels;
const dvar_t* r_showCullXmodelsEntNum;
const dvar_t* r_showDObjModelNamesDist;
const dvar_t* r_showDynEntModelNames;
const dvar_t* r_showEntModelNamesDist;
const dvar_t* r_showForceNoCull;
const dvar_t* r_showForcedInvisibleCells;
const dvar_t* r_showLightGrid;
const dvar_t* r_showLightingOrigins;
const dvar_t* r_showLodInfo;
const dvar_t* r_showMissingLightGrid;
const dvar_t* r_showOccluders;
const dvar_t* r_showOutdoorVolumeDebug;
const dvar_t* r_showPenetration;
const dvar_t* r_showPortals;
const dvar_t* r_showReflectionProbeSelection;
const dvar_t* r_showSModelNames;
const dvar_t* r_showShadowMapDebugText;
const dvar_t* r_showSkinned;
const dvar_t* r_showSunDirectionDebug;
const dvar_t* r_showSurfCounts;
const dvar_t* r_showTess;
const dvar_t* r_showTriCounts;
const dvar_t* r_showTris;
const dvar_t* r_showVertCounts;
const dvar_t* r_singleCell;
const dvar_t* r_skinnedCacheOverflowLodMaxScale;
const dvar_t* r_skinnedCacheOverflowLodScaleDecay;
const dvar_t* r_skipDrawTris;
const dvar_t* r_skipPvs;
const dvar_t* r_skyColorTemp;
const dvar_t* r_skyRotation;
const dvar_t* r_skyTransition;
const dvar_t* r_sky_intensity_angle0;
const dvar_t* r_sky_intensity_angle1;
const dvar_t* r_sky_intensity_factor0;
const dvar_t* r_sky_intensity_factor1;
const dvar_t* r_sky_intensity_showDebugDisplay;
const dvar_t* r_smp_worker;
const dvar_t* r_smp_worker_threads;
const dvar_t* r_sortBackToFront;
const dvar_t* r_sortDrawSurfsBsp;
const dvar_t* r_sortDrawSurfsStaticModel;
const dvar_t* r_specularMap;
const dvar_t* r_spotLightEntityShadows;
const dvar_t* r_spotLightSModelShadows;
const dvar_t* r_spotLightShadows;
const dvar_t* r_ssao;
const dvar_t* r_ssaoBias;
const dvar_t* r_ssaoDebug;
const dvar_t* r_ssaoIntensity;
const dvar_t* r_ssaoRadius;
const dvar_t* r_ssaoScale;
const dvar_t* r_staticModelDumpLodInfo;
const dvar_t* r_stereo3DAvailable;
const dvar_t* r_stereo3DEyeSeparation;
const dvar_t* r_stereo3DEyeSeparationScaler;
const dvar_t* r_stereo3DIPD;
const dvar_t* r_stereo3DMode;
const dvar_t* r_stereo3DOn;
const dvar_t* r_stereoFocusDepth;
const dvar_t* r_stereoGunShift;
const dvar_t* r_stereoTurretShift;
const dvar_t* r_sunFlareTint;
const dvar_t* r_sun_from_dvars;
const dvar_t* r_superFlareDraw;
const dvar_t* r_superflare_drawlist;
const dvar_t* r_swrk_override_characterCharredAmount;
const dvar_t* r_swrk_override_characterDissolveColor;
const dvar_t* r_swrk_override_wetness;
const dvar_t* r_tension_enable;
const dvar_t* r_texFilterAnisoMax;
const dvar_t* r_texFilterAnisoMin;
const dvar_t* r_texFilterDisable;
const dvar_t* r_texFilterMipBias;
const dvar_t* r_texFilterMipMode;
const dvar_t* r_texFilterQuality;
const dvar_t* r_txaa;
const dvar_t* r_txaaDepth1;
const dvar_t* r_txaaDepth2;
const dvar_t* r_txaaMode;
const dvar_t* r_txaaMotion1;
const dvar_t* r_txaaMotion2;
const dvar_t* r_txaaSupported;
const dvar_t* r_ui3d_debug_display;
const dvar_t* r_ui3d_h;
const dvar_t* r_ui3d_use_debug_values;
const dvar_t* r_ui3d_w;
const dvar_t* r_ui3d_x;
const dvar_t* r_ui3d_y;
const dvar_t* r_useHidePartbits;
const dvar_t* r_use_separate_char_tech;
const dvar_t* r_vc_compile;
const dvar_t* r_viewportBackingColor;
const dvar_t* r_vsync;
const dvar_t* r_vsyncThreshold;
const dvar_t* r_warmShaders;
const dvar_t* r_waterFogTest;
const dvar_t* r_waterSheetingFX_allowed;
const dvar_t* r_waterSheetingFX_distortionScaleFactor;
const dvar_t* r_waterSheetingFX_enable;
const dvar_t* r_waterSheetingFX_magnitude;
const dvar_t* r_waterSheetingFX_radius;
const dvar_t* r_waterWaveAmplitude;
const dvar_t* r_waterWaveAngle;
const dvar_t* r_waterWaveBase;
const dvar_t* r_waterWaveNormalScale;
const dvar_t* r_waterWavePhase;
const dvar_t* r_waterWaveScriptShader0;
const dvar_t* r_waterWaveScriptShader1;
const dvar_t* r_waterWaveSpeed;
const dvar_t* r_waterWaveSteepness;
const dvar_t* r_waterWaveWavelength;
const dvar_t* r_xdebug;
const dvar_t* r_zbufferDebug;
const dvar_t* r_zbufferDebugRange;
const dvar_t* r_zebrastripe;
const dvar_t* r_zfar;
const dvar_t* r_znear;
const dvar_t* r_znear_depthhack;
const dvar_t* r_zombieDarkness;
const dvar_t* r_zombieDisableEarthEffect;
const dvar_t* r_zombieDisableSlideEffect;
const dvar_t* r_zombieNameAllowDevList;
const dvar_t* r_zombieNameAllowFriendsList;
const dvar_t* sm_fastSunShadow;
const dvar_t* sm_maxLights;
const dvar_t* sm_polygonOffsetBias;
const dvar_t* sm_showOverlay;
const dvar_t* sm_showOverlayDepthBounds;
const dvar_t* sm_showSpotAxis;
const dvar_t* sm_spotEnable;
const dvar_t* sm_spotQuality;
const dvar_t* sm_spotShadowFadeTime;
const dvar_t* sm_spotShadowLargeRadiusScale;
const dvar_t* sm_strictCull;
const dvar_t* sm_sunAlwaysCastsShadow;
const dvar_t* sm_sunEnable;
const dvar_t* sm_sunQuality;
const dvar_t* sm_sunSampleSizeNear;
const dvar_t* sm_sunShadowCenter;
const dvar_t* sm_sunShadowScale;
const dvar_t* sm_sunShadowSmall;
const dvar_t* sm_sunShadowSmallEnable;
const dvar_t* vc_FBM;
const dvar_t* vc_FGM;
const dvar_t* vc_FSM;
const dvar_t* vc_HMB;
const dvar_t* vc_HMG;
const dvar_t* vc_HMR;
const dvar_t* vc_LIB;
const dvar_t* vc_LIG;
const dvar_t* vc_LIW;
const dvar_t* vc_LOB;
const dvar_t* vc_LOW;
const dvar_t* vc_LUT;
const dvar_t* vc_MMB;
const dvar_t* vc_MMG;
const dvar_t* vc_MMR;
const dvar_t* vc_RE;
const dvar_t* vc_RGBH;
const dvar_t* vc_RGBL;
const dvar_t* vc_RS;
const dvar_t* vc_SMB;
const dvar_t* vc_SMG;
const dvar_t* vc_SMR;
const dvar_t* vc_SNAP;
const dvar_t* vc_YH;
const dvar_t* vc_YL;
const dvar_t* vid_xpos;
const dvar_t* vid_ypos;
#pragma endregion

#pragma region DVARENUMS
const char* names[8]{
	"r_smp_worker_thread0", "r_smp_worker_thread1", "r_smp_worker_thread2", "r_smp_worker_thread3", "r_smp_worker_thread4", "r_smp_worker_thread5", "r_smp_worker_thread6", "r_smp_worker_thread7"
};
const char* codecNames[3]{
	"MJPEG",
	"VP8"
};
const char* clipSizeNames[5]{
	"360",
	"480",
	"720",
	"1080"
};

const char* mipFilterNames[5]{
	"Unchanged",
	"Force Trilinear",
	"Force Bilinear"
};

const char* debugShaderNames[7]{
	"none",
	"normal",
	"basisTangent",
	"basisBionormal",
	"basisNormal",
	"lmapDir"
};

const char* debugPerformanceNames[4]{
	"none",
	"current",
	"average"
};

const char* lightMapNames[34]{
	"Black",
	"Unchanged",
	"Intensity2",
	"Intensity3",
	"Intensity4",
	"Intensity5",
	"Intensity6",
	"Intensity7",
	"Intensity8",
	"Intensity9",
	"Intensity10",
	"Intensity11",
	"Intensity12",
	"Intensity13",
	"Intensity14",
	"Intensity15",
	"Intensity16",
	"Intensity17",
	"Intensity18",
	"Intensity19",
	"Intensity20",
	"Intensity21",
	"Intensity22",
	"Intensity23",
	"Intensity24",
	"Intensity25",
	"Intensity26",
	"Intensity27",
	"Intensity28",
	"Intensity29",
	"Intensity30",
	"Intensity31"
};

const char* colorMapNames[6]{
	"Black", "Unchanged", "White", "Gray Linear", "Gray sRGB"
};

const char* alphaMapNames[5]{
	"Unchanged", "255", "192", "128"
};
const char* specularMapNames[4]{
	"Non metal (sRGB 56)", "Unchanged", "Metal (sRGB 255)"
};
const char* glossMapNames[8]{
	"", "Unchanged", "255", "204", "153", "102", "51"
};
const char* occlusionMapNames[5]{
	"", "Unchanged", "255", "128"
};
const char* lodInfoNames[6]{
	"None", "All", "Current LOD", "Current Distance", "Auto LOD Distance Only"
};
const char* showPenetrationNames[4]{
	"off", "flash penetrable materials" "flash non-penetrable materials"
};
#pragma endregion

void R_RegisterDvars();