#ifndef STATE_H
#define STATE_H

#include "vulkan_core.h"
#include "glfw3.h"

typedef struct VulkanerStateMachine
{
    uint32_t width;
    uint32_t height;
    uint32_t imagesLength;

    VkImage *images;
    GLFWwindow *window;
    VkImageView *imageViews;
    VkQueueFamilyProperties *queueFamilyProperties;

    VkQueue deviceQueue;
    VkDevice logicalDevice;
    VkSwapchainKHR swapchain;
    VkInstance vulkanInstance;
    VkSurfaceKHR surfaceHandle;
    VkPhysicalDevice physicalDevice;

    VkPipelineShaderStageCreateInfo pipelineShaderCreateInfo[2];
} VulkanerStateMachine;

void destroyStateMachine(VulkanerStateMachine* stateMachine);

#endif