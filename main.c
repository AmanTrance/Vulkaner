#include "instance.c"
#include "device.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "glfw3.h"
#include "vulkan_core.h"


int main() {
    if (glfwInit() == GLFW_FALSE) {
        return 1;
    }

    uint32_t extensions_length = 0;

    const char** const extensions = glfwGetRequiredInstanceExtensions(&extensions_length);

    VkInstance vulkan_instance = create_vulkan_instance("vulkaner", extensions, extensions_length);
    Device device = setup_device(vulkan_instance);

    vkDestroyDevice(device.logical_device, NULL);
    vkDestroyInstance(vulkan_instance, NULL);

    return 0;
}