#ifndef INSTANCE_H
#define INSTANCE_H

#include "vulkan_core.h"

VkInstance create_vulkan_instance(const char* application_name, const char** const extensions, uint32_t extensions_length);

#endif