#include "spirv.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

SpirvByteCode readSpirv(const char* vPath, const char* fPath) {
    SpirvByteCode spirvDetails = {
        .vertexBuffer = NULL,
        .fragmentBuffer = NULL
    };

    int vertexfd = open(vPath, O_RDONLY);
    int fragmentfd = open(fPath, O_RDONLY);

    struct stat vertexStat;
    struct stat fragmentStat;

    fstat(vertexfd, &vertexStat);
    fstat(fragmentfd, &fragmentStat);

    uint8_t* vertexBuffer = (uint8_t*)malloc(sizeof(uint8_t) * vertexStat.st_size);

    ssize_t vertexReadSize = read(vertexfd, vertexBuffer, vertexStat.st_size);

    uint8_t* fragmentBuffer = (uint8_t*)malloc(sizeof(uint8_t) * fragmentStat.st_size);

    ssize_t fragmentReadSize = read(fragmentfd, fragmentBuffer, fragmentStat.st_size);

    free(vertexBuffer);
    free(fragmentBuffer);

    close(vertexfd);
    close(fragmentfd);

    return spirvDetails;
}