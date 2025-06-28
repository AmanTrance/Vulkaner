#include "swapchain.h"
#include "device.h"

#include "vulkan_core.h"
#include "glfw3.h"

#include <stdio.h>
#include <stdlib.h>

SwapchainDetails createSwapchain(VkInstance vulkanInstance, VkSurfaceKHR surfaceHandler, Device* device, GLFWwindow* window) {
    int width;
    int height;

    SwapchainDetails swapchainDetails = {
        .swapchain = NULL,
        .images = NULL,
        .imageViews = NULL,
        .imagesLength = 0
    };    

    glfwGetFramebufferSize(window, &width, &height);

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->physicalDevice, surfaceHandler, &surfaceCapabilities) != VK_SUCCESS) {
        return swapchainDetails;
    }

    uint32_t presentModesCount;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device->physicalDevice, surfaceHandler, &presentModesCount, NULL) != VK_SUCCESS) {
        return swapchainDetails;
    }

    VkPresentModeKHR presentModes[presentModesCount];
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device->physicalDevice, surfaceHandler, &presentModesCount, presentModes) != VK_SUCCESS) {
        return swapchainDetails;
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
        return swapchainDetails;
    }

    VkSurfaceFormatKHR formats[formatCounts];
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device->physicalDevice, surfaceHandler, &formatCounts, formats) != VK_SUCCESS) {
        return swapchainDetails;
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

    const VkSwapchainCreateInfoKHR swapchainCreateInfo = {
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
        return swapchainDetails;
    }

    uint32_t imagesCount;
    if (vkGetSwapchainImagesKHR(device->logicalDevice, swapchain, &imagesCount, NULL) != VK_SUCCESS) {
        return swapchainDetails;
    }

    VkImage* images = (VkImage*)malloc(sizeof(VkImage) * imagesCount);
    if (vkGetSwapchainImagesKHR(device->logicalDevice, swapchain, &imagesCount, images) != VK_SUCCESS) {
        free(images);
        return swapchainDetails;
    }

    VkImageView* imageViews = (VkImageView*)malloc(sizeof(VkImageView) * imagesCount);

    for (int i = 0; i < imagesCount; i++) {
        const VkImageViewCreateInfo imageViewCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = NULL,
            .image = images[i],
            .format = formatToUse,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .components = {
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .r = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseArrayLayer = 0,
                .baseMipLevel = 0,
                .layerCount = 1,
                .levelCount = 1
            }
        };

        if (vkCreateImageView(device->logicalDevice, &imageViewCreateInfo, NULL, &imageViews[i]) != VK_SUCCESS) {
            free(images);
            free(imageViews);
            return swapchainDetails;
        }
    }

    swapchainDetails.swapchain = swapchain;
    swapchainDetails.images = images;
    swapchainDetails.imageViews = imageViews;
    swapchainDetails.imagesLength = imagesCount;

    return swapchainDetails;
}