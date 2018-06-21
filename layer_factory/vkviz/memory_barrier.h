#ifndef MEMORY_BARRIER_H
#define MEMORY_BARRIER_H

#include <vulkan.h>
#include <fstream>

#include "device.h"

class MemoryBarrier {
   protected:
    VkAccessFlags src_access_mask_;
    VkAccessFlags dst_access_mask_;

    virtual ~MemoryBarrier() = default;

   public:

    virtual void Log(std::ofstream& out_file) const {
        out_file << "  Memory barrier" << std::endl;
        out_file << "    Source access mask: " << src_access_mask_;
        out_file << "    Destination access mask: " << dst_access_mask_;
    }
};

class ImageMemoryBarrier : public MemoryBarrier {
    VkImage image_;
    VkImageSubresourceRange subresource_range_;

   public:
    ImageMemoryBarrier(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImage image,
                       VkImageSubresourceRange subresource_range)
        : image_(image), subresource_range_(subresource_range) {
        src_access_mask_ = srcAccessMask;
        dst_access_mask_ = dstAccessMask;
    }

    void Log(std::ofstream& out_file) const override {
        out_file << "    Is an image barrier" << std::endl;
        out_file << "    Image handle: " << image_ << std::endl;
    }
};

class BufferMemoryBarrier : public MemoryBarrier {
    VkBuffer buffer_;
    VkVizMemoryRegion memory_region_;

   public:
    BufferMemoryBarrier(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size) : BufferMemoryBarrier(srcAccessMask, dstAccessMask, buffer, {offset, size}) {}

    BufferMemoryBarrier(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkBuffer buffer, VkVizMemoryRegion memory_region)
        : buffer_(buffer), memory_region_(memory_region) {
        src_access_mask_ = srcAccessMask;
        dst_access_mask_ = dstAccessMask;
    }

    void Log(std::ofstream& out_file) const override {
        out_file << "    Is a buffer barrier" << std::endl;
        out_file << "    Buffer handle: " << buffer_ << std::endl;
    }
};

class GlobalMemoryBarrier : public MemoryBarrier {
   public:
    GlobalMemoryBarrier(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask) {
        src_access_mask_ = srcAccessMask;
        dst_access_mask_ = dstAccessMask;
    }

    void Log(std::ofstream& out_file) const override {
        out_file << "    Is a global barrier" << std::endl;
    }
};

#endif  // MEMORY_BARRIER_H
