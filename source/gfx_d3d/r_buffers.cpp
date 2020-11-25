#include "r_buffers.h"
#include "r_init.h"
#include "r_dvars.h"

#include <qcommon/common.h>
#include <universal/assertive.h>

char* R_LockVertexBuffer(ID3D11DeviceContext* device, ID3D11Buffer* handle, int offset, int bytes, int lockFlags)
{
    int hr;
    const char* errorDescription;
    D3D11_MAPPED_SUBRESOURCE map;

    if (!handle
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(handle)",
            nullptr))
    {
        __debugbreak();
    }
    while (1)
    {
        hr = device->Map(handle, 0, (D3D11_MAP)lockFlags, 0, &map);
        if (hr < 0)
            break;
        if (!alwaysfails)
            return (char*)map.pData + offset;
    }
    do
    {
        ++g_disableRendering;
        errorDescription = R_ErrorDescription(hr);
        Com_Error(
            ERR_FATAL,
            __FILE__" (%i) device->Map(handle, 0, (D3D11_MAP)lockFlags, 0, &map) failed: %s\n",
            __LINE__,
            errorDescription);
    } while (alwaysfails);
    return (char*)map.pData + offset;
}

void R_UnlockVertexBuffer(ID3D11DeviceContext* device, ID3D11Buffer* handle)
{
    if (!handle
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(handle)",
            nullptr))
    {
        __debugbreak();
    }
    device->Unmap(handle, 0);
}

char* R_LockIndexBuffer(ID3D11DeviceContext* device, ID3D11Buffer* handle, int offset, int bytes, int lockFlags)
{
    int hr;
    const char* errorDescription;
    D3D11_MAPPED_SUBRESOURCE map;

    if (!handle
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(handle)",
            nullptr))
    {
        __debugbreak();
    }
    while (1)
    {
        hr = device->Map(handle, 0, (D3D11_MAP)lockFlags, 0, &map);
        if (hr < 0)
            break;
        if (!alwaysfails)
            return (char*)map.pData + offset;
    }
    do
    {
        ++g_disableRendering;
        errorDescription = R_ErrorDescription(hr);
        Com_Error(
            ERR_FATAL,
            __FILE__" (%i) device->Map(handle, 0, (D3D11_MAP)lockFlags, 0, &map) failed: %s\n",
            149,
            errorDescription);
    } while (alwaysfails);
    return (char*)map.pData + offset;
}

void R_UnlockIndexBuffer(ID3D11DeviceContext* device, ID3D11Buffer* handle)
{
    if (!handle
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "(handle)",
            nullptr))
    {
        __debugbreak();
    }
    device->Unmap(handle, 0);
}

char* R_AllocDynamicVertexBuffer(ID3D11Buffer** vb, int sizeInBytes)
{
    int hr;
    const char* errorDescription;
    D3D11_BUFFER_DESC bd;

    if (!vb
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(vb)", nullptr))
    {
        __debugbreak();
    }
    if (sizeInBytes <= 0
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((sizeInBytes > 0))",
            "(sizeInBytes) = %i",
            sizeInBytes))
    {
        __debugbreak();
    }
    if (Dvar_GetBool(r_loadForRenderer))
    {
        bd.MiscFlags = 0;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeInBytes;
        bd.BindFlags = 1;
        bd.CPUAccessFlags = 0x10000;
        while (1)
        {
            hr = dx.device->CreateBuffer(&bd, 0, vb);
            if (hr < 0)
                break;
            if (!alwaysfails)
                return 0;
        }
        do
        {
            ++g_disableRendering;
            errorDescription = R_ErrorDescription(hr);
            Com_Error(
                ERR_FATAL,
                __FILE__" (%i) dx.device->CreateBuffer(&bd, 0, vb) failed: %s\n",
                178,
                errorDescription);
        } while (alwaysfails);
    }
    return 0;
}

void* R_AllocStaticVertexBuffer(ID3D11Buffer** vb, int sizeInBytes, const void* data)
{
    int hr;
    const char* errorDescription;
    D3D11_SUBRESOURCE_DATA initData;
    D3D11_BUFFER_DESC bd;

    if (!vb
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(vb)", nullptr))
    {
        __debugbreak();
    }
    if (sizeInBytes <= 0
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((sizeInBytes > 0))",
            "(sizeInBytes) = %i",
            sizeInBytes))
    {
        __debugbreak();
    }
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    initData.SysMemPitch = 0;
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeInBytes;
    bd.BindFlags = 1;
    initData.SysMemSlicePitch = 0;
    initData.pSysMem = data;
    while (1)
    {
        hr = dx.device->CreateBuffer(&bd, &initData, vb);
        if (hr < 0)
            break;
        if (!alwaysfails)
            return 0;
    }
    do
    {
        ++g_disableRendering;
        errorDescription = R_ErrorDescription(hr);
        Com_Error(
            ERR_FATAL,
            __FILE__" (%i) dx.device->CreateBuffer(&bd, &initData, vb) failed: %s\n",
            200,
            errorDescription);
    } while (alwaysfails);
    return 0;
}

