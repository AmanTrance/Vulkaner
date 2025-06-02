#ifndef SURFACE_H
#define SURFACE_H

#include "device.h"
#include "vulkan_core.h"
#include "glfw3.h"

VkSurfaceKHR setup_surface(VkInstance vulkan_instance, Device* device, GLFWwindow* window);

#endif