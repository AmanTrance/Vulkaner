#include "surface.h"
#include "device.h"

#include "vulkan_core.h"
#include "vulkan.h"
#include "glfw3.h"

#include <stdio.h>
#include <stdlib.h>

VkSurfaceKHR setupSurface(VkInstance vulkanInstance, GLFWwindow* window) {
    VkSurfaceKHR surfaceHandle;

    if (glfwCreateWindowSurface(vulkanInstance, window, NULL, &surfaceHandle) != VK_SUCCESS) {
        printf("Window Surface Creation Failed\n");
        return NULL;
    }

    return surfaceHandle;
}