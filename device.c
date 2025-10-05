#include "device.h"

#include "vulkan_core.h"

#include <stdio.h>
#include <stdlib.h>

void createDevice(VulkanerStateMachine *stateMachine)
{
    uint32_t devicesCount;
    if (
        vkEnumeratePhysicalDevices(stateMachine->vulkanInstance, &devicesCount, NULL) != VK_SUCCESS)
    {
        exit(1);
    }

    VkPhysicalDevice physicalDevices[devicesCount];
    if (
        vkEnumeratePhysicalDevices(stateMachine->vulkanInstance, &devicesCount, physicalDevices) != VK_SUCCESS)
    {
        exit(1);
    }

    uint32_t deviceQueuePropertiesCount;
    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevices[devicesCount - 1],
        &deviceQueuePropertiesCount,
        NULL);

    stateMachine->queueFamilyProperties = (VkQueueFamilyProperties *)malloc(
        sizeof(VkQueueFamilyProperties) * deviceQueuePropertiesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevices[devicesCount - 1],
        &deviceQueuePropertiesCount,
        stateMachine->queueFamilyProperties);

    VkPhysicalDeviceFeatures physicalDeviceFeatures;

    vkGetPhysicalDeviceFeatures(
        physicalDevices[devicesCount - 1],
        &physicalDeviceFeatures);

    uint32_t deviceExtensionsLength;
    vkEnumerateDeviceExtensionProperties(
        physicalDevices[devicesCount - 1],
        NULL,
        &deviceExtensionsLength,
        NULL);

    VkExtensionProperties *deviceExtensions = (VkExtensionProperties *)malloc(
        sizeof(VkExtensionProperties) * deviceExtensionsLength);
    vkEnumerateDeviceExtensionProperties(
        physicalDevices[devicesCount - 1],
        NULL,
        &deviceExtensionsLength,
        deviceExtensions);

    const char *extensions[deviceExtensionsLength];

    for (uint32_t i = 0; i < deviceExtensionsLength; i++)
    {
        extensions[i] = deviceExtensions[i].extensionName;
    }

    float queuePriorities[1] = {1.0};

    VkDeviceQueueCreateInfo deviceQueueInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .queueFamilyIndex = 0,
        .queueCount = stateMachine->queueFamilyProperties[0].queueCount,
        .flags = 0,
        .pQueuePriorities = queuePriorities};

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
        .queueCreateInfoCount = 1};

    if (
        vkCreateDevice(
            physicalDevices[devicesCount - 1],
            &logicalDeviceInfo,
            NULL,
            &stateMachine->logicalDevice) != VK_SUCCESS)
    {
        printf("Logical Device Creation Failed\n");
        exit(1);
    }

    vkGetDeviceQueue(
        stateMachine->logicalDevice,
        0,
        0,
        &stateMachine->deviceQueue);

    stateMachine->physicalDevice = physicalDevices[devicesCount - 1];

    free(deviceExtensions);
}