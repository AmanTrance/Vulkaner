#include "instance.h"
#include "device.h"
#include "surface.h"

#include "glfw3.h"
#include "vulkan_core.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    if (glfwInit() == GLFW_FALSE) {
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Vulkaner", NULL, NULL);

    uint32_t extensions_length = 0;

    const char** const extensions = glfwGetRequiredInstanceExtensions(&extensions_length);

    VkInstance vulkan_instance = create_vulkan_instance("vulkaner", extensions, extensions_length);
    Device device = setup_device(vulkan_instance);
    VkSurfaceKHR surface_handle = setup_surface(vulkan_instance, &device, window);

    vkDestroySurfaceKHR(vulkan_instance, surface_handle, NULL);
    vkDestroyDevice(device.logical_device, NULL);
    vkDestroyInstance(vulkan_instance, NULL);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}