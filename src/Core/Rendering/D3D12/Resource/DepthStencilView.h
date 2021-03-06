#pragma once

#include <Core/Rendering/D3D12/D3D12.h>
#include <Core/types.h>

namespace Kraid
{

    namespace D3D12
    {

        struct GPUDevice;
        struct Texture1DResource;
        struct Texture2DResource;
        struct GraphicsCommandList;

        struct DepthStencilView
        {
            D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle;
            uint32 width = 0;
            uint32 height = 0;

            DepthStencilView() = default;
            DepthStencilView(GPUDevice& device, Texture1DResource& resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);
            DepthStencilView(GPUDevice& device, Texture2DResource& resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);

            void Clear(GraphicsCommandList& command_list);
        };

    }

}

