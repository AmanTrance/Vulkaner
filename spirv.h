#ifndef SPIRV_H
#define SPIRV_H

#include <stdint.h>

typedef struct SpirvByteCode {
    uint8_t* vertexBuffer;
    uint8_t* fragmentBuffer;
} SpirvByteCode;

SpirvByteCode readSpirv(const char* vPath, const char* fPath);

#endif