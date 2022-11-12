#pragma once
#include "luna/graphics/vulkan/vulkan_defines.hpp"
#include "luna/graphics/vulkan/vk_wrappers/instance.hpp"
#include "luna/graphics/vulkan/vk_wrappers/device.hpp"
#include "luna/graphics/vulkan/error.hpp"
#include "luna/graphics/types.hpp"
#include "luna/io/dlloader.hpp"
#include "luna/log/log.hpp"
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <utility>
#include <atomic>
#include <unordered_map>
#include <array>
#include <map>
namespace luna {
namespace vulkan {
auto create_pool(Device& device, int queue_family) -> vk::CommandPool;
struct Buffer {
  vk::Buffer buffer = {};
  VmaAllocation alloc = {};
  bool valid = false;
  int gpu = -1;
};

struct Image {
  vk::Image image = {};
  VmaAllocation alloc = {};
  bool valid = false;
  gfx::ImageInfo info = {};
};

struct CommandBuffer {
  vk::CommandBuffer cmd = {};
  std::vector<vk::Semaphore> wait_sems = {};
  std::vector<vk::Semaphore> signal_sems = {};
  vk::Fence fence = {};
  int gpu = -1;
  bool valid = false;
  CommandBuffer* parent = nullptr;
};

struct Pipeline {
  vk::Pipeline pipeline;
  vk::PipelineBindPoint bind_point;
};

struct Descriptor {
  vk::DescriptorSet descriptor;
  int32_t pipeline;
};

struct GlobalResources {
  io::Dlloader vulkan_loader;
  Instance instance;
  std::vector<VmaAllocator> allocators;
  std::vector<Device> devices;
  std::vector<Buffer> buffers;
  std::vector<Image> images;
  std::vector<vk::SurfaceKHR> surfaces;
  std::vector<CommandBuffer> cmds;
  std::vector<Pipeline> pipelines;
  std::vector<Descriptor> descriptors;
  private:
    GlobalResources();
    ~GlobalResources();
    auto make_instance() -> void;
    friend GlobalResources& global_resources();
};

auto global_resources() -> GlobalResources&;

template<typename T>
auto find_valid_entry(T container) -> size_t {
  auto index = 0u;
  for(auto& a : container) {
    if(!a.valid) {return index;}
    index++;
  }
  LunaAssert(false, "Ran out of space!");
  return 0;
}

inline auto convert(luna::gfx::ImageFormat fmt) -> vk::Format {
  switch(fmt) {
    case gfx::ImageFormat::RGBA8: return vk::Format::eR8G8B8A8Srgb;
    case gfx::ImageFormat::BRGA8: return vk::Format::eB8G8R8A8Srgb;
    case gfx::ImageFormat::RGBA32F: return vk::Format::eR32G32B32A32Sfloat;
    default: return vk::Format::eR8G8B8A8Srgb;
  }
}

inline auto convert(vk::Format fmt) -> gfx::ImageFormat {
  switch(fmt) {
    case vk::Format::eR8G8B8A8Srgb: return gfx::ImageFormat::RGBA8;
    case vk::Format::eB8G8R8A8Srgb: return gfx::ImageFormat::BRGA8;
    case vk::Format::eR32G32B32A32Sfloat: return gfx::ImageFormat::RGBA32F;
    default: return gfx::ImageFormat::RGBA8;
  }
}

inline auto create_cmd(int gpu, CommandBuffer* parent = nullptr) -> int32_t {
  auto& res = global_resources();
  auto index = find_valid_entry(res.cmds);
  auto& cmd = res.cmds[index];
  auto& device = res.devices[gpu];
  auto pool = create_pool(device, device.graphics().id);
  auto info = vk::CommandBufferAllocateInfo();
  auto fence_info = vk::FenceCreateInfo();
  info.setCommandBufferCount(1);
  info.setCommandPool(pool);
  info.setLevel(parent ? vk::CommandBufferLevel::eSecondary : vk::CommandBufferLevel::ePrimary);
  cmd.cmd = error(device.gpu.allocateCommandBuffers(info, device.m_dispatch)).data()[0];
  cmd.fence = error(device.gpu.createFence(fence_info, device.allocate_cb, device.m_dispatch));
  cmd.valid = true;
  return index;
}

inline auto destroy_cmd(int32_t handle) {
  auto& res = global_resources();
  auto& cmd = res.cmds[handle];
  cmd.valid = false;
}

inline auto create_image(gfx::ImageInfo& in_info, vk::Image import) -> int32_t {
  auto& res = luna::vulkan::global_resources();
  auto index = luna::vulkan::find_valid_entry(res.images);
  auto& image = res.images[index];
  image.info = in_info;
  image.image = import;
  image.valid = true;
  return index;
}

inline auto create_image(gfx::ImageInfo& in_info, const unsigned char*) -> int32_t {
  auto& res = luna::vulkan::global_resources();
  auto& allocator = res.allocators[in_info.gpu];
  auto info = vk::ImageCreateInfo();
  auto alloc_info = VmaAllocationCreateInfo{};
  auto index = luna::vulkan::find_valid_entry(res.images);
  auto& image = res.images[index];

  luna::log_debug("Vulkan -> Creating image on gpu ", in_info.gpu, " with width ", in_info.width, " and height ", in_info.height);
  info.extent = vk::Extent3D{static_cast<unsigned>(in_info.width), static_cast<unsigned>(in_info.height)};
  info.arrayLayers = in_info.layers;
  info.format = luna::vulkan::convert(in_info.format);
  info.imageType = vk::ImageType::e2D;
  info.initialLayout = vk::ImageLayout::eUndefined;
  info.mipLevels = in_info.num_mips;
  alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

  auto& c_info = static_cast<VkImageCreateInfo&>(info);
  auto c_image = static_cast<VkImage>(image.image);
  vmaCreateImage(allocator, &c_info, &alloc_info, &c_image, &image.alloc, nullptr);
  image.info = in_info;
  image.image = c_image;
  image.valid = true;
  return index;
}

inline auto destroy_image(int32_t handle) -> void {
  auto& res  = luna::vulkan::global_resources();
  auto& img = res.images[handle];
  auto c_img = static_cast<VkImage>(img.image);
  vmaDestroyImage(res.allocators[img.info.gpu], c_img, img.alloc);
  img.valid = false;
}
}
}