void* R_AllocDynamicIndexBuffer(ID3D11Buffer** ib, int sizeInBytes)
{
    int hr;
    const char* errorDescription;
    D3D11_BUFFER_DESC bd;

    if (Dvar_GetBool(r_loadForRenderer))
    {
        bd.MiscFlags = 0;
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeInBytes;
        bd.BindFlags = 2;
        bd.CPUAccessFlags = 0x10000;
        while (1)
        {
            hr = dx.device->CreateBuffer(&bd, 0, ib);
            if (hr < 0)
                break;
            if (!alwaysfails)
                return 0;
        }
        do
        {
            ++g_disableRendering;
            errorDescription = R_ErrorDescription(hr);
            Com_Error(
                ERR_FATAL,
                __FILE__" (%i) dx.device->CreateBuffer(&bd, 0, ib) failed: %s\n",
                __LINE__,
                errorDescription);
        } while (alwaysfails);
    }
    return 0;
}

void* R_AllocStaticIndexBuffer(ID3D11Buffer** ib, int sizeInBytes, const void* data)
{
    int hr;
    const char* errorDescription;
    D3D11_SUBRESOURCE_DATA initData;
    D3D11_BUFFER_DESC bd;

    if (!ib
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(ib)", nullptr))
    {
        __debugbreak();
    }
    if (sizeInBytes <= 0
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            0,
            "((sizeInBytes > 0))",
            "(sizeInBytes) = %i",
            sizeInBytes))
    {
        __debugbreak();
    }
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    initData.SysMemPitch = 0;
    bd.Usage = D3D11_USAGE_IMMUTABLE;
    bd.ByteWidth = sizeInBytes;
    bd.BindFlags = 2;
    initData.SysMemSlicePitch = 0;
    initData.pSysMem = data;
    while (1)
    {
        hr = dx.device->CreateBuffer(&bd, &initData, ib);
        if (hr < 0)
            break;
        if (!alwaysfails)
            return 0;
    }
    do
    {
        ++g_disableRendering;
        errorDescription = R_ErrorDescription(hr);
        Com_Error(
            ERR_FATAL,
            __FILE__" (%i) dx.device->CreateBuffer(&bd, &initData, ib) failed: %s\n",
            __LINE__,
            errorDescription);
    } while (alwaysfails);
    return 0;
}

void Load_IndexBuffer(ID3D11Buffer** ib, void* bufferData, int indexCount)
{
    if (Dvar_GetBool(r_loadForRenderer))
    {
        if (!bufferData
            && !Assert_MyHandler(
                __FILE__,
                __LINE__,
                0,
                "(bufferData)",
                nullptr))
        {
            __debugbreak();
        }
        R_AllocStaticIndexBuffer(ib, 2 * indexCount, bufferData);
    }
}

void Load_VertexBuffer(ID3D11Buffer** vb, void* bufferData, int sizeInBytes)
{
    if (Dvar_GetBool(r_loadForRenderer) && bufferData)
        R_AllocStaticVertexBuffer(vb, sizeInBytes, bufferData);
    else
        *vb = 0;
}

void R_InitDynamicVertexBufferState(GfxVertexBufferState* vb, int bytes)
{
    char* dynVertexBuffer; // edi

    if (!vb
        && !Assert_MyHandler(__FILE__, __LINE__, 0, "(vb)", nullptr))
    {
        __debugbreak();
    }
    vb->total = bytes;
    vb->used = 0;
    dynVertexBuffer = R_AllocDynamicVertexBuffer(&vb->buffer, bytes);
    if (dynVertexBuffer
        && !Assert_MyHandler(
            __FILE__,
            __LINE__,
            1,
            "(verts == 0)",
            nullptr))
    {
        __debugbreak();
    }
    vb->verts = dynVertexBuffer;
}

void R_InitSkinnedVertexBuffers(SkinnedVertsDef* sv)
{
}

void R_FreeStaticVertexBuffer(ID3D11Buffer* vb)
{
    ID3D11Buffer* inBuf;
    ID3D11Buffer* relBuf;
    int releaseCheck;
    const char* vaErr;

    inBuf = vb;
    do
    {
        relBuf = inBuf;
        inBuf = 0;
        if (!relBuf
            && !Assert_MyHandler(__FILE__, __LINE__, 0, "(var)", nullptr))
        {
            __debugbreak();
        }
        releaseCheck = relBuf->Release();
        if (releaseCheck)
        {
            vaErr = va(
                "%s (%i) %s->Release() failed: %i leak(s)!",
                __FILE__,
                __LINE__,
                "vb",
                releaseCheck);
            if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(!useCount)", "%s", vaErr))
                __debugbreak();
        }
    } while (alwaysfails);
}

void R_FreeStaticIndexBuffer(ID3D11Buffer* ib)
{
    ID3D11Buffer* inBuf; // edi
    ID3D11Buffer* relBuf; // esi
    int releaseCheck; // eax
    const char* vaErr; // eax

    inBuf = ib;
    do
    {
        relBuf = inBuf;
        inBuf = 0;
        if (!relBuf
            && !Assert_MyHandler(__FILE__, __LINE__, 0, "(var)", nullptr))
        {
            __debugbreak();
        }
        releaseCheck = relBuf->Release();
        if (releaseCheck)
        {
            vaErr = va(
                "%s (%i) %s->Release() failed: %i leak(s)!",
                __FILE__,
                __LINE__,
                "ib",
                releaseCheck);
            if (!Assert_MyHandler(__FILE__, __LINE__, 0, "(!useCount)", "%s", vaErr))
                __debugbreak();
        }
    } while (alwaysfails);
}

void R_ShutdownSkinnedVertexBuffers()
{

}
