#include "surface.h"
#include "device.h"

#include "vulkan_core.h"
#include "glfw3.h"

#include <stdio.h>
#include <stdlib.h>

VkSurfaceKHR setup_surface(VkInstance vulkan_instance, Device* device, GLFWwindow* window) {
    VkSurfaceKHR surface_handle;

    if (glfwCreateWindowSurface(vulkan_instance, window, NULL, &surface_handle) != VK_SUCCESS) {
        printf("Window Surface Creation Failed\n");
        vkDestroyDevice(device->logical_device, NULL);
        vkDestroyInstance(vulkan_instance, NULL);
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }

    return surface_handle;
}