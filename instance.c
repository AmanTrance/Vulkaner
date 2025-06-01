#include <stdio.h>
#include <stdlib.h>
#include "vulkan_core.h"

VkInstance create_vulkan_instance(const char* application_name, const char** const extensions, uint32_t extensions_length) {
    VkApplicationInfo vulkan_application_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = VK_NULL_HANDLE,
        .pApplicationName = application_name,
        .applicationVersion = 1,
        .apiVersion = VK_API_VERSION_1_4,
        .pEngineName = NULL,
        .engineVersion = 0
    };

    VkInstanceCreateInfo vulkan_instance_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .pApplicationInfo = &vulkan_application_info,
        .enabledExtensionCount = extensions_length,
        .enabledLayerCount = 0,
        .ppEnabledExtensionNames = extensions,
        .ppEnabledLayerNames = NULL,
        .flags = 0
    };

    VkInstance vulkan_instance;
    
    if (vkCreateInstance(&vulkan_instance_info, NULL, &vulkan_instance) != VK_SUCCESS) {
        printf("Vulkan Instance Result Failed\n");
        exit(1);
    }

    return vulkan_instance;
}