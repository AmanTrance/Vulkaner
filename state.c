#include "state.h"

#include <stdlib.h>

void destroyStateMachine(VulkanerStateMachine *stateMachine)
{
    free(stateMachine->imageViews);
    free(stateMachine->images);

    vkDestroySwapchainKHR(
        stateMachine->logicalDevice,
        stateMachine->swapchain,
        NULL);

    vkDestroyDevice(stateMachine->logicalDevice, NULL);
    vkDestroyInstance(stateMachine->vulkanInstance, NULL);

    glfwDestroyWindow(stateMachine->window);

    free(stateMachine->queueFamilyProperties);
}