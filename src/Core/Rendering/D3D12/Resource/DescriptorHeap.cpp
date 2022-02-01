#include "DescriptorHeap.h"

#include <Core/Windows.h>
#include <Core/Utils/Log.h>

#include "../GPUDevice.h"

namespace Kraid
{

    namespace D3D12
    {

        ID3D12DescriptorHeap* DescriptorHeap::operator->()
        {
            return this->descriptor_heap.Get();
        }

        D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::operator[](const uint64 index)
        {
            if(index > this->heap_size)
            {
                LWARNING("Out of bounds heap access");
                return {NULL};
            }
            return {this->descriptor_heap->GetCPUDescriptorHandleForHeapStart().ptr + index * this->descriptor_increment_size};
        }

        RTVDescriptorHeap::RTVDescriptorHeap(GPUDevice& device, uint32 heap_size)
        {
            this->descriptor_increment_size = device.descriptor_sizes.rtv;
            this->heap_size = heap_size;

            D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
            heap_desc.NumDescriptors = heap_size;
            heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

            D3DCALL(device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&this->descriptor_heap)), "RTV Descriptor Heap Created");
        }

        DSVDescriptorHeap::DSVDescriptorHeap(GPUDevice& device, uint32 heap_size)
        {
            this->descriptor_increment_size = device.descriptor_sizes.dsv;
            this->heap_size = heap_size;

            D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
            heap_desc.NumDescriptors = heap_size;
            heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
            heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

            D3DCALL(device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&this->descriptor_heap)), "DSV Descriptor Heap Created");
        }

        SamplerDescriptorHeap::SamplerDescriptorHeap(GPUDevice& device, uint32 heap_size)
        {
            this->descriptor_increment_size = device.descriptor_sizes.sampler;
            this->heap_size = heap_size;

            D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
            heap_desc.NumDescriptors = heap_size;
            heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
            heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

            D3DCALL(device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&this->descriptor_heap)), "Sampler Descriptor Heap Created");
        }
        
        CBV_SRV_UAVDescriptorHeap::CBV_SRV_UAVDescriptorHeap(GPUDevice& device, uint32 heap_size)
        {
            this->descriptor_increment_size = device.descriptor_sizes.cbv_srv_uav;
            this->heap_size = heap_size;

            D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
            heap_desc.NumDescriptors = heap_size;
            heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

            D3DCALL(device->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&this->descriptor_heap)), "CBV/SRV/UAV Descriptor Heap Created");
        }

    }

}