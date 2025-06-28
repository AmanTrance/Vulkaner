#include "instance.h"

#include "vulkan_core.h"

#include <stdio.h>
#include <stdlib.h>

VkInstance createVulkanInstance(const char* applicationName, const char** const extensions, uint32_t extensionsLength) {
    VkApplicationInfo vulkanApplicationInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = VK_NULL_HANDLE,
        .pApplicationName = applicationName,
        .applicationVersion = 1,
        .apiVersion = VK_API_VERSION_1_4,
        .pEngineName = NULL,
        .engineVersion = 0
    };

    VkInstanceCreateInfo vulkanInstanceInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .pApplicationInfo = &vulkanApplicationInfo,
        .enabledExtensionCount = extensionsLength,
        .enabledLayerCount = 0,
        .ppEnabledExtensionNames = extensions,
        .ppEnabledLayerNames = NULL,
        .flags = 0
    };

    VkInstance vulkanInstance;
    
    if (vkCreateInstance(&vulkanInstanceInfo, NULL, &vulkanInstance) != VK_SUCCESS) {
        printf("Vulkan Instance Result Failed\n");
        exit(1);
    }

    return vulkanInstance;
}