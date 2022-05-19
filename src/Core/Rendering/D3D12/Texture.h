#pragma once

#include <Core/Rendering/D3D12/D3D12.h>
#include <Core/Rendering/D3D12/Resource/Resource.h>
#include <Core/DataStructures/StringView.h>
#include <Core/Rendering/D3D12/Resource/DescriptorHeap.h>

namespace Kraid
{

namespace D3D12
{

    enum class TextureAddressingMode
    {
        Wrap = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        Mirror = D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
        Border = D3D12_TEXTURE_ADDRESS_MODE_BORDER,
        Clamp = D3D12_TEXTURE_ADDRESS_MODE_CLAMP
    };

    enum class TextureSamplingMode
    {
        Point = D3D12_FILTER_MIN_MAG_MIP_POINT,
        Linear = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        Anisotropic = D3D12_FILTER_ANISOTROPIC
    };

    class TextureSampler
    {
    public:
        float border_color[4] = {0,0,0,0};
        TextureAddressingMode addressing_mode = TextureAddressingMode::Wrap;
        TextureSamplingMode sampling_mode = TextureSamplingMode::Linear;
        D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor_handle;
        D3D12_GPU_DESCRIPTOR_HANDLE gpu_descriptor_handle;

        TextureSampler(
                GPUDevice& device,
                SamplerDescriptorHeap& heap, uint64 heap_index,
                TextureSamplingMode sample_mode = TextureSamplingMode::Linear,
                TextureAddressingMode addressing_mode = TextureAddressingMode::Wrap);
        TextureSampler(
                GPUDevice& device,
                SamplerDescriptorHeap& heap, uint64 heap_index,
                float border_color[4],
                TextureSamplingMode sampling_mode = TextureSamplingMode::Linear);

        void Bind(GraphicsCommandList& command_list, uint64 slot = 0);

    private:
        D3D12_SAMPLER_DESC CreateSamplerDescription() const;
    };

    class Texture
    {
    public:
        Texture2DResource texture;
        UploadBufferResource upload_buffer;
        uint64 width = 0;
        uint64 height = 0;
        uint8 channel_count = 0;

    public:
        Texture() = default;
        Texture(const StringView& filepath, GPUDevice& device, GraphicsCommandList& command_list); 
    };

}

}
