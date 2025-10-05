#include "swapchain.h"

#include "vulkan_core.h"
#include "glfw3.h"

#include <stdio.h>
#include <stdlib.h>

void createSwapchain(VulkanerStateMachine *stateMachine)
{
    int width;
    int height;

    glfwGetFramebufferSize(
        stateMachine->window,
        &width,
        &height);

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    if (
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            stateMachine->physicalDevice,
            stateMachine->surfaceHandle,
            &surfaceCapabilities) != VK_SUCCESS)
    {
        exit(1);
    }

    uint32_t presentModesCount;
    if (
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            stateMachine->physicalDevice,
            stateMachine->surfaceHandle,
            &presentModesCount,
            NULL) != VK_SUCCESS)
    {
        exit(1);
    }

    VkPresentModeKHR presentModes[presentModesCount];
    if (
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            stateMachine->physicalDevice,
            stateMachine->surfaceHandle,
            &presentModesCount,
            presentModes) != VK_SUCCESS)
    {
        exit(1);
    }

    VkPresentModeKHR presentModeToUse;
    for (int i = 0; i < presentModesCount; i++)
    {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            presentModeToUse = VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    if (
        presentModeToUse == 0)
    {
        presentModeToUse = VK_PRESENT_MODE_FIFO_KHR;
    }

    uint32_t formatCounts;
    if (
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            stateMachine->physicalDevice,
            stateMachine->surfaceHandle,
            &formatCounts,
            NULL) != VK_SUCCESS)
    {
        exit(1);
    }

    VkSurfaceFormatKHR formats[formatCounts];
    if (
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            stateMachine->physicalDevice,
            stateMachine->surfaceHandle,
            &formatCounts,
            formats) != VK_SUCCESS)
    {
        exit(1);
    }

    VkFormat formatToUse;
    VkColorSpaceKHR colorSpaceToUse;

    for (uint32_t i = 0; i < formatCounts; i++)
    {
        if (((formats[i].format & VK_FORMAT_B8G8R8A8_SRGB) == VK_FORMAT_B8G8R8A8_SRGB))
        {
            formatToUse = VK_FORMAT_B8G8R8A8_SRGB;
            colorSpaceToUse = formats[i].colorSpace;
        }
    }

    if (
        formatToUse == 0 && formatCounts > 0)
    {
        formatToUse = formats[0].format;
        colorSpaceToUse = formats[0].colorSpace;
    }

    const VkSwapchainCreateInfoKHR swapchainCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,
        .oldSwapchain = NULL,
        .surface = stateMachine->surfaceHandle,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = NULL,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentModeToUse,
        .imageFormat = formatToUse,
        .clipped = VK_TRUE,
        .imageExtent = {
            .height = height,
            .width = width},
        .minImageCount = surfaceCapabilities.minImageCount,
        .imageUsage = surfaceCapabilities.supportedUsageFlags,
        .flags = VK_SWAPCHAIN_CREATE_PROTECTED_BIT_KHR,
        .imageArrayLayers = 1,
        .imageColorSpace = colorSpaceToUse};

    if (
        vkCreateSwapchainKHR(
            stateMachine->logicalDevice,
            &swapchainCreateInfo,
            NULL,
            &stateMachine->swapchain) != VK_SUCCESS)
    {
        exit(1);
    }

    if (
        vkGetSwapchainImagesKHR(
            stateMachine->logicalDevice,
            stateMachine->swapchain,
            &stateMachine->imagesLength,
            NULL) != VK_SUCCESS)
    {
        exit(1);
    }

    stateMachine->images = (VkImage *)malloc(
        sizeof(VkImage) * stateMachine->imagesLength);
    if (
        vkGetSwapchainImagesKHR(
            stateMachine->logicalDevice,
            stateMachine->swapchain,
            &stateMachine->imagesLength,
            stateMachine->images) != VK_SUCCESS)
    {
        exit(1);
    }

    stateMachine->imageViews = (VkImageView *)malloc(
        sizeof(VkImageView) * stateMachine->imagesLength);

    for (int i = 0; i < stateMachine->imagesLength; i++)
    {
        const VkImageViewCreateInfo imageViewCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = NULL,
            .image = stateMachine->images[i],
            .format = formatToUse,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .components = {
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .r = VK_COMPONENT_SWIZZLE_IDENTITY},
            .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseArrayLayer = 0, .baseMipLevel = 0, .layerCount = 1, .levelCount = 1}};

        if (
            vkCreateImageView(
                stateMachine->logicalDevice,
                &imageViewCreateInfo,
                NULL,
                &stateMachine->imageViews[i]) != VK_SUCCESS)
        {
            exit(1);
        }
    }
}