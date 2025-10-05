#ifndef STATE_H
#define STATE_H

#include "vulkan_core.h"
#include "glfw3.h"

typedef struct VulkanerStateMachine
{
    __uint32_t imagesLength;
    GLFWwindow *window;
    VkImage *images;
    VkImageView *imageViews;
    VkQueueFamilyProperties *queueFamilyProperties;
    VkInstance vulkanInstance;
    VkSurfaceKHR surfaceHandle;
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;
    VkQueue deviceQueue;
    VkSwapchainKHR swapchain;
} VulkanerStateMachine;

#endif