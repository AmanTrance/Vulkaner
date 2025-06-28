#ifndef INSTANCE_H
#define INSTANCE_H

#include "vulkan_core.h"

VkInstance createVulkanInstance(const char* applicationName, const char** const extensions, uint32_t extensionsLength);

#endif