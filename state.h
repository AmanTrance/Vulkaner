#ifndef STATE_H
#define STATE_H

#include "vulkan_core.h"
#include "glfw3.h"

typedef struct VulkanerStateMachine
{
    uint32_t height;
    uint32_t width;
    uint32_t imagesLength;
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
    VkPipelineShaderStageCreateInfo pipelineShaderCreateInfo[2];
} VulkanerStateMachine;

void destroyStateMachine(VulkanerStateMachine* stateMachine);

#endif