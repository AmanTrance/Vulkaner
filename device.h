#ifndef DEVICE_H
#define DEVICE_H

#include "vulkan_core.h"

typedef struct Device {
    VkPhysicalDevice        raw_device;
    VkDevice                logical_device;
    VkQueue                 device_queue;
    VkQueueFamilyProperties queue_family_properties;
} Device;

Device setup_device(VkInstance vulkan_instance);

#endif