#include "vkviz.h"
#include "vizgen.h"
#include <algorithm>

MainWindow* VkViz::window_ = nullptr;

VkResult VkViz::PostCallBeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) {
    GetCommandBuffer(commandBuffer).Begin();
}

VkResult VkViz::PostCallEndCommandBuffer(VkCommandBuffer commandBuffer) { GetCommandBuffer(commandBuffer).End(); }

VkResult VkViz::PostCallResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) {
    GetCommandBuffer(commandBuffer).Reset();
}

VkResult VkViz::PostCallAllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo,
                                               VkCommandBuffer* pCommandBuffers) {
    AddCommandBuffers(pAllocateInfo, pCommandBuffers);
}

void VkViz::PostCallFreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount,
                                       const VkCommandBuffer* pCommandBuffers) {
    RemoveCommandBuffers(commandBufferCount, pCommandBuffers);
}

VkResult VkViz::PostCallCreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo,
                                         const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {
    AddRenderPass(device, pCreateInfo, pRenderPass);
}

void VkViz::PostCallDestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator) {
    RemoveRenderPass(device, renderPass);
}

VkResult VkViz::PostCallQueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) {
    for (uint32_t i = 0; i < submitCount; ++i) {
        for (uint32_t j = 0; j < pSubmits[i].commandBufferCount; ++j) {
            window_->AddCommandBuffer(GetCommandBuffer(pSubmits[i].pCommandBuffers[i]));
        }
    }
}

VkResult VkViz::PostCallQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {
    out_file_ << "Frame: " << current_frame_ << std::endl;
    current_frame_++;
}

// An instance needs to be declared to turn on a layer in the layer_factory framework
VkViz vkviz_layer;
