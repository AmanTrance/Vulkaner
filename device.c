#include <stdio.h>
#include <stdlib.h>
#include "vulkan_core.h"

typedef struct Device {
    VkPhysicalDevice        raw_device;
    VkDevice                logical_device;
    VkQueue                 device_queue;
    VkQueueFamilyProperties queue_family_properties;
} Device;

Device setup_device(VkInstance vulkan_instance) {
    VkPhysicalDevice physical_device;
    uint32_t device_count = 1;

    if (
        vkEnumeratePhysicalDevices(vulkan_instance, &device_count, &physical_device) != VK_SUCCESS &&
        vkEnumeratePhysicalDevices(vulkan_instance, &device_count, &physical_device) != VK_INCOMPLETE        
    ) {
        printf("Devices Fetch Failed\n");
        exit(1);
    }

    uint32_t device_queue_properties_count = 1;
    VkQueueFamilyProperties device_queue_family_properties;

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &device_queue_properties_count, &device_queue_family_properties);

    VkDeviceQueueCreateInfo device_queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .queueFamilyIndex = 0,
        .queueCount = 1,
        .flags = VK_DEVICE_QUEUE_CREATE_FLAG_BITS_MAX_ENUM,
        .pQueuePriorities = NULL
    };

    VkDeviceCreateInfo logical_device_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = NULL,
        .pEnabledFeatures = NULL,
        .pQueueCreateInfos = &device_queue_info,
        .queueCreateInfoCount = 1
    };

    VkDevice logical_device;
    if (vkCreateDevice(physical_device, &logical_device_info, NULL, &logical_device) != VK_SUCCESS) {
        printf("Logical Device Creation Failed\n");
        printf("%d\n", vkCreateDevice(physical_device, &logical_device_info, NULL, &logical_device));
        exit(1);
    }

    VkQueue device_queue;
    vkGetDeviceQueue(logical_device, 0, 0, &device_queue);

    Device device = {
        .raw_device = physical_device,
        .logical_device = logical_device,
        .device_queue = device_queue,
        .queue_family_properties = device_queue_family_properties
    };

    return device;
}