#include "r_dvars.h"
#include "r_sky.h"

#include <qcommon/threads.h>

void __cdecl R_RegisterDvars()
{
    int value; // eax
    int v1; // esi
    unsigned __int16 v2; // di
    const char* v3; // ebx
    dvar_t* v4; // eax
    const dvar_t* v5; // edx
    dvar_t* v6; // eax
    bool v7; // al
    bool v8; // al
    dvar_t* v9; // eax
    unsigned int v10; // [esp+18h] [ebp-10h]
    const char* dvarName; // [esp+20h] [ebp-8h]
    bool v12; // [esp+24h] [ebp-4h]

    R_RegisterSunDvars();
    r_fontResolution = _Dvar_RegisterInt(
        "r_fontResolution",
        720,
        720,
        1080,
        0x40u,
        "Console font resolution setting. The value is 720 or 1080.");
    r_resolution = _Dvar_RegisterInt("r_resolution", 720, 480, 1080, 1u, "Console Resolution setting.");
    r_ssao = _Dvar_RegisterBool("r_ssao", 0, 0, "Enable SSAO.");
    r_ssaoScale = _Dvar_RegisterFloat("r_ssaoScale", 50.0, 0.0, 1000.0, 0, "SSAO scale.");
    r_ssaoRadius = _Dvar_RegisterFloat("r_ssaoRadius", 12.0, 0.0, 200.0, 0, "SSAO radius.");
    r_ssaoBias = _Dvar_RegisterFloat("r_ssaoBias", 0.55000001, 0.0, 5.0, 0, "SSAO bias.");
    r_ssaoIntensity = _Dvar_RegisterFloat("r_ssaoIntensity", 0.52999997, 0.0, 1.0, 0, "SSAO intensity.");
    r_ssaoDebug = _Dvar_RegisterBool("r_ssaoDebug", 0, 0, "SSAO debug.");
    r_txaa = _Dvar_RegisterBool("r_txaa", 0, 0, "Enable TXAA.");
    r_txaaSupported = _Dvar_RegisterBool("r_txaaSupported", 0, 0, "True if TXAA is supported.");
    r_txaaMode = _Dvar_RegisterInt("r_txaaMode", -1, -1, 7, 0, "TXAA mode.");
    r_txaaDepth1 = _Dvar_RegisterFloat("r_txaaDepth1", 0.02, 0.0, 1.0, 0, "TXAA depth1");
    r_txaaDepth2 = _Dvar_RegisterFloat("r_txaaDepth2", 1.0, 0.0, 1.0, 0, "TXAA depth2");
    r_txaaMotion1 = _Dvar_RegisterFloat("r_txaaMotion1", 32.0, 0.0, 128.0, 0, "TXAA motion1");
    r_txaaMotion2 = _Dvar_RegisterFloat("r_txaaMotion2", 0.0099999998, 0.0, 128.0, 0, "TXAA motion2");
    r_clipCodec = _Dvar_RegisterEnum("r_clipCodec", codecNames, 1, 0, "Codec used when rendering clips.");
    r_clipSize = _Dvar_RegisterEnum("r_clipSize", clipSizeNames, 1, 0, "Render clip size.");
    r_clipFPS = _Dvar_RegisterInt("r_clipFPS", 30, 10, 60, 0, "Render clip FPS.");
    vid_xpos = _Dvar_RegisterInt("vid_xpos", 3, -4096, 4096, 1u, "Game window horizontal position");
    vid_ypos = _Dvar_RegisterInt("vid_ypos", 22, -4096, 4096, 1u, "game window vertical position");
    r_autopriority = _Dvar_RegisterBool(
        "r_autopriority",
        0,
        1u,
        "Automatically set the priority of the windows process when the game is minimized");
    r_fullscreen = _Dvar_RegisterInt(
        "r_fullscreen",
        1,
        0,
        2,
        0x21u,
        "Game window mode (window, fullscreen, fullscreen window)");
    r_previousFullscreen = _Dvar_RegisterInt(
        "r_previousFullscreen",
        1,
        0,
        2,
        1u,
        "Prevous fullscreen mode (fullscreen, fullscreen window)");
    r_convergence = _Dvar_RegisterFloat("r_convergence", 6.0, 0.1, 100.0, 0, "Change 3D convergence");
    r_ignoreHwGamma = _Dvar_RegisterBool("r_ignorehwgamma", 0, 0x21u, "Ignore hardware gamma");
    r_texFilterAnisoMax = _Dvar_RegisterInt(
        "r_texFilterAnisoMax",
        16,
        1,
        16,
        1u,
        "Maximum anisotropy to use for texture filtering");
    r_texFilterDisable = _Dvar_RegisterBool(
        "r_texFilterDisable",
        0,
        0x80u,
        "Disables all texture filtering (uses nearest only.)");
    r_texFilterAnisoMin = _Dvar_RegisterInt(
        "r_texFilterAnisoMin",
        1,
        1,
        16,
        1u,
        "Minimum anisotropy to use for texture filtering (overridden by max)");
    r_texFilterMipMode = _Dvar_RegisterEnum(
        "r_texFilterMipMode",
        mipFilterNames,
        0,
        1u,
        "Forces all mipmaps to use a particular blend between levels (or disables mipping.)");
    r_texFilterMipBias = _Dvar_RegisterFloat("r_texFilterMipBias", 0.0, -16.0, 15.99, 0x80u, "Change the mipmap bias");
    r_fullbright = _Dvar_RegisterBool("r_fullbright", 0, 0x80u, "Toggles rendering without lighting");
    r_debugShader = _Dvar_RegisterEnum("r_debugShader", debugShaderNames, 0, 0x80u, "Enable normal map debug shader");
    r_debugPerformance = _Dvar_RegisterEnum(
        "r_debugPerformance",
        debugPerformanceNames,
        0,
        0x80u,
        "Enable performance debug shader");
    r_lightConflicts = _Dvar_RegisterBool("r_lightConflicts", 0, 0x80u, "Shows conflicts between primary lights");
    r_zebrastripe = _Dvar_RegisterInt(
        "r_zebrastripe",
        0,
        0,
        3,
        0x80u,
        "Material intensity threshold, 1 = Diffuse Black, 2 = Macbeth Black, 3 = 18% Average Grey");
    r_loadingScreen = _Dvar_RegisterBool("r_loadingScreen", 0, 0x4000u, "True if the game is playing a loading movie");
    r_multiGpu = _Dvar_RegisterBool("r_multiGpu", 1, 1u, "Optimize for multiple GPUs (if present)");
    r_texFilterQuality = _Dvar_RegisterInt("r_texFilterQuality", 1, 0, 2, 0x21u, "Texture filtering quality.");
    r_lodScaleRigid = _Dvar_RegisterFloat(
        "r_lodScaleRigid",
        1.0,
        0.1,
        10.0,
        0x80u,
        "Scale the level of detail distance for rigid models (larger reduces detail)");
    r_lodBiasRigid = _Dvar_RegisterFloat(
        "r_lodBiasRigid",
        0.0,
        -10000.0,
        10000.0,
        0x80u,
        "Bias the level of detail distance for rigid models (negative increases detail)");
    r_lodScaleSkinned = _Dvar_RegisterFloat(
        "r_lodScaleSkinned",
        1.0,
        0.1,
        10.0,
        0x80u,
        "Scale the level of detail distance for skinned models (larger reduces detail)");
    r_lodBiasSkinned = _Dvar_RegisterFloat(
        "r_lodBiasSkinned",
        0.0,
        -10000.0,
        10000.0,
        0x80u,
        "Bias the level of detail distance for skinned models (negative increases detail)");
    r_skinnedCacheOverflowLodMaxScale = _Dvar_RegisterFloat(
        "r_skinnedCacheOverflowLodMaxScale",
        4.0,
        1.0,
        10.0,
        0,
        "Maximum LOD scale when skinned vertex cache is overflowed");
    r_skinnedCacheOverflowLodScaleDecay = _Dvar_RegisterFloat(
        "r_skinnedCacheOverflowLodScaleDecay",
        3.0,
        0.0099999998,
        10.0,
        0,
        "Amount of time it takes the LOD scale to settle back to 1.0f after it overflowed");
    r_autoLodScale = _Dvar_RegisterBool("r_autoLodScale", 0, 0x80u, "Enable FPS-based auto LOD scale");
    r_znear = _Dvar_RegisterFloat(
        "r_znear",
        4.0,
        1.0,
        10000.0,
        (r_glob.isMultiplayer ? 0x80 : 0) | 0x5000,
        "Things closer than this aren't drawn.  Reducing this increases z-fighting in the distance.");
    r_znear_depthhack = _Dvar_RegisterFloat("r_znear_depthhack", 0.1, 0.001, 16.0, 0x80u, "Viewmodel near clip plane");
    r_zfar = _Dvar_RegisterFloat(
        "r_zfar",
        0.0,
        0.0,
        3.4028235e38,
        0x80u,
        "Change the distance at which culling fog reaches 100% opacity; 0 is off");
    r_fog = _Dvar_RegisterBool("r_fog", 1, 0x80u, "Set to 0 to disable fog");
    r_picmip_manual = _Dvar_RegisterBool(
        "r_picmip_manual",
        0,
        1u,
        "If 0, picmip is set automatically.  If 1, picmip is set based on the other r_picmip dvars.");
    r_picmip = _Dvar_RegisterInt(
        "r_picmip",
        0,
        0,
        3,
        1u,
        "Picmip level of color maps.  If r_picmip_manual is 0, this is read-only.");
    r_picmip_bump = _Dvar_RegisterInt(
        "r_picmip_bump",
        0,
        0,
        3,
        1u,
        "Picmip level of normal maps.  If r_picmip_manual is 0, this is read-only.");
    r_picmip_spec = _Dvar_RegisterInt(
        "r_picmip_spec",
        0,
        0,
        3,
        1u,
        "Picmip level of specular maps.  If r_picmip_manual is 0, this is read-only.");
    r_lightMapSecondary = _Dvar_RegisterEnum(
        "r_lightMapSecondary",
        lightMapNames,
        1,
        0x80u,
        "Replace secondary lightmaps with varying intensities");
    r_lightMapFilterDisable = _Dvar_RegisterBool("r_lightMapFilterDisable", 0, 0x80u, "Disable filtering on lightmaps");
    r_colorMap = _Dvar_RegisterEnum("r_colorMap", colorMapNames, 1, 0x80u, "Override all color maps");
    r_normalMap = _Dvar_RegisterFloat("r_normalMap", 0.0, 0.0, 1.0, 0x80u, "Lerp all normal maps to a flat normal map");
    r_drawLitDrawlists = _Dvar_RegisterBool("r_drawLitDrawlists", 1, 0x80u, "disable drawing of the LIT drawlists");
    r_drawLightmapDrawlists = _Dvar_RegisterBool(
        "r_drawLightmapDrawlists",
        1,
        0x80u,
        "disable drawing of the LIGHTMAP drawlists");
    r_alphaMap = _Dvar_RegisterEnum("r_alphaMap", alphaMapNames, 0, 0x80u, "Override all alpha maps");
    r_specularMap = _Dvar_RegisterEnum("r_specularMap", specularMapNames, 1, 0x80u, "Replace all specular maps");
    r_glossMap = _Dvar_RegisterEnum("r_glossMap", glossMapNames, 1, 0x80u, "Replace all gloss maps");
    r_occlusionMap = _Dvar_RegisterEnum("r_occlusionMap", occlusionMapNames, 1, 0x80u, "Replace all occlusion maps");
    r_warmShaders = _Dvar_RegisterBool("r_warmShaders", 0, 0, "Set to true to warm pixel shaders next frame");
    r_loadForRenderer = _Dvar_RegisterBool(
        "r_loadForRenderer",
        1,
        0x20u,
        "Set to false to disable dx allocations (for dedicated server mode)");
    r_shaderWarmingPerFrame = _Dvar_RegisterInt(
        "r_shaderWarmingPerFrame",
        5,
        1,
        50,
        0,
        "Incremental number of shaders to warm per frame");
    r_showLodInfo = _Dvar_RegisterEnum("r_showLodInfo", lodInfoNames, 0, 0x80u, "Show LOD info");
    r_showTris = _Dvar_RegisterInt("r_showTris", 0, 0, 3, 0x80u, "Show triangle outlines");
    r_showTriCounts = _Dvar_RegisterBool("r_showTriCounts", 0, 0x80u, "Triangle count for each rendered entity");
    r_showSurfCounts = _Dvar_RegisterBool("r_showSurfCounts", 0, 0x80u, "Surface count for each rendered entity");
    r_showVertCounts = _Dvar_RegisterBool("r_showVertCounts", 0, 0x80u, "Vertex count for each entity");
    r_resampleScene = _Dvar_RegisterBool(
        "r_resampleScene",
        1,
        0x80u,
        "Upscale the frame buffer with sharpen filter and color correction.");
    r_showPenetration = _Dvar_RegisterEnum(
        "r_showPenetration",
        showPenetrationNames,
        0,
        0x80u,
        "Shows materials tagged with 'nopenetrate'");
    r_xdebug = _Dvar_RegisterInt("r_xdebug", 0, 0, 3, 0x80u, "xmodel/xanim debug rendering");
    r_showForceNoCull = _Dvar_RegisterBool("r_showForceNoCull", 0, 0x80u, "Show ents with force no cull set");
    r_showSkinned = _Dvar_RegisterBool("r_showSkinned", 0, 0x80u, "Show skinned ents");
    r_showBounds = _Dvar_RegisterBool("r_showBounds", 0, 0x80u, "Show entity bounds");
    r_showLightGrid = _Dvar_RegisterBool("r_showLightGrid", 0, 0x80u, "Show light grid debugging information");
    r_showLightingOrigins = _Dvar_RegisterBool("r_showLightingOrigins", 0, 0x80u, "Show lighting origins for models");
    r_showMissingLightGrid = _Dvar_RegisterBool(
        "r_showMissingLightGrid",
        1,
        0,
        "Use rainbow colors for entities that are outside the light grid");
    r_showReflectionProbeSelection = _Dvar_RegisterEnum(
        "r_showReflectionProbeSelection",
        showReflectionProbeSelectionNames,
        0,
        0x80u,
        "Show reflection probe selection");
    r_hidePartTestOn = _Dvar_RegisterBool("r_hidePartTestOn", 0, 0, "enable viewmodel hide bone test");
    r_lightTweakAmbient = _Dvar_RegisterFloat("r_lightTweakAmbient", 0.1, 0.0, 4.0, 0x240u, "Ambient light strength");
    r_lightTweakSunLight = _Dvar_RegisterFloat("r_lightTweakSunLight", 1.0, 0.0, 32.0, 0x1200u, "Sunlight strength");
    r_lightTweakAmbientColor = _Dvar_RegisterColor(
        "r_lightTweakAmbientColor",
        1.0,
        0.0,
        0.0,
        1.0,
        0x240u,
        "Light ambient color");
    r_lightTweakSunColor = _Dvar_RegisterLinearRGB("r_lightTweakSunColor", 1.0, 1.0, 0.0, 0.0, 1.0, 0x1200u, "Sun color");
    r_lightTweakSunDiffuseColor = _Dvar_RegisterColor(
        "r_lightTweakSunDiffuseColor",
        0.0,
        0.0,
        1.0,
        1.0,
        0x240u,
        "Sun diffuse color");
    r_lightTweakSunDirection = _Dvar_RegisterVec3(
        "r_lightTweakSunDirection",
        0.0,
        0.0,
        0.0,
        -360.0,
        360.0,
        0x1200u,
        "Sun direction in degrees");
    r_showSunDirectionDebug = _Dvar_RegisterBool(
        "r_showSunDirectionDebug",
        0,
        0x80u,
        "Toggles the display of sun direction debug");
    r_showOutdoorVolumeDebug = _Dvar_RegisterBool(
        "r_showOutdoorVolumeDebug",
        0,
        0x80u,
        "Toggles the display of particle outdoor volume debug");
    r_materialXYZ = _Dvar_RegisterColorXYZ(
        "r_materialXYZ",
        3.8599999,
        3.98,
        4.5500002,
        0.0,
        100.0,
        0,
        "material color checker XYZ d65 coords");
    r_drawInfo = _Dvar_RegisterEnum("r_drawInfo", r_drawInfoNames, 0, 0x4000u, "rendering debug info");
    r_drawPrimHistogram = _Dvar_RegisterBool(
        "r_drawPrimHistogram",
        0,
        0x80u,
        "Draws a histogram of the sizes of each primitive batch");
    r_norefresh = _Dvar_RegisterBool("r_norefresh", 0, 0x80u, "Skips all rendering.  Useful for benchmarking.");
    r_scaleViewport = _Dvar_RegisterFloat(
        "r_scaleViewport",
        1.0,
        0.0,
        1.0,
        0x80u,
        "Scale 3D viewports by this fraction.  Use this to see if framerate is pixel shader bound.");
    r_smp_worker = _Dvar_RegisterBool("r_smp_worker", 1, 0, "Process renderer front end in a separate thread");
    r_smp_worker_threads = _Dvar_RegisterInt("r_smp_worker_threads", Sys_GetDefaultWorkerThreadsCount(), 2, 8, 0, "Number of worker threads");
    v1 = 0;
    if (Dvar_GetInt(r_smp_worker_threads) > 0)
    {
        do
        {
            dvarName = names[v1];
            v2 = 0x4000;
            v12 = 1;
            v3 = "Enable worker thread";
            if (v1 >= Sys_GetDefaultWorkerThreadsCount())
            {
                v2 = 64;
                v12 = 0;
                v3 = "Worker thread always disabled; not enough hardware threads";
            }
            v4 = _Dvar_RegisterBool(dvarName, v12, v2, v3);
            v5 = r_smp_worker_threads;
            r_smp_worker_thread[v1++] = v4;
        } while (v1 < Dvar_GetInt(v5));
    }
    r_showCullDistDebug = _Dvar_RegisterBool("r_showCullDistDebug", 0, 0x80u, "Toggles corner culldist debug display");
    r_showShadowMapDebugText = _Dvar_RegisterBool(
        "r_showShadowMapDebugText",
        0,
        0x80u,
        "Toggles corner shadowmap debug display");
    r_aaSamples = _Dvar_RegisterInt(
        "r_aaSamples",
        1,
        1,
        16,
        0x21u,
        "Anti-aliasing sample count; 1 disables anti-aliasing");
    r_aaSamplesMax = _Dvar_RegisterInt(
        "r_aaSamplesMax",
        8,
        1,
        16,
        0,
        "Maximum number of samples supported for anti-aliasing.");
    r_vsync = _Dvar_RegisterBool(
        "r_vsync",
        1,
        1u,
        "Enable v-sync before drawing the next frame to avoid 'tearing' artifacts.");
    r_vsyncThreshold = _Dvar_RegisterInt(
        "r_vsyncThreshold",
        10,
        0,
        100,
        0,
        "Specify threshold percentage for allowed vsync 'tearing'.");
    r_clearColor = _Dvar_RegisterColor(
        "r_clearColor",
        0.25,
        0.375,
        0.5,
        1.0,
        0,
        "Color to clear the screen to when clearing the frame buffer");
    r_clearColor2 = _Dvar_RegisterColor(
        "r_clearColor2",
        0.5,
        0.25,
        0.0,
        1.0,
        0,
        "Color to clear every second frame to (for use during development)");
    r_viewportBackingColor = _Dvar_RegisterColor(
        "r_viewportBackingColor",
        0.079999998,
        0.079999998,
        0.079999998,
        1.0,
        0,
        "Color to clear the frame buffer with before compositing viewports");
    r_drawSun = _Dvar_RegisterBool("r_drawSun", 1, 0, "Enable sun effects");
    r_drawWorld = _Dvar_RegisterBool("r_drawWorld", 1, 0x80u, "Enable world rendering");
    r_lightTweakSunDirection = _Dvar_RegisterBool(
        "r_drawSceneEnts",
        1,
        0x80u,
        "Enable scene entity rendering");
    r_drawSceneModels = _Dvar_RegisterBool("r_drawSceneModels", 1, 0x80u, "Enable scene model rendering");
    r_drawPoly = _Dvar_RegisterBool("r_drawPoly", 1, 0x80u, "Enable poly rendering");
    r_drawDynEnts = _Dvar_RegisterBool("r_drawDynEnts", 1, 0x80u, "Enable dynamic entity rendering");
    r_drawBModels = _Dvar_RegisterBool("r_drawBModels", 1, 0x80u, "Enable brush model rendering");
    r_drawSModels = _Dvar_RegisterBool("r_drawSModels", 1, 0x80u, "Enable static model rendering");
    r_drawSModelsLV = _Dvar_RegisterBool("r_drawSModelsLV", 1, 0x80u, "Enable vertex baked static model rendering");
    r_drawModelsGobo = _Dvar_RegisterBool("r_drawModelsGobo", 1, 0x80u, "Enable model gobo rendering");
    r_drawXModels = _Dvar_RegisterBool("r_drawXModels", 1, 0x80u, "Enable xmodel rendering");
    r_drawSkinnedModels = _Dvar_RegisterBool("r_drawSkinnedModels", 1, 0x80u, "Enable skinned rendering");
    r_dlightLimit = _Dvar_RegisterInt(
        "r_dlightLimit",
        4,
        0,
        4,
        0,
        "Maximum number of dynamic lights drawn simultaneously");
    r_dobjLimit = _Dvar_RegisterInt("r_dobjLimit", 512, 0, 1024, 0x80u, "Maximum number of DObjs drawn simultaneously");
    r_modelLimit = _Dvar_RegisterInt(
        "r_modelLimit",
        1024,
        0,
        1024,
        0x80u,
        "Maximum number of models drawn simultaneously");
    r_brushLimit = _Dvar_RegisterInt(
        "r_brushLimit",
        512,
        0,
        1024,
        0x80u,
        "Maximum number of brushes drawn simultaneously");
    r_spotLightShadows = _Dvar_RegisterBool("r_spotLightShadows", 1, 0x80u, "Enable shadows for spot lights.");
    r_spotLightSModelShadows = _Dvar_RegisterBool(
        "r_spotLightSModelShadows",
        1,
        0x80u,
        "Enable static model shadows for spot lights.");
    r_spotLightEntityShadows = _Dvar_RegisterBool(
        "r_spotLightEntityShadows",
        1,
        0x80u,
        "Enable entity shadows for spot lights.");
    if (r_glob.isMultiplayer)
    {
        r_flashLightFovInnerFraction = _Dvar_RegisterFloat(
            "r_flashLightFovInnerFraction",
            0.69999999,
            0.0,
            0.99000001,
            0x5000u,
            "Relative Inner FOV angle for the dynamic flash light. 0 is full fade 0.99 is almost no fade.");
        r_flashLightStartRadius = _Dvar_RegisterFloat(
            "r_flashLightStartRadius",
            0.0,
            0.0,
            1200.0,
            0x5000u,
            "Radius of the circle at the start of the flash light in inches.");
        r_flashLightEndRadius = _Dvar_RegisterFloat(
            "r_flashLightEndRadius",
            300.0,
            1.0,
            1200.0,
            0x5000u,
            "Radius of the circle at the end of the flash light in inches.");
        r_flashLightShadows = _Dvar_RegisterBool("r_flashLightShadows", 1, 0x5000u, "Enable shadows for flash light.");
        r_flashLightBrightness = _Dvar_RegisterFloat(
            "r_flashLightBrightness",
            10.0,
            0.0,
            25.0,
            0x5000u,
            "Brightness scale for flash light.");
        r_flashLightOffset = _Dvar_RegisterVec3(
            "r_flashLightOffset",
            0.0,
            2.0,
            1.0,
            -100.0,
            100.0,
            0x5000u,
            "Relative position of the Flashlight.");
        r_flashLightRange = _Dvar_RegisterFloat(
            "r_flashLightRange",
            350.0,
            1.0,
            1200.0,
            0x5000u,
            "Distance of the flash light in inches.");
        r_flashLightColor = _Dvar_RegisterVec3("r_flashLightColor", 1.0, 1.0, 1.0, 0.0, 1.0, 0x5000u, "Flashlight Color.");
        r_flashLightBobAmount = _Dvar_RegisterVec3(
            "r_flashLightBobAmount",
            0.0,
            0.0,
            0.0,
            0.0,
            50.0,
            0x5000u,
            "Flashlight Movement Amount.");
        r_flashLightBobRate = _Dvar_RegisterVec3(
            "r_flashLightBobRate",
            0.0,
            0.2,
            0.30000001,
            0.0,
            2.0,
            0x5000u,
            "Flashlight Movement Rate.");
        r_flashLightSpecularScale = _Dvar_RegisterFloat(
            "r_flashLightSpecularScale",
            1.0,
            0.0,
            10.0,
            0x5000u,
            "Flashlight Specular Scale.");
        v6 = _Dvar_RegisterFloat("r_flashLightFlickerAmount", 0.0, 0.0, 1.0, 0x5000u, "Flashlight Flicker Amount.");
        v10 = 20480;
    }
    else
    {
        r_flashLightFovInnerFraction = _Dvar_RegisterFloat(
            "r_flashLightFovInnerFraction",
            0.69999999,
            0.0,
            0.99000001,
            0x1000u,
            "Relative Inner FOV angle for the dynamic flash light. 0 is full fade 0.99 is almost no fade.");
        r_flashLightStartRadius = _Dvar_RegisterFloat(
            "r_flashLightStartRadius",
            0.0,
            0.0,
            1200.0,
            0x1000u,
            "Radius of the circle at the start of the flash light in inches.");
        r_flashLightEndRadius = _Dvar_RegisterFloat(
            "r_flashLightEndRadius",
            300.0,
            1.0,
            1200.0,
            0x1000u,
            "Radius of the circle at the end of the flash light in inches.");
        r_flashLightShadows = _Dvar_RegisterBool("r_flashLightShadows", 1, 0x1000u, "Enable shadows for flash light.");
        r_flashLightBrightness = _Dvar_RegisterFloat(
            "r_flashLightBrightness",
            10.0,
            0.0,
            25.0,
            0x1000u,
            "Brightness scale for flash light.");
        r_flashLightOffset = _Dvar_RegisterVec3(
            "r_flashLightOffset",
            0.0,
            2.0,
            1.0,
            -100.0,
            100.0,
            0x1000u,
            "Relative position of the Flashlight.");
        r_flashLightRange = _Dvar_RegisterFloat(
            "r_flashLightRange",
            350.0,
            1.0,
            1200.0,
            0x1000u,
            "Distance of the flash light in inches.");
        r_flashLightColor = _Dvar_RegisterVec3("r_flashLightColor", 1.0, 1.0, 1.0, 0.0, 1.0, 0x1000u, "Flashlight Color.");
        r_flashLightBobAmount = _Dvar_RegisterVec3(
            "r_flashLightBobAmount",
            0.0,
            0.0,
            0.0,
            0.0,
            50.0,
            0x1000u,
            "Flashlight Movement Amount.");
        r_flashLightBobRate = _Dvar_RegisterVec3(
            "r_flashLightBobRate",
            0.0,
            0.2,
            0.30000001,
            0.0,
            2.0,
            0x1000u,
            "Flashlight Movement Rate.");
        r_flashLightSpecularScale = _Dvar_RegisterFloat(
            "r_flashLightSpecularScale",
            1.0,
            0.0,
            10.0,
            0x1000u,
            "Flashlight Specular Scale.");
    }
    r_flashLightFlickerAmount = _Dvar_RegisterFloat("r_flashLightFlickerAmount", 0.0, 0.0, 1.0, 0x1000u, "Flashlight Flicker Amount.");;
    r_flashLightFlickerRate = _Dvar_RegisterFloat(
        "r_flashLightFlickerRate",
        65.0,
        0.1,
        65.0,
        4096,
        "Flashlight Flicker Rate (Times per second).");
    r_drawPrimCap = _Dvar_RegisterInt(
        "r_drawPrimCap",
        0,
        -1,
        10000,
        0x80u,
        "Only draw primitive batches with less than this many triangles");
    r_drawPrimFloor = _Dvar_RegisterInt(
        "r_drawPrimFloor",
        0,
        0,
        10000,
        0x80u,
        "Only draw primitive batches with more than this many triangles");
    r_skipDrawTris = _Dvar_RegisterBool("r_skipDrawTris", 0, 0x80u, "Skip drawing primitive tris.");
    r_drawWater = _Dvar_RegisterBool("r_drawWater", 1, 0x80u, "Enable water rendering");
    r_lockPvs = _Dvar_RegisterBool(
        "r_lockPvs",
        0,
        0x80u,
        "Request to lock the viewpoint used for determining what is visible to the current position and direction");
    r_lockPvsInCode = _Dvar_RegisterBool(
        "r_lockPvsInCode",
        0,
        0x80u,
        "Indicates if pvslock is currently on, in code. Only code should modified this dvar");
    r_skipPvs = _Dvar_RegisterBool(
        "r_skipPvs",
        0,
        0x80u,
        "Skipt the determination of what is in the potentially visible set (disables most drawing)");
    r_singleCell = _Dvar_RegisterBool(
        "r_singleCell",
        0,
        0x80u,
        "Only draw things in the same cell as the camera.  Most useful for seeing how big the current cell is.");
    r_showForcedInvisibleCells = _Dvar_RegisterBool(
        "r_showForcedInvisibleCells",
        0,
        0x80u,
        "Only draw cells that are forced to be off.");
    r_portalWalkLimit = _Dvar_RegisterInt(
        "r_portalWalkLimit",
        0,
        0,
        100,
        0x80u,
        "Stop portal recursion after this many iterations.  Useful for debugging portal errors.");
    r_portalMinRecurseDepth = _Dvar_RegisterInt(
        "r_portalMinRecurseDepth",
        2,
        0,
        100,
        0x80u,
        "Ignore r_portalMinClipArea for portals with fewer than this many parent portals.");
    r_enableOccluders = _Dvar_RegisterBool("r_enableOccluders", 1, 0x80u, "Enable occluders");
    r_enableGlassDpvs = _Dvar_RegisterBool("r_enableGlassDpvs", 1, 0x80u, "Enable DPVS for glass");
    r_portalOccluderTest = _Dvar_RegisterBool("r_portalOccluderTest", 1, 0x80u, "Test portals against occluders");
    r_showPortals = _Dvar_RegisterInt("r_showPortals", 0, 0, 3, 0x80u, "Show portals for debugging");
    r_showOccluders = _Dvar_RegisterInt("r_showOccluders", 0, 0, 4, 0x80u, "Show occluders for debugging");
    r_showAabbTrees = _Dvar_RegisterInt(
        "r_showAabbTrees",
        0,
        0,
        2,
        0x80u,
        "Show axis-aligned bounding box trees used in potentially visibility set determination");
    r_showSModelNames = _Dvar_RegisterFloat(
        "r_showSModelNames",
        -1.0,
        -1.0,
        3000.0,
        0x80u,
        "Show static model names within range");
    r_showDynEntModelNames = _Dvar_RegisterBool("r_showDynEntModelNames", 0, 0x80u, "Show dynamic entity model names");
    r_showDObjModelNamesDist = _Dvar_RegisterFloat(
        "r_showDObjModelNamesDist",
        -1.0,
        -1.0,
        3000.0,
        0x80u,
        "Show dobj model names within range");
    r_showEntModelNamesDist = _Dvar_RegisterFloat(
        "r_showEntModelNamesDist",
        -1.0,
        -1.0,
        3000.0,
        0x80u,
        "Show entity model names within range");
    r_showTess = _Dvar_RegisterEnum("r_showTess", r_showTessNames, 0, 0x80u, "Show details for each surface");
    r_showCullBModels = _Dvar_RegisterInt("r_showCullBModels", 0, 0, 3, 0x80u, "Show culled brush models");
    r_showCullSModels = _Dvar_RegisterInt("r_showCullSModels", 0, 0, 3, 0x80u, "Show culled static models");
    r_showCullXModels = _Dvar_RegisterInt("r_showCullXModels", 0, 0, 3, 0x80u, "Show culled xmodels");
    r_showCullXmodelsEntNum = _Dvar_RegisterInt(
        "r_showCullXmodelsEntNum",
        -1,
        -1,
        1024,
        0x80u,
        "Target r_showCullXModels to a specific entity");
    r_showCollision = _Dvar_RegisterEnum(
        "r_showCollision",
        showCollisionNames,
        0,
        0x80u,
        "Show the collision surfaces for the selected mask types");
    r_showCollisionGroups = _Dvar_RegisterEnum(
        "r_showCollisionGroups",
        showCollisionGroupsNames,
        0,
        0x80u,
        "Select whether to show the terrain, brush or all collision surface groups");
    r_showCollisionPolyType = _Dvar_RegisterEnum(
        "r_showCollisionPolyType",
        showCollisionPolyTypeNames,
        0,
        0x80u,
        "Select whether to display the collision surfaces as wireframe, poly interiors, or both");
    r_showCollisionDepthTest = _Dvar_RegisterBool(
        "r_showCollisionDepthTest",
        1,
        0x80u,
        "Select whether to use depth test in collision surfaces display");
    r_showCollisionDist = _Dvar_RegisterFloat(
        "r_showCollisionDist",
        500.0,
        1.0,
        3.4028235e38,
        0x80u,
        "Maximum distance to show collision surfaces");
    r_highLodDist = _Dvar_RegisterFloat(
        "r_highLodDist",
        -1.0,
        -1.0,
        3.4028235e38,
        0x80u,
        "Distance for high level of detail");
    r_mediumLodDist = _Dvar_RegisterFloat(
        "r_mediumLodDist",
        -1.0,
        -1.0,
        3.4028235e38,
        0x80u,
        "Distance for medium level of detail");
    r_lowLodDist = _Dvar_RegisterFloat(
        "r_lowLodDist",
        -1.0,
        -1.0,
        3.4028235e38,
        0x80u,
        "Distance for low level of detail");
    r_lowestLodDist = _Dvar_RegisterFloat(
        "r_lowestLodDist",
        -1.0,
        -1.0,
        3.4028235e38,
        0x80u,
        "Distance for lowest level of detail");
    r_forceLod = _Dvar_RegisterEnum("r_forceLod", r_forceLodNames, 4, 0x80u, "Force all level of detail to this level");
    r_modelVertColor = _Dvar_RegisterBool(
        "r_modelVertColor",
        1,
        0xA0u,
        "Set to 0 to replace all model vertex colors with white when loaded");
    sm_sunEnable = _Dvar_RegisterBool("sm_sunEnable", 1, 0x1000u, "Enable sun shadow mapping from script");
    sm_sunQuality = _Dvar_RegisterInt("sm_sunQuality", 0, 0, 2, 1u, "Sun shadow quality");
    sm_spotQuality = _Dvar_RegisterInt("sm_spotQuality", 0, 0, 2, 1u, "Spot shadow quality");
    sm_spotEnable = _Dvar_RegisterBool("sm_spotEnable", 1, 0x1000u, "Enable spot shadow mapping from script");
    sm_maxLights = _Dvar_RegisterInt("sm_maxLights", 4, 0, 4, 1u, "Limits how many primary lights can have shadow maps");
    sm_spotShadowFadeTime = _Dvar_RegisterFloat(
        "sm_spotShadowFadeTime",
        1.0,
        0.0099999998,
        5.0,
        0,
        "How many seconds it takes for a primary light shadow map to fade in or out");
    sm_showOverlay = _Dvar_RegisterEnum("sm_showOverlay", sm_showOverlayNames, 0, 0x80u, "Show shadow map overlay");
    sm_showOverlayDepthBounds = _Dvar_RegisterVec2(
        "sm_showOverlayDepthBounds",
        0.25,
        0.75,
        0.0,
        1.0,
        0x80u,
        "Near and far depth values for the shadow map overlay");
    sm_showSpotAxis = _Dvar_RegisterBool("sm_showSpotAxis", 0, 0x80u, "Show spot shadow origin and orientation");
    sm_sunAlwaysCastsShadow = _Dvar_RegisterBool(
        "sm_sunAlwaysCastsShadow",
        0,
        0x1080u,
        "forces sun to always do shadow mapping, from script");
    sm_polygonOffsetBias = _Dvar_RegisterInt(
        "sm_polygonOffsetBias",
        0x2000,
        0,
        0x10000,
        0x10A0u,
        "Shadow map offset bias");
    sm_sunSampleSizeNear = _Dvar_RegisterFloat("sm_sunSampleSizeNear", 0.5, 0.0625, 4.0, 0x5000u, "Shadow sample size");
    sm_sunShadowCenter = _Dvar_RegisterVec3(
        "sm_sunShadowCenter",
        0.0,
        0.0,
        0.0,
        -3.4028235e38,
        3.4028235e38,
        0x1080u,
        "Sun shadow center, 0 0 0 means don't override");
    sm_sunShadowScale = _Dvar_RegisterFloat("sm_sunShadowScale", 1.0, 0.25, 1.0, 0x1080u, "Sun shadow scale optimization");
    sm_sunShadowSmall = _Dvar_RegisterBool("sm_sunShadowSmall", 0, 0x1000u, "force quarter resolution sun shadow map");
    sm_sunShadowSmallEnable = _Dvar_RegisterBool(
        "sm_sunShadowSmallEnable",
        0,
        0x5000u,
        "use quarter resolution sun shadow map");
    r_flameScaler = _Dvar_RegisterFloat("r_flameScaler", 1.0, 0.0, 1.0, 0x1000u, "Flame Scaler");
    r_zombieDisableSlideEffect = _Dvar_RegisterBool("r_zombieDisableSlideEffect", 0, 0x5000u, "Disable Slide Effect");
    r_zombieDisableEarthEffect = _Dvar_RegisterBool("r_zombieDisableEarthEffect", 0, 0x5000u, "Disable Earth Effect");
    r_zombieNameAllowFriendsList = _Dvar_RegisterBool(
        "r_zombieNameAllowFriendsList",
        1,
        0x5000u,
        "Allow zombie name to be from friends list");
    r_zombieNameAllowDevList = _Dvar_RegisterBool(
        "r_zombieNameAllowDevList",
        1,
        0x5000u,
        "Allow zombie name to be from dev list");
    sm_spotShadowLargeRadiusScale = _Dvar_RegisterFloat(
        "sm_spotShadowLargeRadiusScale",
        1.0,
        0.0,
        5.0,
        0x1080u,
        "Radius scaler for large spot shadow switch over");
    sm_strictCull = _Dvar_RegisterBool("sm_strictCull", 0, 0x80u, "Strict shadow map cull");
    sm_fastSunShadow = _Dvar_RegisterBool("sm_fastSunShadow", 1, 0x80u, "Fast sun shadow");
    r_noipak = _Dvar_RegisterBool("r_noipak", 0, 0x10u, "do not load/stream images from ipak(pc only)");
    r_stereo3DAvailable = _Dvar_RegisterBool("r_stereo3DAvailable", 0, 1u, "3D Mode available");
    r_stereo3DMode = _Dvar_RegisterInt("r_stereo3DMode", R_Is3DOn(), 0, 1, 1u, "3D Rendering mode");
    r_stereo3DEyeSeparation = _Dvar_RegisterFloat(
        "r_stereo3DEyeSeparation",
        0.30000001,
        0.0,
        100.0,
        0x1000u,
        "3D eye separation");
    r_stereo3DEyeSeparationScaler = _Dvar_RegisterFloat(
        "r_stereo3DEyeSeparationScaler",
        1.0,
        0.0,
        1.0,
        0x1000u,
        "3D eye separation scaler (adjustment to separation used from scripts)");
    r_stereoFocusDepth = _Dvar_RegisterFloat("r_stereoFocusDepth", 20.0, 20.0, 70.0, 1u, "3D eye focus depth");
    r_stereoGunShift = _Dvar_RegisterFloat("r_stereoGunShift", 0.60000002, 0.0, 0.92000002, 1u, "3D gun shift");
    r_stereoTurretShift = _Dvar_RegisterFloat("r_stereoTurretShift", 0.0, 0.0, 10000.0, 0x1000u, "3D turret shift");
    r_stereo3DIPD = _Dvar_RegisterFloat("r_stereo3DIPD", 0.072649531, 0.0, 1.0, 0x1000u, "interpupillary distance");
    r_dualPlayEnable = _Dvar_RegisterBool("r_dualPlayEnable", 0, 1u, "DualPlay on/off toggle");
    r_dualPlayActive = _Dvar_RegisterBool("r_dualPlayActive", 0, 0x1000u, "Set ingame when Split Screen is active");
    r_dualPlayTestCount = _Dvar_RegisterBool(
        "r_dualPlayTestCount",
        1,
        0x20000u,
        "Enable testing of backend framebuffer double counting");
    r_blur = _Dvar_RegisterFloat("r_blur", 0.0, 0.0, 32.0, 0x80u, "Dev tweak to blur the screen");
    r_flame_allowed = _Dvar_RegisterBool("r_flame_allowed", 1, 1u, "Allow flame effect.");
    r_filmLut = _Dvar_RegisterFloat("r_filmLut", -1.0, -1.0, 9.0, 0, "Film LUT Index.");
    r_filmTweakLut = _Dvar_RegisterInt("r_filmTweakLut", -1, -1, 9, 0x1000u, "Tweak Film LUT Index.");
    r_forceCameraNode = _Dvar_RegisterInt("r_forceCameraNode", -1, -1, 3000, 0x80u, "Force camera to pathnode position.");
    r_forceCameraHdg = _Dvar_RegisterInt("r_forceCameraHdg", 0, 0, 3, 0x80u, "Force camera to heading.");
    r_forceCameraRecord = _Dvar_RegisterBool("r_forceCamerarecord", 0, 0x80u, "Turn on/off force camera record Mode.");
    r_enablePlayerShadow = _Dvar_RegisterBool(
        "r_enablePlayerShadow",
        0,
        (r_glob.isMultiplayer ? 0x80 : 0) | 0x5000,
        "Enable First Person Player Shadow.");
    if (r_glob.isMultiplayer)
        v9 = _Dvar_RegisterInt("r_enableFlashlight", 0, 0, 2, 0x5000u, "Enable Player Flashlight.");
    else
        v9 = _Dvar_RegisterInt("r_enableFlashlight", 0, 0, 2, 0x1000u, "Enable Player Flashlight.");
    r_enableFlashlight = v9;
    r_waterFogTest = _Dvar_RegisterBool("r_waterFogTest", 0, 0x80u, "Enable Water Fog Test.");
    r_contrast = _Dvar_RegisterFloat("r_contrast", 1.0, 0.0, 4.0, 0x80u, "Contrast adjustment");
    r_brightness = _Dvar_RegisterFloat("r_brightness", 0.0, 0.5, 1.5, 0x80u, "Brightness adjustment");
    r_desaturation = _Dvar_RegisterFloat("r_desaturation", 1.0, 0.0, 4.0, 0x80u, "Desaturation adjustment");
    r_filmTweakBrightness = _Dvar_RegisterFloat(
        "r_filmTweakBrightness",
        0.0,
        -1.0,
        1.0,
        0x1000u,
        "Tweak dev var; film color brightness");
    r_filmTweakDesaturation = _Dvar_RegisterFloat(
        "r_filmTweakDesaturation",
        0.2,
        0.0,
        1.0,
        0x1000u,
        "Tweak dev var; Desaturation applied after all 3D drawing");
    r_filmTweakInvert = _Dvar_RegisterBool("r_filmTweakInvert", 0, 0x1000u, "Tweak dev var; enable inverted video");
    r_filmUseTweaks = _Dvar_RegisterBool("r_filmUseTweaks", 0, 0x80u, "Overide film effects with tweak dvar values");
    r_filmTweakEnable = _Dvar_RegisterBool("r_filmTweakEnable", 0, 0x1000u, "Tweak dev var; enable film color effects");
    exp_softclip = _Dvar_RegisterFloat("exp_softclip", 0.25, 0.0, 1.0, 0x5000u, "soft clip value");
    vc_LIB = _Dvar_RegisterVec4("vc_LIB", 0.0, 0.0, 0.0, 0.0, 0.0, 32.0, 0x5000u, "Leonardo");
    vc_LIG = _Dvar_RegisterVec4("vc_LIG", 1.0, 1.0, 1.0, 1.0, 0.0099999998, 10.0, 0x5000u, "Leonardo");
    vc_LIW = _Dvar_RegisterVec4("vc_LIW", 32.0, 32.0, 32.0, 32.0, 0.0, 32.0, 0x5000u, "Leonardo");
    vc_LOB = _Dvar_RegisterVec4("vc_LOB", 0.0, 0.0, 0.0, 0.0, 0.0, 32.0, 0x5000u, "Leonardo");
    vc_LOW = _Dvar_RegisterVec4("vc_LOW", 32.0, 32.0, 32.0, 32.0, 0.0, 32.0, 0x5000u, "Leonardo");
    vc_RGBH = _Dvar_RegisterVec4("vc_RGBH", 0.0, 0.0, 0.0, 0.0, 0.0, 32.0, 0x5000u, "Leonardo");
    vc_RGBL = _Dvar_RegisterVec4("vc_RGBL", 0.0, 0.0, 0.0, 0.0, 0.0, 32.0, 0x5000u, "Leonardo");
    vc_YH = _Dvar_RegisterVec4("vc_YH", 0.0, 0.0, 0.0, 0.0, 0.0, 32.0, 0x5000u, "Leonardo");
    vc_YL = _Dvar_RegisterVec4("vc_YL", 0.0, 0.0, 0.0, 0.0, 0.0, 32.0, 0x5000u, "Leonardo");
    r_bloomTweaks = _Dvar_RegisterBool("r_bloomTweaks", 0, 0x5000u, "enbale bloom tweaks");
    r_bloomHiQuality = _Dvar_RegisterBool("r_bloomHiQuality", 0, 0x4001u, "bloom quality");
    r_dofHDR = _Dvar_RegisterInt("r_dofHDR", 0, 0, 2, 0x4001u, "dof mode, 0 = legacy 1 = hdr low 2 = hdr hi");
    r_zombieDarkness = _Dvar_RegisterBool("r_zombieDarkness", 0, 0x5000u, "zombie darkness postfx");
    vc_RS = _Dvar_RegisterVec4("vc_RS", 0.0, 0.5, 0.0, 0.75, 0.0, 1.0, 0x5000u, "Leonardo");
    vc_RE = _Dvar_RegisterVec4("vc_RE", 0.5, 1.0, 0.25, 1.0, 0.0, 1.0, 0x5000u, "Leonardo");
    vc_SMR = _Dvar_RegisterVec4("vc_SMR", 1.0, 0.0, 0.0, 0.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_SMG = _Dvar_RegisterVec4("vc_SMG", 0.0, 1.0, 0.0, 0.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_SMB = _Dvar_RegisterVec4("vc_SMB", 0.0, 0.0, 1.0, 0.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_MMR = _Dvar_RegisterVec4("vc_MMR", 1.0, 0.0, 0.0, 0.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_MMG = _Dvar_RegisterVec4("vc_MMG", 0.0, 1.0, 0.0, 0.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_MMB = _Dvar_RegisterVec4("vc_MMB", 0.0, 0.0, 1.0, 0.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_HMR = _Dvar_RegisterVec4("vc_HMR", 1.0, 0.0, 0.0, 0.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_HMG = _Dvar_RegisterVec4("vc_HMG", 0.0, 1.0, 0.0, 0.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_HMB = _Dvar_RegisterVec4("vc_HMB", 0.0, 0.0, 1.0, 0.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_FGM = _Dvar_RegisterVec4("vc_FGM", 1.0, 1.0, 1.0, 1.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_FSM = _Dvar_RegisterVec4("vc_FSM", 0.0, 0.0, 0.0, 1.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_FBM = _Dvar_RegisterVec4("vc_FBM", 0.0, 0.0, 0.0, 0.0, -100.0, 100.0, 0x5000u, "Leonardo");
    vc_LUT = _Dvar_RegisterInt("vc_LUT", 0, -32, 32, 0x5000u, "Lut index");
    vc_SNAP = _Dvar_RegisterInt("vc_SNAP", 0, 0, 32, 0x5000u, "hdr snapshot");
    r_sunFlareTint = _Dvar_RegisterVec4(
        "r_sunFlareTint",
        0.0,
        0.0,
        1.0,
        1.0,
        0.0,
        1.0,
        0x1000u,
        "Sun Flare Sprite Color Tint");
    r_superFlareDraw = _Dvar_RegisterBool("r_superFlareDraw", 1, 0x5000u, "toggle superflare rendering");
    r_waterWaveAngle = _Dvar_RegisterVec4("r_waterWaveAngle", 0.0, 0.0, 0.0, 0.0, 0.0, 360.0, 0x5000u, "water wave angle");
    r_waterWaveWavelength = _Dvar_RegisterVec4(
        "r_waterWaveWavelength",
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1024.0,
        0x5000u,
        "water wave wavelength");
    r_waterWaveAmplitude = _Dvar_RegisterVec4(
        "r_waterWaveAmplitude",
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        256.0,
        0x5000u,
        "water wave amplitude");
    r_waterWavePhase = _Dvar_RegisterVec4(
        "r_waterWavePhase",
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        6.283185,
        0x5000u,
        "water wave phase");
    r_waterWaveSteepness = _Dvar_RegisterVec4(
        "r_waterWaveSteepness",
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        1.0,
        0x5000u,
        "water wave steepness");
    r_waterWaveSpeed = _Dvar_RegisterVec4("r_waterWaveSpeed", 1.0, 1.0, 1.0, 1.0, 0.0, 2.0, 0x5000u, "water wave speed");
    r_waterWaveBase = _Dvar_RegisterFloat("r_waterWaveBase", 0.0, -48.0, 48.0, 0x5000u, "water base height");
    r_waterWaveNormalScale = _Dvar_RegisterFloat("r_waterWaveNormalScale", 1.0, 0.25, 4.0, 0x5000u, "water normal scale");
    r_waterWaveScriptShader0 = _Dvar_RegisterVec4(
        "r_waterWaveScriptShader0",
        0.0,
        0.0,
        0.0,
        0.0,
        -3.4028235e38,
        3.4028235e38,
        0x5000u,
        "water shader parameters mapped to shader constant 'motionblurDirectionAndMagnitude'");
    r_waterWaveScriptShader1 = _Dvar_RegisterVec4(
        "r_waterWaveScriptShader1",
        0.0,
        0.0,
        0.0,
        0.0,
        -3.4028235e38,
        3.4028235e38,
        0x5000u,
        "water shader parameters mapped to shader constant 'bloomScale'");
    r_dof_enable = _Dvar_RegisterBool("r_dof_enable", 1, 0, "Enable the depth of field effect");
    r_dof_tweak = _Dvar_RegisterBool(
        "r_dof_tweak",
        0,
        0x4080u,
        "Use dvars to set the depth of field effect; overrides r_dof_enable");
    r_dof_nearBlur = _Dvar_RegisterFloat(
        "r_dof_nearBlur",
        6.0,
        4.0,
        10.0,
        0x4080u,
        "Sets the radius of the gaussian blur used by depth of field, in pixels at 640x480");
    r_dof_farBlur = _Dvar_RegisterFloat(
        "r_dof_farBlur",
        1.8,
        0.0,
        10.0,
        0x4080u,
        "Sets the radius of the gaussian blur used by depth of field, in pixels at 640x480");
    r_dof_viewModelStart = _Dvar_RegisterFloat(
        "r_dof_viewModelStart",
        2.0,
        0.0,
        128.0,
        0x4080u,
        "Depth of field viewmodel start distance, in inches");
    r_dof_viewModelEnd = _Dvar_RegisterFloat(
        "r_dof_viewModelEnd",
        8.0,
        0.0,
        128.0,
        0x4080u,
        "Depth of field viewmodel end distance, in inches");
    r_dof_nearStart = _Dvar_RegisterFloat(
        "r_dof_nearStart",
        10.0,
        0.0,
        1000.0,
        0x4080u,
        "Depth of field near start distance, in inches");
    r_dof_nearEnd = _Dvar_RegisterFloat(
        "r_dof_nearEnd",
        60.0,
        0.0,
        1000.0,
        0x4080u,
        "Depth of field near end distance, in inches");
    r_dof_farStart = _Dvar_RegisterFloat(
        "r_dof_farStart",
        1000.0,
        0.0,
        20000.0,
        0x4080u,
        "Depth of field far start distance, in inches");
    r_dof_farEnd = _Dvar_RegisterFloat(
        "r_dof_farEnd",
        7000.0,
        0.0,
        20000.0,
        0x4080u,
        "Depth of field far end distance, in inches");
    r_dof_bias = _Dvar_RegisterFloat(
        "r_dof_bias",
        0.5,
        0.1,
        3.0,
        0x4080u,
        "Depth of field bias as a power function (like gamma); less than 1 is sharper");
    r_dof_showdebug = _Dvar_RegisterBool("r_dof_showdebug", 0, 0x80u, "displays DOF parameter debug");
    r_flameFX_distortionScaleFactor = _Dvar_RegisterVec4(
        "r_flameFX_distortionScaleFactor",
        0.0,
        1.0,
        1.0,
        0.51191801,
        0.0,
        1.0,
        0x1081u,
        "Distortion uv scales (Default to 1)");
    r_flameFX_magnitude = _Dvar_RegisterFloat(
        "r_flameFX_magnitude",
        0.021514701,
        0.0,
        1.0,
        0x1081u,
        "Distortion magnitude");
    r_flameFX_FPS = _Dvar_RegisterInt("r_flameFX_FPS", 15, 1, 60, 0x1081u, "fire frames per sec");
    r_flameFX_fadeDuration = _Dvar_RegisterFloat(
        "r_flameFX_fadeDuration",
        0.5,
        0.0,
        15.0,
        0x1001u,
        "Sets fade duration in seconds");
    r_waterSheetingFX_allowed = _Dvar_RegisterBool("r_waterSheetingFX_allowed", 1, 1u, "Enable the water sheeting effect");
    r_waterSheetingFX_enable = _Dvar_RegisterBool("r_waterSheetingFX_enable", 0, 0, "Enable the water sheeting effect");
    r_waterSheetingFX_distortionScaleFactor = _Dvar_RegisterVec4(
        "r_waterSheetingFX_distortionScaleFactor",
        0.021961,
        1.0,
        0.0,
        0.0,
        0.0,
        1.0,
        0,
        "Distortion uv scales (Default to 1)");
    r_waterSheetingFX_magnitude = _Dvar_RegisterFloat(
        "r_waterSheetingFX_magnitude",
        0.065538801,
        0.0,
        1.0,
        0,
        "Distortion magnitude");
    r_waterSheetingFX_radius = _Dvar_RegisterFloat(
        "r_waterSheetingFX_radius",
        4.4405098,
        0.0,
        32.0,
        0,
        "Tweak dev var; Glow radius in pixels at 640x480");
    r_anaglyphFX_enable = _Dvar_RegisterBool("r_anaglyphFX_enable", 0, 1u, "Enable red/green Anaglyph 3DS");
    r_rimIntensity_debug = _Dvar_RegisterBool("r_rimIntensity_debug", 0, 0x5000u, "Enable the rim intensity debug");
    r_rimIntensity = _Dvar_RegisterFloat(
        "r_rimIntensity",
        0.0,
        0.0,
        50.0,
        0x5000u,
        "Rim intensity for character skin/cloth");
    r_genericSceneVector_debug = _Dvar_RegisterBool(
        "r_genericSceneVector_debug",
        0,
        0x5000u,
        "Enable the generic scene vector debug");
    r_genericSceneVector0 = _Dvar_RegisterVec4(
        "r_genericSceneVector0",
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        100.0,
        0x5000u,
        "Generic Scene Vector 0");
    r_genericSceneVector1 = _Dvar_RegisterVec4(
        "r_genericSceneVector1",
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        100.0,
        0x5000u,
        "Generic Scene Vector 1");
    r_genericSceneVector2 = _Dvar_RegisterVec4(
        "r_genericSceneVector2",
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        100.0,
        0x5000u,
        "Generic Scene Vector 2");
    r_genericSceneVector3 = _Dvar_RegisterVec4(
        "r_genericSceneVector3",
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        100.0,
        0x5000u,
        "Generic Scene Vector 3");
    r_reviveFX_debug = _Dvar_RegisterBool("r_reviveFX_debug", 0, 0x80u, "Enable the revive effect debug");
    r_reviveFX_edgeColorTemp = _Dvar_RegisterFloat(
        "r_reviveFX_edgeColorTemp",
        6500.0,
        1650.0,
        25000.0,
        0x5000u,
        "edge color temp");
    r_reviveFX_edgeSaturation = _Dvar_RegisterFloat(
        "r_reviveFX_edgeSaturation",
        1.0,
        0.0,
        2.0,
        0x5000u,
        "edge saturation");
    r_reviveFX_edgeScale = _Dvar_RegisterVec3(
        "r_reviveFX_edgeScale",
        1.0,
        1.0,
        1.0,
        0.0,
        2.0,
        0x5000u,
        "edge scale (tint) adjust RGB");
    r_reviveFX_edgeContrast = _Dvar_RegisterVec3(
        "r_reviveFX_edgeContrast",
        1.0,
        1.0,
        1.0,
        0.0,
        2.0,
        0x5000u,
        "edge contrast adjust RGB");
    r_reviveFX_edgeOffset = _Dvar_RegisterVec3(
        "r_reviveFX_edgeOffset",
        0.0,
        0.0,
        0.0,
        -2.0,
        2.0,
        0x5000u,
        "edge tv style brightness adjust RGB");
    r_reviveFX_edgeAmount = _Dvar_RegisterFloat("r_reviveFX_edgeAmount", 0.0, 0.0, 1.0, 0x5000u, "edge effect amount");
    r_reviveFX_edgeMaskAdjust = _Dvar_RegisterFloat(
        "r_reviveFX_edgeMaskAdjust",
        0.0,
        -1.0,
        1.0,
        0x5000u,
        "edge mask expansion contraction");
    r_poisonFX_debug_enable = _Dvar_RegisterBool("r_poisonFX_debug_enable", 0, 0x5000u, "Enable the poison effect");
    r_poisonFX_pulse = _Dvar_RegisterFloat("r_poisonFX_pulse", 1.1, 1.0, 2.0, 0x4000u, "pulse rate for distortion");
    r_poisonFX_dvisionX = _Dvar_RegisterFloat(
        "r_poisonFX_dvisionX",
        0.0,
        -20.0,
        20.0,
        0x4000u,
        "amount of double vision x");
    r_poisonFX_blurMin = _Dvar_RegisterFloat("r_poisonFX_blurMin", 0.0, 0.0, 20.0, 0x5000u, "blur min");
    r_poisonFX_blurMax = _Dvar_RegisterFloat(
        "r_poisonFX_blurMax",
        4.0,
        0.0,
        20.0,
        (r_glob.isMultiplayer ? 0x80 : 0) | 0x5000,
        "blur max");
    if (r_glob.isMultiplayer)
    {
        Dvar_SetFloat(r_poisonFX_pulse, 2.0);
        Dvar_SetFloat(r_poisonFX_dvisionX, 20.0);
        Dvar_SetFloat(r_poisonFX_blurMin, 1.5);
    }
    r_zbufferDebug = _Dvar_RegisterBool("r_zbufferDebug", 0, 0x80u, "debug zbuffer display");
    r_zbufferDebugRange = _Dvar_RegisterFloat(
        "r_zbufferDebugRange",
        10000.0,
        1.0,
        15000.0,
        0x80u,
        "debug zbufffer max display range");
    r_fogTweak = _Dvar_RegisterBool("r_fogTweak", 0, 0x80u, "enable dvar tweaks");
    r_fogBaseDist = _Dvar_RegisterFloat("r_fogBaseDist", 0.0, 0.0, 20000.0, 0x80u, "start distance");
    r_fogHalfDist = _Dvar_RegisterFloat("r_fogHalfDist", 1.0, 1.0, 40000.0, 0x80u, "distance at which fog is 50%");
    r_fogBaseHeight = _Dvar_RegisterFloat("r_fogBaseHeight", 0.0, -50000.0, 50000.0, 0x80u, "start height");
    r_fogHalfHeight = _Dvar_RegisterFloat("r_fogHalfHeight", 1.0, 1.0, 50000.0, 0x80u, "height at which fog is 50%");
    r_fogColor = _Dvar_RegisterVec3("r_fogColor", 0.5, 0.5, 0.5, 0.0, 32.0, 0x80u, "color");
    r_fogOpacity = _Dvar_RegisterFloat("r_fogOpacity", 0.25, 0.0, 1.0, 0x80u, "opacity");
    r_fogSunColor = _Dvar_RegisterVec3("r_fogSunColor", 1.0, 1.0, 1.0, 0.0, 32.0, 0x80u, "sun color");
    r_fogSunOpacity = _Dvar_RegisterFloat("r_fogSunOpacity", 0.5, 0.0, 1.0, 0x80u, "sun opacity");
    r_fogSunPitch = _Dvar_RegisterFloat("r_fogSunPitch", -45.0, -360.0, 360.0, 0x80u, "sun pitch");
    r_fogSunYaw = _Dvar_RegisterFloat("r_fogSunYaw", 0.0, -360.0, 360.0, 0x80u, "sun yaw");
    r_fogSunInner = _Dvar_RegisterFloat("r_fogSunInner", 0.0, 0.0, 180.0, 0x80u, "sun angle start");
    r_fogSunOuter = _Dvar_RegisterFloat("r_fogSunOuter", 0.0, 0.0, 180.0, 0x80u, "sun angle end");
    r_exposureTweak = _Dvar_RegisterBool("r_exposureTweak", 0, 0x5000u, "enable the exposure dvar tweak");
    r_exposureValue = _Dvar_RegisterFloat("r_exposureValue", 3.0, -6.0, 16.0, 0x5000u, "exposure ev stops");
    r_finalShiftX = _Dvar_RegisterVec3("r_finalShiftX", 0.0, 0.0, 0.0, -5.0, 5.0, 0x5000u, "r,g,b channel shift x");
    r_finalShiftY = _Dvar_RegisterVec3("r_finalShiftY", 0.0, 0.0, 0.0, -5.0, 5.0, 0x5000u, "r,g,b channel shift y");
    r_primaryLightUseTweaks = _Dvar_RegisterBool(
        "r_primaryLightUseTweaks",
        0,
        0x80u,
        "Use primary light intensity tweaks");
    r_primaryLightTweakDiffuseStrength = _Dvar_RegisterFloat(
        "r_primaryLightTweakDiffuseStrength",
        1.0,
        0.0,
        10.0,
        0x80u,
        "Tweak the diffuse intensity for primary lights");
    r_primaryLightTweakSpecularStrength = _Dvar_RegisterFloat(
        "r_primaryLightTweakSpecularStrength",
        1.0,
        0.0,
        10.0,
        0x80u,
        "Tweak the specular intensity for primary lights");
    r_lightGridEnableTweaks = _Dvar_RegisterBool(
        "r_lightGridEnableTweaks",
        0,
        0x1000u,
        "Enable tweaks of the light color from the light grid");
    r_lightGridIntensity = _Dvar_RegisterFloat(
        "r_lightGridIntensity",
        1.0,
        0.0,
        2.0,
        0x1000u,
        "Adjust the intensity of light color from the light grid");
    r_lightGridContrast = _Dvar_RegisterFloat(
        "r_lightGridContrast",
        0.0,
        -1.0,
        1.0,
        0x1000u,
        "Adjust the contrast of light color from the light grid");
    r_heroLighting = _Dvar_RegisterBool("r_heroLighting", 1, 0, "Enable hero-only lighting");
    r_heroLightScale = _Dvar_RegisterVec3(
        "r_heroLightScale",
        1.0,
        1.0,
        1.0,
        0.0,
        2.0,
        (r_glob.isMultiplayer ? 0x80 : 0) | 0x5000,
        "hero diffuse light scale");
    r_num_viewports = _Dvar_RegisterInt("r_num_viewports", 0, 0, 4, 0, "number of viewports to expect to render");
    r_pix_material = _Dvar_RegisterBool("r_pix_material", 1, 0, "Enable pix material names");
    r_outdoorAwayBias = _Dvar_RegisterFloat(
        "r_outdoorAwayBias",
        32.0,
        -3.4028235e38,
        3.4028235e38,
        0x1000u,
        "Affects the height map lookup for making sure snow doesn't go indoors");
    r_outdoorDownBias = _Dvar_RegisterFloat(
        "r_outdoorDownBias",
        0.0,
        -3.4028235e38,
        3.4028235e38,
        0x1000u,
        "Affects the height map lookup for making sure snow doesn't go indoors");
    r_outdoorFeather = _Dvar_RegisterFloat(
        "r_outdoorFeather",
        8.0,
        -3.4028235e38,
        3.4028235e38,
        0x1000u,
        "Outdoor z-feathering value");
    Dvar_SetModified((dvar_t*)r_outdoorFeather);
    r_sun_from_dvars = _Dvar_RegisterBool(
        "r_sun_from_dvars",
        0,
        0x80u,
        "Set sun flare values from dvars rather than the level");
    developer = _Dvar_RegisterInt("developer", 0, 0, 2, 0, "Enable development environment");
    sv_cheats = _Dvar_RegisterBool("sv_cheats", 1, 0x48u, "Allow server side cheats");
    r_defaultMode = _Dvar_RegisterString("r_defaultMode", nullptr, 0, "Default resolution mode");
    r_monitor = _Dvar_RegisterInt(
        "r_monitor",
        0,
        0,
        8,
        0x21u,
        "Index of the monitor to use in a multi monitor system; 0 picks automatically.");
    r_currentMonitor = _Dvar_RegisterInt("r_currentMonitor", 0, 0, 8, 0, "Index of the current full screen monitor.");
    r_monitorCount = _Dvar_RegisterInt("r_monitorCount", 0, 0, 8, 0, "Number of monitors connected.");
    r_aspectRatio = _Dvar_RegisterEnum(
        "r_aspectRatio",
        s_aspectRatioNames,
        0,
        0x21u,
        "Screen aspect ratio.  Most widescreen monitors are 16:10 instead of 16:9.");
    r_aspectRatioWindow = _Dvar_RegisterFloat("r_aspectRatioWindow", 1.7777778, 0.0, 10.0, 0, "Screen aspect ratio.");
    r_customMode = _Dvar_RegisterString(
        "r_customMode",
        nullptr,
        0x21u,
        "Special resolution mode for the remote debugger");
    r_open_automate = _Dvar_RegisterBool("r_open_automate", 0, 0, "Enable Nvidia Open Automate testing");
    r_superflare_drawlist = _Dvar_RegisterEnum(
        "r_superflare_drawlist",
        superflareDrawlistModesNames,
        2,
        0,
        "supreflare drawlist selection.");
    R_WarnInitDvars();
    r_staticModelDumpLodInfo = _Dvar_RegisterBool(
        "r_staticModelDumpLodInfo",
        0,
        0,
        "Dump static model info for the next frame.");
    r_grassEnable = _Dvar_RegisterBool("r_grassEnable", 1, 0, "Diables grass movement");
    r_skyTransition = _Dvar_RegisterFloat("r_skyTransition", 0.0, 0.0, 1.0, 0x1000u, "Sky transition blend factor.");
    r_skyRotation = _Dvar_RegisterFloat("r_skyRotation", 0.0, -360.0, 360.0, 0x1000u, "Sky rotation angle.");
    r_shaderDebugA = _Dvar_RegisterInt("r_shaderDebugA", 0, 0, 1, 0x1000u, "Shader Debug A");
    r_shaderDebugB = _Dvar_RegisterInt("r_shaderDebugB", 0, 0, 1, 0x1000u, "Shader Debug B");
    r_shaderDebugC = _Dvar_RegisterFloat("r_shaderDebugC", 0.0, 0.0, 1.0, 0x1000u, "Shader Debug C");
    r_enableCubicUpsample = _Dvar_RegisterBool("r_enableCubicUpsample", 1, 0x1000u, "Enable Cubic Upsample");
    r_cubicUpsampleParam = _Dvar_RegisterFloat(
        "r_cubicUpsampleParam",
        -0.5,
        -1.0,
        1.0,
        0x1000u,
        "Cubic Upsample Parameter");
    r_skyColorTemp = _Dvar_RegisterFloat("r_skyColorTemp", 6500.0, 1650.0, 25000.0, 0x1000u, "Sky box color temp.");
    r_blinkLitQuasiOpaque = _Dvar_RegisterInt(
        "r_blinkLitQuasiOpaque",
        0,
        0,
        2,
        0x80u,
        "Blink Lit Transparencies with Depth write (Foliage Blend etc).");
    r_blinkTrans = _Dvar_RegisterInt("r_blinkTrans", 0, 0, 2, 0x80u, "Blink All Transparencies.");
    r_blinkLayers = _Dvar_RegisterInt("r_blinkLayers", 0, 0, 4, 0x80u, "Blink materials with specific number of layers.");
    r_blinkCullNone = _Dvar_RegisterInt("r_blinkCullNone", 0, 0, 2, 0x80u, "Blink materials with cull none.");
    r_modelSkelWorker = _Dvar_RegisterBool("r_modelSkelWorker", 1, 0x80u, "Enable model skel worker.");
    r_fullPrepass = _Dvar_RegisterBool("r_fullPrepass", 0, 0x80u, "Enable full prepass.");
    r_lightmapOnlyBspSurfs = _Dvar_RegisterBool(
        "r_lightmapOnlyBspSurfs",
        1,
        0x80u,
        "Enable lightmap_only_surfs tech for BSP draw surfaces");
    r_lightmapOnlySModelSurfs = _Dvar_RegisterBool(
        "r_lightmapOnlySModelSurfs",
        1,
        0x80u,
        "Enable lightmap_only_surfs tech for static model draw surfaces");
    r_lightmapOnlyEntSurfs = _Dvar_RegisterBool(
        "r_lightmapOnlyEntSurfs",
        1,
        0x80u,
        "Enable lightmap_only_surfs tech for scene ent draw surfaces");
    r_sortDrawSurfsBsp = _Dvar_RegisterBool("r_sortDrawSurfsBsp", 0, 0x80u, "Sort BSP draw surfaces.");
    r_sortDrawSurfsStaticModel = _Dvar_RegisterBool(
        "r_sortDrawSurfsStaticModel",
        1,
        0x80u,
        "Sort static model draw surfaces.");
    r_sortBackToFront = _Dvar_RegisterBool("r_sortBackToFront", 1, 0x80u, "Sort transparent models back to front.");
    r_swrk_override_characterCharredAmount = _Dvar_RegisterFloat(
        "r_swrk_override_characterCharredAmount",
        0.0,
        0.0,
        1.0,
        0x80u,
        "overrides the __characterCharredAmount Shaderworks tweak");
    r_swrk_override_characterDissolveColor = _Dvar_RegisterVec4(
        "r_swrk_override_characterDissolveColor",
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        10.0,
        0x80u,
        "overrides the __characterDissolveColor Shaderworks tweak");
    r_swrk_override_wetness = _Dvar_RegisterFloat(
        "r_swrk_override_wetness",
        0.0,
        0.0,
        1.0,
        0x80u,
        "Overrides the character wetness.");
    r_debugShowPrimaryLights = _Dvar_RegisterBool(
        "r_debugShowPrimaryLights",
        0,
        0x80u,
        "shows location of primary lights");
    r_debugShowDynamicLights = _Dvar_RegisterBool(
        "r_debugShowDynamicLights",
        0,
        0x80u,
        "shows location of dynamic lights");
    r_debugShowCoronas = _Dvar_RegisterBool("r_debugShowCoronas", 0, 0x80u, "shows location of coronas");
    r_debugMetalStorm = _Dvar_RegisterBool("r_debugMetalStorm", 0, 0x80u, "shows extra MetalStorm info");
    r_disableGenericFilter = _Dvar_RegisterBool("r_disableGenericFilter", 0, 0x80u, "Stops the Generic Filters running");
    r_drawDebugFogParams = _Dvar_RegisterBool("r_drawDebugFogParams", 0, 0x80u, "enables the fog debug display");
    r_debugShowPrimaryLightLines = _Dvar_RegisterEnum(
        "r_debugShowPrimaryLightLines",
        showPrimaryLightLinesNames,
        0,
        0x80u,
        "Show primary light selection");
    r_seethru_decal_enable = _Dvar_RegisterBool(
        "r_seethru_decal_enable",
        !r_glob.isMultiplayer,
        0x1080u,
        "Toggles see-through impact holes");
    Dvar_SetBool(r_seethru_decal_enable, !r_glob.isMultiplayer);
    r_tension_enable = _Dvar_RegisterBool("r_tension_enable", 1, 0x1080u, "Toggles tension");
    _Dvar_RegisterVec4(
        "r_MaterialParameterTweak",
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        1.0,
        0x280u,
        "Modifies currently selected material parameter");
    r_ui3d_debug_display = _Dvar_RegisterBool("r_ui3d_debug_display", 0, 0x80u, "Show UI3D debug overlay");
    r_ui3d_use_debug_values = _Dvar_RegisterInt("r_ui3d_use_debug_values", -1, -1, 8, 0x80u, "Use UI debug values");
    r_ui3d_x = _Dvar_RegisterFloat("r_ui3d_x", 0.0, 0.0, 1.0, 0x80u, "ui3d texture window x");
    r_ui3d_y = _Dvar_RegisterFloat("r_ui3d_y", 0.0, 0.0, 1.0, 0x80u, "ui3d texture window y");
    r_ui3d_w = _Dvar_RegisterFloat("r_ui3d_w", 1.0, 0.0, 1.0, 0x80u, "ui3d texture window width");
    r_lightTweakSunDirection = _Dvar_RegisterFloat(
        "r_ui3d_h",
        1.0,
        0.0,
        1.0,
        0x80u,
        "ui3d texture window height");
    r_multi_extracam_debug = _Dvar_RegisterInt(
        "r_multi_extracam_debug",
        -1,
        -1,
        10,
        0x80u,
        "-1 is disable, >0 is index of current active multi-extracam, when r_missile_cam_debug_display==3 mode");
    r_missile_cam_debug_display = _Dvar_RegisterInt(
        "r_missile_cam_debug_display",
        0,
        0,
        10,
        0x80u,
        "Show missile camera debug overlay");
    r_extracam_custom_aspectratio = _Dvar_RegisterFloat(
        "r_extracam_custom_aspectratio",
        -1.0,
        -1.0,
        5.0,
        0x1000u,
        "custom extracam aspect ratio (width/height). Set to -1 to use default aspect ratio");
    r_extracam_lod_scale = _Dvar_RegisterFloat(
        "r_extracam_lod_scale",
        2.0,
        0.1,
        100.0,
        0x1000u,
        "Scales an objects distance from the extracam for LOD calculation");
    r_qrcode_debug_display = _Dvar_RegisterInt("r_qrcode_debug_display", -1, -1, 12, 0x80u, nullptr);
    r_qrcode_debug_display_size = _Dvar_RegisterFloat(
        "r_qrcode_debug_display_size",
        400.0,
        0.0,
        2000.0,
        0x80u,
        nullptr);
    r_shader_constant_set_debug_range = _Dvar_RegisterFloat(
        "r_shader_constant_set_debug_range",
        -1.0,
        -2.0,
        2000.0,
        0x80u,
        "How far to draw scs debug view");
    r_shader_constant_set_enable = _Dvar_RegisterBool(
        "r_shader_constant_set_enable",
        1,
        0x80u,
        "enables shader constant sets");
    r_sky_intensity_showDebugDisplay = _Dvar_RegisterBool(
        "r_sky_intensity_showDebugDisplay",
        0,
        0x80u,
        "Enables sky intensity debugging display");
    r_sky_intensity_angle0 = _Dvar_RegisterFloat(
        "r_sky_intensity_angle0",
        90.0,
        -90.0,
        90.0,
        0x1000u,
        "angle0 of sky intensity");
    r_sky_intensity_angle1 = _Dvar_RegisterFloat(
        "r_sky_intensity_angle1",
        90.0,
        -90.0,
        90.0,
        0x1000u,
        "angle1 of sky intensity");
    r_sky_intensity_factor0 = _Dvar_RegisterFloat(
        "r_sky_intensity_factor0",
        1.0,
        0.0,
        10.0,
        0x1000u,
        "angle0 of sky intensity");
    r_sky_intensity_factor1 = _Dvar_RegisterFloat(
        "r_sky_intensity_factor1",
        1.0,
        0.0,
        10.0,
        0x1000u,
        "angle1 of sky intensity");
    r_fog_disable = _Dvar_RegisterBool("r_fog_disable", 0, 0x80u, "Disables fog");
    r_grassWindForceEnable = _Dvar_RegisterBool(
        "r_grassWindForceEnable",
        1,
        0x80u,
        "Enables wind gusts that affect dynFoliage grass");
    r_dpvs_useCellForceInvisibleBits = _Dvar_RegisterBool(
        "r_dpvs_useCellForceInvisibleBits",
        1,
        0x80u,
        "turns on/off the script-driven cell vis bits");
    r_remotescreen_renderlastframe = _Dvar_RegisterBool(
        "r_remotescreen_renderlastframe",
        1,
        0x80u,
        "toggles if remote screen update draws the last frame");
    r_vc_compile = _Dvar_RegisterInt("r_vc_compile", 0, 0, 0x7FFFFFFF, 0, nullptr);
    r_occQueryDebug = _Dvar_RegisterBool("r_occQueryDebug", 0, 0x80u, "draws occlusion query testing polygons");
    r_useHidePartbits = _Dvar_RegisterBool("r_useHidePartbits", 1, 0x80u, "toggles hide partbits usage");
    r_cameraInfo = _Dvar_RegisterBool("r_cameraInfo", 0, 0x80u, "toggles exposure focal length display");
    r_shaderbinddebug = _Dvar_RegisterBool(
        "r_shaderbinddebug",
        0,
        0x80u,
        "toggles debug print of vertex/pixelshader binding");
}

bool R_CheckDvarModified(const dvar_t* dvar)
{
    if (Dvar_GetModified(dvar))
    {
        Dvar_ClearModified((dvar_t*)dvar);
        return 1;
    }
    return 0;
}

float R_GetDefaultNearClip()
{
    dvarType_t dvarType; // ecx
    long double defaultNearClip; // st7

    if (r_znear)
    {
        dvarType = r_znear->type;
        if (dvarType != 2 && dvarType != 6)
            defaultNearClip = atof(r_znear->current.string);
    }
    return defaultNearClip;
}

float R_GetDefaultNearClip_DepthHack()
{
    float value;
    value = Dvar_GetFloat(r_znear_depthhack);
    if (0.1 - value >= 0.0)
        return 0.1;
    return value;
}

void DvarBlock_SetFog()
{
    if (gDvarDataReady == DBLOCK_READY_FOG)
    {
        Dvar_SetFloat(r_fogBaseDist, dvar_buffer.v[0]);
        Dvar_SetFloat(r_fogHalfDist, dvar_buffer.v[1]);
        Dvar_SetFloat(r_fogBaseHeight, dvar_buffer.v[2]);
        Dvar_SetFloat(r_fogHalfHeight, dvar_buffer.v[3]);
        Dvar_SetVec3(r_fogColor, vecin.v[0], vecin.v[1], vecin.v[2]);
        Dvar_SetFloat(r_fogOpacity, vecin.v[3]);
        Dvar_SetVec3(r_fogSunColor, stru_83BAA78.v[0], stru_83BAA78.v[1], stru_83BAA78.v[2]);
        Dvar_SetFloat(r_fogSunOpacity, stru_83BAA78.v[3]);
        Dvar_SetFloat(r_fogSunPitch, stru_83BAA88.v[0]);
        Dvar_SetFloat(r_fogSunYaw, stru_83BAA88.v[1]);
        Dvar_SetFloat(r_fogSunInner, stru_83BAA88.v[2]);
        Dvar_SetFloat(r_fogSunOuter, stru_83BAA88.v[3]);
        gDvarDataReady = DBLOCK_EMPTY;
    }
}

void DvarBlock_SetWSI()
{
    if (gDvarDataReady == DBLOCK_READY_WSI)
    {
        if (dvar_buffer.v[0] == 0.0)
            Dvar_SetBool(r_exposureTweak, 0);
        else
            Dvar_SetBool(r_exposureTweak, 1);
        Dvar_SetFloat(r_exposureValue, dvar_buffer.v[1]);
        Dvar_SetVec3(r_lightTweakSunDirection, dvar_buffer.v[2], dvar_buffer.v[3], 0.0);
        gDvarDataReady = DBLOCK_EMPTY;
    }
}

void DvarBlock_SetVcBloom()
{
    if (gDvarDataReady == DBLOCK_READY_BLOOM)
    {
        Dvar_SetVec4FromVec4(vc_LIB, &dvar_buffer);
        Dvar_SetVec4FromVec4(vc_LIG, &vecin);
        Dvar_SetVec4FromVec4(vc_LIW, &stru_83BAA78);
        Dvar_SetVec4FromVec4(vc_LOB, &stru_83BAA88);
        Dvar_SetVec4FromVec4(vc_LOW, &stru_83BAA98);
        Dvar_SetVec4FromVec4(vc_RGBH, &stru_83BAAA8);
        Dvar_SetVec4FromVec4(vc_RGBL, &stru_83BAAB8);
        Dvar_SetVec4FromVec4(vc_YH, &stru_83BAAC8);
        Dvar_SetVec4FromVec4(vc_YL, &stru_83BAAD8);
        gDvarDataReady = DBLOCK_EMPTY;
    }
}

void DvarBlock_SetVcColor()
{
    if (gDvarDataReady == DBLOCK_READY_COLOR)
    {
        Dvar_SetVec4FromVec4(vc_RS, &dvar_buffer);
        Dvar_SetVec4FromVec4(vc_RE, &vecin);
        Dvar_SetVec4FromVec4(vc_SMR, &stru_83BAA78);
        Dvar_SetVec4FromVec4(vc_SMG, &stru_83BAA88);
        Dvar_SetVec4FromVec4(vc_SMB, &stru_83BAA98);
        Dvar_SetVec4FromVec4(vc_MMR, &stru_83BAAA8);
        Dvar_SetVec4FromVec4(vc_MMG, &stru_83BAAB8);
        Dvar_SetVec4FromVec4(vc_MMB, &stru_83BAAC8);
        Dvar_SetVec4FromVec4(vc_HMR, &stru_83BAAD8);
        Dvar_SetVec4FromVec4(vc_HMG, &stru_83BAAE8);
        Dvar_SetVec4FromVec4(vc_HMB, &stru_83BAAF8);
        Dvar_SetVec4FromVec4(vc_FGM, &stru_83BAB08);
        Dvar_SetVec4FromVec4(vc_FSM, &stru_83BAB18);
        Dvar_SetVec4FromVec4(vc_FBM, &stru_83BAB28);
        gDvarDataReady = DBLOCK_EMPTY;
    }
}
