#ifndef SWAPCHAIN_H
#define SWAPCHAIN

#include "device.h"
#include "vulkan_core.h"
#include "glfw3.h"

typedef struct SwapchainDetails {
    VkSwapchainKHR swapchain;
    VkImage* images;
    VkImageView* imageViews;
    uint32_t imagesLength;
} SwapchainDetails;

SwapchainDetails createSwapchain(VkInstance vulkanInstance, VkSurfaceKHR surfaceHandler, Device* device, GLFWwindow* window);

#endif