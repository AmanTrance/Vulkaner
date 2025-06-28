#include "instance.h"
#include "device.h"
#include "surface.h"
#include "swapchain.h"
#include "spirv.h"

#include "glfw3.h"
#include "vulkan_core.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    readSpirv("./vertex.spv", "./fragment.spv");

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

    SwapchainDetails swapchainDetails = createSwapchain(vulkanInstance, surfaceHandle, &device, window);
    if (swapchainDetails.swapchain == NULL) {
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

    for (int i = 0; i < swapchainDetails.imagesLength; i++) {
        vkDestroyImageView(device.logicalDevice, swapchainDetails.imageViews[i], NULL);
    }

    vkDestroySwapchainKHR(device.logicalDevice, swapchainDetails.swapchain, NULL);
    vkDestroySurfaceKHR(vulkanInstance, surfaceHandle, NULL);
    vkDestroyDevice(device.logicalDevice, NULL);
    vkDestroyInstance(vulkanInstance, NULL);

    free(swapchainDetails.imageViews);
    free(swapchainDetails.images);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}