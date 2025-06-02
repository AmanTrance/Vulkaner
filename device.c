#include "device.h"

#include "vulkan_core.h"

#include <stdio.h>
#include <stdlib.h>

Device setup_device(VkInstance vulkan_instance) {
    VkPhysicalDevice physical_devices[5];
    uint32_t devices_count = 5;

    vkEnumeratePhysicalDevices(vulkan_instance, &devices_count, physical_devices);

    uint32_t device_queue_properties_count = 10;
    VkQueueFamilyProperties* device_queue_family_properties = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * 10);

    vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[devices_count - 1], &device_queue_properties_count, device_queue_family_properties);

    VkPhysicalDeviceFeatures physical_device_features;

    vkGetPhysicalDeviceFeatures(physical_devices[devices_count - 1], &physical_device_features);

    uint32_t device_extensions_count = 200;
    VkExtensionProperties* device_extensions = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * 200);

    vkEnumerateDeviceExtensionProperties(physical_devices[devices_count - 1], NULL, &device_extensions_count, device_extensions);

    const char* extensions[device_extensions_count];

    for (uint32_t i = 0; i < device_extensions_count; i++) {
        extensions[i] = device_extensions[i].extensionName;
    }

    VkDeviceQueueCreateInfo device_queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .queueFamilyIndex = 0,
        .queueCount = device_queue_family_properties[0].queueCount,
        .flags = VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT,
        .pQueuePriorities = NULL
    };

    const char* validation_layers[1] = { "VK_LAYER_KHRONOS_validation" };

    VkDeviceCreateInfo logical_device_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .enabledLayerCount = 1,
        .ppEnabledLayerNames = validation_layers,
        .enabledExtensionCount = device_extensions_count,
        .ppEnabledExtensionNames = extensions,
        .pEnabledFeatures = &physical_device_features,
        .pQueueCreateInfos = &device_queue_info,
        .queueCreateInfoCount = 1
    };

    VkDevice logical_device;
    if (vkCreateDevice(physical_devices[devices_count - 1], &logical_device_info, NULL, &logical_device) != VK_SUCCESS) {
        printf("Logical Device Creation Failed\n");
        free(device_queue_family_properties);
        free(device_extensions);
        vkDestroyInstance(vulkan_instance, NULL);
        exit(1);
    }

    VkQueue device_queue;
    vkGetDeviceQueue(logical_device, 0, 0, &device_queue);

    Device device = {
        .raw_device = physical_devices[devices_count - 1],
        .logical_device = logical_device,
        .device_queue = device_queue,
        .queue_family_properties = device_queue_family_properties[0]
    };

    free(device_extensions);
    free(device_queue_family_properties);

    return device;
}