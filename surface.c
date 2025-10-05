#include "surface.h"

#include "vulkan_core.h"
#include "vulkan.h"
#include "glfw3.h"

#include <stdio.h>
#include <stdlib.h>

void createSurface(VulkanerStateMachine *stateMachine)
{
    if (
        glfwCreateWindowSurface(
            stateMachine->vulkanInstance,
            stateMachine->window,
            NULL,
            &stateMachine->surfaceHandle) != VK_SUCCESS)
    {
        printf("Window Surface Creation Failed\n");
        exit(1);
    }
}