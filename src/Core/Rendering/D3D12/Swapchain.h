#pragma once

#include <Core/Rendering/D3D12/D3D12.h>
#include <vector>

#include <Core/Rendering/D3D12/Resource/DescriptorHeap.h>
#include <Core/Rendering/D3D12/Resource/Resource.h>
#include <Core/Rendering/D3D12/Resource/DepthStencilView.h>
#include <Core/Rendering/D3D12/Resource/RenderTargetView.h>

namespace Kraid
{
    struct Window;

    namespace D3D12
    {
        
        struct GPUDevice;
        struct GraphicsCommandList;

        using namespace Microsoft::WRL;

        struct Swapchain
        {
            ComPtr<IDXGISwapChain4> swapchain = nullptr;
            std::vector<ComPtr<ID3D12Resource2>> render_target_buffers;
            Texture2DResource depth_stencil_buffer;
            DepthStencilView depth_stencil_view;
            RTVDescriptorHeap rtv_heap;
            DSVDescriptorHeap dsv_heap;
            uint8 render_target_count = 2;

        public:
            Swapchain() = default;
            Swapchain(GPUDevice& device, Window& window, GraphicsCommandList& command_list);
            IDXGISwapChain4* operator->();

        private:
            void CreateSwapchain(GPUDevice& device, Window& window);
            void CreateRenderTargetViews(GPUDevice& device);
            void CreateDepthStencilView(GPUDevice& device, Window& window, GraphicsCommandList&  command_list);
            void SetViewport(GraphicsCommandList& command_list, Window& window);
            void SetScisorRect(GraphicsCommandList& command_list, Window& window);
            void SetRenderTarget(GraphicsCommandList& command_list);
        };

    }
}