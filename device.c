#include "device.h"

#include "vulkan_core.h"

#include <stdio.h>
#include <stdlib.h>

Device setupDevice(VkInstance vulkanInstance) {
    VkPhysicalDevice physicalDevices[5];
    uint32_t devicesCount = 5;

    vkEnumeratePhysicalDevices(vulkanInstance, &devicesCount, physicalDevices);

    uint32_t deviceQueuePropertiesCount = 10;
    VkQueueFamilyProperties* deviceQueueFamilyProperties = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * 10);

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[devicesCount - 1], &deviceQueuePropertiesCount, deviceQueueFamilyProperties);

    VkPhysicalDeviceFeatures physicalDeviceFeatures;

    vkGetPhysicalDeviceFeatures(physicalDevices[devicesCount - 1], &physicalDeviceFeatures);

    uint32_t deviceExtensionsLength = 200;
    VkExtensionProperties* deviceExtensions = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * 200);

    vkEnumerateDeviceExtensionProperties(physicalDevices[devicesCount - 1], NULL, &deviceExtensionsLength, deviceExtensions);

    const char* extensions[deviceExtensionsLength];

    for (uint32_t i = 0; i < deviceExtensionsLength; i++) {
        extensions[i] = deviceExtensions[i].extensionName;
    }

    float queuePriorities[1] = { 1.0 };

    VkDeviceQueueCreateInfo deviceQueueInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .queueFamilyIndex = 0,
        .queueCount = deviceQueueFamilyProperties[0].queueCount,
        .flags = 0,
        .pQueuePriorities = queuePriorities
    };

    VkDeviceCreateInfo logicalDeviceInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = deviceExtensionsLength,
        .ppEnabledExtensionNames = extensions,
        .pEnabledFeatures = &physicalDeviceFeatures,
        .pQueueCreateInfos = &deviceQueueInfo,
        .queueCreateInfoCount = 1
    };

    VkDevice logicalDevice;
    if (vkCreateDevice(physicalDevices[devicesCount - 1], &logicalDeviceInfo, NULL, &logicalDevice) != VK_SUCCESS) {
        printf("Logical Device Creation Failed\n");
        free(deviceQueueFamilyProperties);
        free(deviceExtensions);
        vkDestroyInstance(vulkanInstance, NULL);
        exit(1);
    }

    VkQueue device_queue;
    vkGetDeviceQueue(logicalDevice, 0, 0, &device_queue);

    Device device = {
        .physicalDevice = physicalDevices[devicesCount - 1],
        .logicalDevice = logicalDevice,
        .deviceQueue = device_queue
    };

    free(deviceExtensions);
    free(deviceQueueFamilyProperties);

    return device;
}