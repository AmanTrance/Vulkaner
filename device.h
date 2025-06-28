#ifndef DEVICE_H
#define DEVICE_H

#include "vulkan_core.h"

typedef struct Device {
    VkPhysicalDevice        physicalDevice;
    VkDevice                logicalDevice;
    VkQueue                 deviceQueue;
} Device;

Device setupDevice(VkInstance vulkanInstance);

#endif