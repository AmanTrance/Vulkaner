#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "glfw3.h"
#include "vulkan.h"
#include "vulkan_core.h"

    // int windowCreationResult = glfwInit();
    // if (windowCreationResult == GLFW_FALSE) {
    //     printf("%s\n", "window creation failed");
    //     return 1;
    // }

    // GLFWwindow* window = glfwCreateWindow(1000, 1000, "test-window", NULL, NULL);

    // glfwDestroyWindow(window);


int main() {
    VkApplicationInfo vulkan_application_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .applicationVersion = 1,
        .pApplicationName = "vulkaner",
        .apiVersion = 0,
        .pEngineName = NULL,
        .engineVersion = 0
    };

    VkInstanceCreateInfo vulkan_instance_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &vulkan_application_info,
        .pNext = NULL,
        .enabledExtensionCount = 0,
        .enabledLayerCount = 0,
        .ppEnabledExtensionNames = NULL,
        .ppEnabledLayerNames = NULL,
        .flags = VK_INSTANCE_CREATE_FLAG_BITS_MAX_ENUM
    };

    VkInstance vulkan_instance;
    
    VkResult vulkan_instance_result = vkCreateInstance(&vulkan_instance_info, NULL, &vulkan_instance);
    if (vulkan_instance_result != VK_SUCCESS) {
        printf("Vulkan Instance Result Failed\n");
        return 1;
    }

    VkPhysicalDevice* physical_devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * 10);
    uint32_t device_count = 10;

    if (vkEnumeratePhysicalDevices(vulkan_instance, &device_count, physical_devices) != VK_SUCCESS) {
        printf("Devices Fetch Failed\n");
        return 1;        
    }

    for (uint32_t i = 0; i < device_count; i++) {
        VkPhysicalDeviceProperties physical_device_properties;

        vkGetPhysicalDeviceProperties(physical_devices[i], &physical_device_properties);

        printf("%s\n", physical_device_properties.deviceName);
    }

    printf("Actual Device Count: %d\n", device_count);

    free(physical_devices);
    vkDestroyInstance(vulkan_instance, NULL);

    return 0;
}