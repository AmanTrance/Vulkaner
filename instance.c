#include "instance.h"

#include "vulkan_core.h"

#include <stdio.h>
#include <stdlib.h>

void createVulkanInstance(VulkanerStateMachine *stateMachine, const char *applicationName)
{
    uint32_t extensionsLength = 0;
    const char *const *extensions = glfwGetRequiredInstanceExtensions(&extensionsLength);

    VkApplicationInfo vulkanApplicationInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = VK_NULL_HANDLE,
        .pApplicationName = applicationName,
        .applicationVersion = 1,
        .apiVersion = VK_API_VERSION_1_4,
        .pEngineName = NULL,
        .engineVersion = 0};

    VkInstanceCreateInfo vulkanInstanceInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .pApplicationInfo = &vulkanApplicationInfo,
        .enabledExtensionCount = extensionsLength,
        .enabledLayerCount = 0,
        .ppEnabledExtensionNames = extensions,
        .ppEnabledLayerNames = NULL,
        .flags = 0};

    if (
        vkCreateInstance(
            &vulkanInstanceInfo,
            NULL,
            &stateMachine->vulkanInstance) != VK_SUCCESS)
    {
        printf("Vulkan Instance Result Failed\n");
        exit(1);
    }
}