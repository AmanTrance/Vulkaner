#include "state.h"
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
    if (!glfwInit()) {
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    VulkanerStateMachine stateMachine;
    stateMachine.window = glfwCreateWindow(1920, 1080, "Vulkaner", NULL, NULL);

    createVulkanInstance(&stateMachine, "Vulkaner");
    createDevice(&stateMachine);
    createSurface(&stateMachine);
    createSwapchain(&stateMachine);
    createShaderModules(&stateMachine);

    while (!glfwWindowShouldClose(stateMachine.window)) {
        glfwPollEvents();
    }

    destroyStateMachine(&stateMachine);
    glfwTerminate();

    return 0;
}