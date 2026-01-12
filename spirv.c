#include "spirv.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

void createShaderModules(VulkanerStateMachine *stateMachine)
{
    int vertexfd = open("vertex.spv", O_RDONLY);
    int fragmentfd = open("fragment.spv", O_RDONLY);

    struct stat vertexStat;
    struct stat fragmentStat;

    fstat(vertexfd, &vertexStat);
    fstat(fragmentfd, &fragmentStat);

    uint8_t *vertexBuffer = (uint8_t *)malloc(sizeof(uint8_t) * vertexStat.st_size);

    ssize_t vertexReadSize = read(vertexfd, vertexBuffer, vertexStat.st_size);

    uint8_t *fragmentBuffer = (uint8_t *)malloc(sizeof(uint8_t) * fragmentStat.st_size);

    ssize_t fragmentReadSize = read(fragmentfd, fragmentBuffer, fragmentStat.st_size);

    close(vertexfd);
    close(fragmentfd);

    const VkShaderModuleCreateInfo vertexModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pCode = (uint32_t *)vertexBuffer,
        .codeSize = vertexReadSize};

    const VkShaderModuleCreateInfo fragmentModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pCode = (uint32_t *)fragmentBuffer,
        .codeSize = fragmentReadSize};

    VkShaderModule vertexShaderModule;
    if (
        vkCreateShaderModule(
            stateMachine->logicalDevice,
            &vertexModuleCreateInfo,
            NULL,
            &vertexShaderModule) != VK_SUCCESS)
    {
        exit(1);
    }

    VkShaderModule fragmentShaderModule;
    if (
        vkCreateShaderModule(
            stateMachine->logicalDevice,
            &fragmentModuleCreateInfo,
            NULL,
            &fragmentShaderModule) != VK_SUCCESS
    )
    {
        exit(1);
    }
}