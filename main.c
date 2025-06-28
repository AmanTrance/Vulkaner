#include "instance.h"
#include "device.h"
#include "surface.h"
#include "swapchain.h"

#include "glfw3.h"
#include "vulkan_core.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    if (!glfwInit()) {
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Vulkaner", NULL, NULL);

    uint32_t extensionsLength = 0;
    const char** const extensions = glfwGetRequiredInstanceExtensions(&extensionsLength);

    VkInstance vulkanInstance = createVulkanInstance("Vulkaner", extensions, extensionsLength);
    VkSurfaceKHR surfaceHandle = setupSurface(vulkanInstance, window);
    if (surfaceHandle == NULL) {
        vkDestroyInstance(vulkanInstance, NULL);

        glfwDestroyWindow(window);
        glfwTerminate();

        return 1;
    }

    Device device = setupDevice(vulkanInstance);

    VkSwapchainKHR swapchain = createSwapchain(vulkanInstance, surfaceHandle, &device, window);
    if (swapchain == NULL) {
        vkDestroySurfaceKHR(vulkanInstance, surfaceHandle, NULL);
        vkDestroyDevice(device.logicalDevice, NULL);
        vkDestroyInstance(vulkanInstance, NULL);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    vkDestroySwapchainKHR(device.logicalDevice, swapchain, NULL);
    vkDestroySurfaceKHR(vulkanInstance, surfaceHandle, NULL);
    vkDestroyDevice(device.logicalDevice, NULL);
    vkDestroyInstance(vulkanInstance, NULL);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}