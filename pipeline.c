#include "pipeline.h"

#include <stdlib.h>
#include <unistd.h>

void createGraphicsPipeline(VulkanerStateMachine *stateMachine)
{
    VkDynamicState dynamicStates[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo pipelineState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates = dynamicStates};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 0,
        .vertexAttributeDescriptionCount = 0};

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE};

    VkPipelineViewportStateCreateInfo viewportState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount = 1};

    VkPipelineRasterizationStateCreateInfo rasterizer = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .lineWidth = 1.0f,
        .cullMode = VK_CULL_MODE_NONE,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f};

    VkPipelineMultisampleStateCreateInfo multisampling = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .sampleShadingEnable = VK_FALSE,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    };

    VkPipelineColorBlendStateCreateInfo colorBlending = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .attachmentCount = 0,
    };

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    };

    VkPipelineLayout pipelineLayout;
    if (
        vkCreatePipelineLayout(
            stateMachine->logicalDevice,
            &pipelineLayoutCreateInfo,
            NULL,
            &pipelineLayout) != VK_SUCCESS)
    {
        exit(1);
    }

    VkAttachmentDescription colorAttachment = {
        .format = VK_FORMAT_B8G8R8A8_SRGB,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR};

    VkAttachmentReference colorAttachmentReference = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentReference};

    VkRenderPassCreateInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &colorAttachment,
        .subpassCount = 1,
        .pSubpasses = &subpass};

    VkRenderPass renderPass;
    if (
        vkCreateRenderPass(
            stateMachine->logicalDevice,
            &renderPassInfo,
            NULL,
            &renderPass) != VK_SUCCESS)
    {
        exit(1);
    }

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = stateMachine->pipelineShaderCreateInfo,
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &inputAssembly,
        .pViewportState = &viewportState,
        .pRasterizationState = &rasterizer,
        .pMultisampleState = &multisampling,
        .pColorBlendState = &colorBlending,
        .pDynamicState = &pipelineState,
        .layout = pipelineLayout,
        .renderPass = renderPass,
        .subpass = 0};

    VkPipeline graphicsPipeline;
    if (
        vkCreateGraphicsPipelines(
            stateMachine->logicalDevice,
            VK_NULL_HANDLE,
            1,
            &graphicsPipelineCreateInfo,
            NULL,
            &graphicsPipeline) != VK_SUCCESS)
    {
        exit(1);
    }

    VkFramebuffer framebuffers[stateMachine->imagesLength];
    for (uint32_t i = 0; i < stateMachine->imagesLength; i++)
    {
        VkImageView attachments[] = {
            stateMachine->imageViews[i]};

        VkFramebufferCreateInfo framebufferInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = renderPass,
            .attachmentCount = 1,
            .pAttachments = attachments,
            .width = stateMachine->width,
            .height = stateMachine->height,
            .layers = 1,
        };

        if (
            vkCreateFramebuffer(
                stateMachine->logicalDevice,
                &framebufferInfo,
                NULL,
                &framebuffers[i]) != VK_SUCCESS)
        {
            exit(1);
        }
    }

    VkCommandPoolCreateInfo poolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = 0};

    VkCommandPool commandPool;
    if (
        vkCreateCommandPool(
            stateMachine->logicalDevice,
            &poolInfo,
            NULL,
            &commandPool) != VK_SUCCESS)
    {
        exit(1);
    }

    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1};

    VkCommandBuffer commandBuffer;
    if (
        vkAllocateCommandBuffers(
            stateMachine->logicalDevice,
            &allocInfo,
            &commandBuffer) != VK_SUCCESS)
    {
        exit(1);
    }

    VkSemaphoreCreateInfo semaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

    VkFenceCreateInfo fenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT};

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    if (
        vkCreateSemaphore(stateMachine->logicalDevice, &semaphoreInfo, NULL, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(stateMachine->logicalDevice, &semaphoreInfo, NULL, &renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(stateMachine->logicalDevice, &fenceInfo, NULL, &inFlightFence) != VK_SUCCESS)
    {
        exit(1);
    }

    while (!glfwWindowShouldClose(stateMachine->window))
    {
        glfwPollEvents();
        vkWaitForFences(stateMachine->logicalDevice,
                        1,
                        &inFlightFence,
                        VK_TRUE,
                        UINT64_MAX);
        vkResetFences(stateMachine->logicalDevice,
                      1,
                      &inFlightFence);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(
            stateMachine->logicalDevice,
            stateMachine->swapchain,
            UINT64_MAX,
            imageAvailableSemaphore,
            VK_NULL_HANDLE,
            &imageIndex);

        vkResetCommandBuffer(commandBuffer, 0);

        VkCommandBufferBeginInfo beginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        };

        if (
            vkBeginCommandBuffer(
                commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            exit(1);
        }

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

        VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = renderPass,
            .framebuffer = framebuffers[imageIndex],
            .renderArea = {
                .offset = {0, 0},
                .extent = {
                    .height = stateMachine->height,
                    .width = stateMachine->width}},
            .clearValueCount = 1,
            .pClearValues = &clearColor};

        vkCmdBeginRenderPass(
            commandBuffer,
            &renderPassBeginInfo,
            VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            graphicsPipeline);

        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = stateMachine->width,
            .height = stateMachine->height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f};

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor = {
            .offset = {0, 0},
            .extent = {
                .height = stateMachine->height,
                .width = stateMachine->width}};

        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(commandBuffer);

        if (
            vkEndCommandBuffer(
                commandBuffer) != VK_SUCCESS)
        {
            exit(1);
        }

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = waitSemaphores,
            .pWaitDstStageMask = waitStages,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = signalSemaphores,
        };

        if (
            vkQueueSubmit(
                stateMachine->deviceQueue,
                1,
                &submitInfo,
                inFlightFence) != VK_SUCCESS)
        {
            exit(1);
        }

        VkSwapchainKHR swapChains[] = {stateMachine->swapchain};

        VkPresentInfoKHR presentInfo = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = signalSemaphores,
            .swapchainCount = 1,
            .pSwapchains = swapChains,
            .pImageIndices = &imageIndex};

        vkQueuePresentKHR(
            stateMachine->deviceQueue,
            &presentInfo);
    }
}