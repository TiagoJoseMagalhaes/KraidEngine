#include "PipelineStateObject.h"

#include <Core/Rendering/D3D12/D3D12.h>
#include <Core/Windows.h>

#include <dxgiformat.h>

namespace Kraid
{

namespace D3D12
{

    //NOTE(Tiago):should this maybe be in the index buffer or vertex buffer class
    enum class TriangleStripDiscontinuityIndicator
    {
        None = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
        Cut0xFFFF = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF,
        Cut0xFFFFFFFF = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF
    };

    void GraphicsPipelineStateObject::Compile(GPUDevice& device)
    {
        D3D12_SHADER_BYTECODE null_bytecode = {};

        D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
        //TODO(Tiago):root signature is missing
        pso_desc.VS = this->vertex_shader != nullptr?this->vertex_shader->GetShaderBytecode():null_bytecode;
        pso_desc.PS = this->pixel_shader != nullptr?this->pixel_shader->GetShaderBytecode():null_bytecode;
        pso_desc.DS = this->domain_shader != nullptr?this->domain_shader->GetShaderBytecode(): null_bytecode;
        pso_desc.HS = this->hull_shader != nullptr?this->hull_shader->GetShaderBytecode():null_bytecode;
        pso_desc.GS = this->geometry_shader != nullptr?this->geometry_shader->GetShaderBytecode():null_bytecode;

        pso_desc.StreamOutput = this->streaming_output_buffer.GetStreamingOutputBufferDescritpion();
        pso_desc.BlendState = this->blending.GetBlendingDescription();
        pso_desc.SampleMask = 0;//TODO(Tiago):needs to be done
        pso_desc.RasterizerState = this->rasterizer.CreateRasterizerDescritpion();
        pso_desc.DepthStencilState = this->depth_stencil_stage.CreateDepthStentilStageDescription();
        pso_desc.InputLayout = this->vertex_layout;
        pso_desc.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE) this->topology_type;
        pso_desc.IBStripCutValue = (D3D12_INDEX_BUFFER_STRIP_CUT_VALUE) TriangleStripDiscontinuityIndicator::None;
        pso_desc.NumRenderTargets = 1;//TODO(Tiago):hardcoded right now
        //setting rtv formats
        {
            pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
            pso_desc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
        }
        //TODO(Tiago):we need some way to specify this when a triangle strip is being used
        pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        pso_desc.SampleDesc.Count = 1;
        pso_desc.SampleDesc.Quality = 0;
        pso_desc.NodeMask = 0;
        pso_desc.CachedPSO = {};
        pso_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

        D3DCALL(device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&this->pso)), "Pipeline State Object Compiled");
    }

    void GraphicsPipelineStateObject::Bind(GraphicsCommandList& comamnd_list) const
    {
        comamnd_list->SetPipelineState(this->pso.Get());
    }

}

}
