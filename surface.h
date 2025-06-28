#ifndef SURFACE_H
#define SURFACE_H

#include "device.h"
#include "vulkan_core.h"
#include "glfw3.h"

VkSurfaceKHR setupSurface(VkInstance vulkanInstance, GLFWwindow* window);

#endif