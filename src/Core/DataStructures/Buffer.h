#pragma once

#include <Core/Utils/Log.h>
#include <Core/types.h>

namespace Kraid
{

    struct Buffer
    {
        uint8* data = nullptr;
        uint64 size = 0;

        Buffer() = default;
        Buffer(uint64 size);
        Buffer(const Buffer& other);
        Buffer(Buffer&& other);
        Buffer(uint8* data, uint64 size);
        Buffer& operator=(const Buffer& other);
        ~Buffer();
    };

}
