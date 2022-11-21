#pragma once
#include "luna/graphics/vulkan/vulkan_defines.hpp"
#include "luna/graphics/vulkan/vk_wrappers/instance.hpp"
#include "luna/graphics/vulkan/vk_wrappers/device.hpp"
#include "luna/graphics/vulkan/vk_wrappers/pipeline.hpp"
#include "luna/graphics/vulkan/vk_wrappers/descriptor.hpp"
#include "luna/graphics/vulkan/vk_wrappers/render_pass.hpp"
#include "luna/graphics/vulkan/vk_wrappers/data_types.hpp"
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
class Descriptor;
class Pipeline;

auto create_pool(Device& device, int queue_family) -> vk::CommandPool;

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
  std::vector<RenderPass> render_passes;
  private:
    GlobalResources();
    ~GlobalResources();
    auto make_instance() -> void;
    friend GlobalResources& global_resources();
};

auto global_resources() -> GlobalResources&;

template<typename T>
auto find_valid_entry(const T& container) -> size_t {
  auto index = 0u;
  for(const auto& a : container) {
    if(!a.valid()) {return index;}
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
  cmd.gpu = gpu;
  return index;
}

inline auto destroy_cmd(int32_t handle) {
  auto& res = global_resources();
  auto& cmd = res.cmds[handle];
  cmd.cmd = nullptr;
}

inline auto create_image(gfx::ImageInfo& in_info, vk::Image import) -> int32_t {
  auto& res = luna::vulkan::global_resources();
  auto index = luna::vulkan::find_valid_entry(res.images);
  auto& image = res.images[index];
  image.info = in_info;
  image.image = import;

  return index;
}

inline auto create_sampler(luna::vulkan::Device& device, luna::vulkan::Image& img) {
  const auto max_anisotropy = 16.0f;
  auto info = vk::SamplerCreateInfo();
  info.setMagFilter(vk::Filter::eNearest);
  info.setMinFilter(vk::Filter::eNearest);
  info.setAddressModeU(vk::SamplerAddressMode::eRepeat);
  info.setAddressModeV(vk::SamplerAddressMode::eRepeat);
  info.setAddressModeW(vk::SamplerAddressMode::eRepeat);
  info.setBorderColor(vk::BorderColor::eIntTransparentBlack);
  info.setCompareOp(vk::CompareOp::eNever);
  info.setMipmapMode(vk::SamplerMipmapMode::eLinear);
  info.setAnisotropyEnable(vk::Bool32(false));
  info.setUnnormalizedCoordinates(vk::Bool32(false));
  info.setCompareEnable(vk::Bool32(false));
  info.setMaxAnisotropy(max_anisotropy);
  info.setMipLodBias(0.0f);
  info.setMinLod(0.0f);
  info.setMaxLod(0.0f);

  img.sampler = luna::vulkan::error(device.gpu.createSampler(info, device.allocation_cb(), device.m_dispatch));
}

inline auto create_image_view(luna::vulkan::Device& device, luna::vulkan::Image& img) -> void {
  auto info = vk::ImageViewCreateInfo();
  auto range = vk::ImageSubresourceRange();

  if (img.info.format == luna::gfx::ImageFormat::Depth) {
    range.setAspectMask(vk::ImageAspectFlagBits::eDepth |
                        vk::ImageAspectFlagBits::eStencil);
  } else {
    range.setAspectMask(vk::ImageAspectFlagBits::eColor);
  }

  range.setBaseArrayLayer(0);
  range.setBaseMipLevel(0);
  range.setLayerCount(img.info.layers);
  range.setLevelCount(1);

  info.setImage(img.image);
  info.setViewType(img.view_type);  //@JH TODO Make configurable.
  info.setFormat(img.format);
  info.setSubresourceRange(range);

  img.view = luna::vulkan::error(device.gpu.createImageView(info, device.allocate_cb, device.m_dispatch));
}

inline auto create_image(gfx::ImageInfo& in_info, vk::ImageLayout layout, vk::ImageUsageFlags usage, const unsigned char*) -> int32_t {
  auto& res = luna::vulkan::global_resources();
  auto& allocator = res.allocators[in_info.gpu];
  auto& gpu = res.devices[in_info.gpu];
  auto info = vk::ImageCreateInfo();
  auto alloc_info = VmaAllocationCreateInfo{};
  auto index = luna::vulkan::find_valid_entry(res.images);
  auto& image = res.images[index];

  if (in_info.is_cubemap) {
    image.view_type = vk::ImageViewType::eCube;
    info.imageType = vk::ImageType::e2D;
  } else {
    switch (in_info.layers) {
      case 0:
        image.view_type = vk::ImageViewType::e1D;
        info.imageType = vk::ImageType::e1D;
        break;
      case 1:
        image.view_type = vk::ImageViewType::e2D;
        info.imageType = vk::ImageType::e2D;
        break;
      default:
        image.view_type = vk::ImageViewType::e2DArray;
        info.imageType = vk::ImageType::e2D;
        break;
    }
  }

  luna::log_debug("Vulkan -> Creating image on gpu ", in_info.gpu, " with width ", in_info.width, " and height ", in_info.height, " with ID ", index);
  info.extent = vk::Extent3D{static_cast<unsigned>(in_info.width), static_cast<unsigned>(in_info.height), 1};
  info.arrayLayers = in_info.layers;
  info.format = luna::vulkan::convert(in_info.format);
  info.initialLayout = vk::ImageLayout::eUndefined;
  info.mipLevels = in_info.num_mips;
  info.usage = usage;
  alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

  auto& c_info = static_cast<VkImageCreateInfo&>(info); 
  auto c_image = static_cast<VkImage>(image.image);
  vmaCreateImage(allocator, &c_info, &alloc_info, &c_image, &image.alloc, nullptr);
  LunaAssert(c_image, "Could not create image given input parameters.");
  image.info = in_info;
  image.image = c_image;
  image.layout = layout;
  image.format = info.format;
  image.usage = usage;

  create_sampler(gpu, image);
  create_image_view(gpu, image);
  return index;
}

inline auto destroy_image(int32_t handle) -> void {
  auto& res  = luna::vulkan::global_resources();
  auto& img = res.images[handle];
  auto c_img = static_cast<VkImage>(img.image);
  vmaDestroyImage(res.allocators[img.info.gpu], c_img, img.alloc);
  img.image = nullptr;
}
}
}