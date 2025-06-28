#include "swapchain.h"
#include "device.h"

#include "vulkan_core.h"
#include "glfw3.h"

#include <stdio.h>

VkSwapchainKHR createSwapchain(VkInstance vulkanInstance, VkSurfaceKHR surfaceHandler, Device* device, GLFWwindow* window) {
    int width;
    int height;
    
    glfwGetFramebufferSize(window, &width, &height);

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->physicalDevice, surfaceHandler, &surfaceCapabilities) != VK_SUCCESS) {
        return NULL;
    }

    uint32_t presentModesCount;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device->physicalDevice, surfaceHandler, &presentModesCount, NULL) != VK_SUCCESS) {
        return NULL;
    }

    VkPresentModeKHR presentModes[presentModesCount];
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device->physicalDevice, surfaceHandler, &presentModesCount, presentModes) != VK_SUCCESS) {
        return NULL;
    }

    VkPresentModeKHR presentModeToUse;
    for (int i = 0; i < presentModesCount; i++) {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentModeToUse = VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    if (presentModeToUse == 0) {
        presentModeToUse = VK_PRESENT_MODE_FIFO_KHR;
    }

    uint32_t formatCounts;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device->physicalDevice, surfaceHandler, &formatCounts, NULL) != VK_SUCCESS) {
        return NULL;
    }

    VkSurfaceFormatKHR formats[formatCounts];
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device->physicalDevice, surfaceHandler, &formatCounts, formats) !== VK_SUCCESS) {
        return NULL;
    }

    VkFormat formatToUse;
    VkColorSpaceKHR colorSpaceToUse;

    for (uint32_t i = 0; i < formatCounts; i++) {
        if (((formats[i].format & VK_FORMAT_B8G8R8A8_SRGB) == VK_FORMAT_B8G8R8A8_SRGB)) {
            formatToUse = VK_FORMAT_B8G8R8A8_SRGB;
            colorSpaceToUse = formats[i].colorSpace;
        }
    }

    if (formatToUse == 0 && formatCounts > 0) {
        formatToUse = formats[0].format;
        colorSpaceToUse = formats[0].colorSpace;
    }

    VkSwapchainCreateInfoKHR const swapchainCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,
        .oldSwapchain = NULL,
        .surface = surfaceHandler,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = NULL,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentModeToUse,
        .imageFormat = formatToUse,
        .clipped = VK_TRUE,
        .imageExtent = {
            .height = height,
            .width = width
        },
        .minImageCount = surfaceCapabilities.minImageCount,
        .imageUsage = surfaceCapabilities.supportedUsageFlags,
        .flags = VK_SWAPCHAIN_CREATE_PROTECTED_BIT_KHR,
        .imageArrayLayers = 1,
        .imageColorSpace = colorSpaceToUse
    };

    VkSwapchainKHR swapchain;
    if (vkCreateSwapchainKHR(device->logicalDevice, &swapchainCreateInfo, NULL, &swapchain) != VK_SUCCESS) {
        return NULL;
    }

    return swapchain;
}