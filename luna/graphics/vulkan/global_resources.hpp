#pragma once
#include "luna/graphics/vulkan/vulkan_defines.hpp"
#include "luna/graphics/vulkan/vk_wrappers/instance.hpp"
#include "luna/graphics/vulkan/vk_wrappers/device.hpp"
#include "luna/graphics/vulkan/vk_wrappers/pipeline.hpp"
#include "luna/graphics/vulkan/vk_wrappers/descriptor.hpp"
#include "luna/graphics/vulkan/vk_wrappers/render_pass.hpp"
#include "luna/graphics/vulkan/vk_wrappers/swapchain.hpp"
#include "luna/graphics/vulkan/window/window.hpp"
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
inline auto begin_command_buffer(int32_t handle) -> void;
inline auto end_command_buffer(int32_t handle) -> void;
inline auto transition_image(int32_t cmd_id, int32_t image_id, vk::ImageLayout layout) -> void;
inline auto copy_buffer_to_image(int32_t cmd_id, int32_t buffer_id, int32_t image_id) -> void;
inline auto submit_command_buffer(int32_t handle) -> void;
inline auto synchronize(int32_t cmd_buffer) -> void;

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
  std::vector<Swapchain> swapchains;
  std::vector<Window> windows;
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
    case gfx::ImageFormat::BGRA8: return vk::Format::eB8G8R8A8Srgb;
    case gfx::ImageFormat::RGBA32F: return vk::Format::eR32G32B32A32Sfloat;
    default: return vk::Format::eR8G8B8A8Srgb;
  }
}

inline auto convert(vk::Format fmt) -> gfx::ImageFormat {
  switch(fmt) {
    case vk::Format::eR8G8B8A8Srgb: return gfx::ImageFormat::RGBA8;
    case vk::Format::eB8G8R8A8Srgb: return gfx::ImageFormat::BGRA8;
    case vk::Format::eR32G32B32A32Sfloat: return gfx::ImageFormat::RGBA32F;
    default: return gfx::ImageFormat::RGBA8;
  }
}

inline auto make_mappable_buffer(int gpu, size_t size) -> int32_t {
  auto& res  = luna::vulkan::global_resources();
  auto info = vk::BufferCreateInfo();
  auto alloc_info = VmaAllocationCreateInfo{};
  auto index = luna::vulkan::find_valid_entry(res.buffers);
  auto& buffer = res.buffers[index];

  luna::log_debug("Vulkan -> Creating mappable buffer on gpu ", gpu, " with size ", size);
  buffer.gpu = gpu;
  alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
  alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
  info.size = size;
  info.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eStorageBuffer;

  auto& c_info = static_cast<VkBufferCreateInfo&>(info);
  auto c_buffer = static_cast<VkBuffer>(buffer.buffer);
  vmaCreateBuffer(res.allocators[gpu], &c_info, &alloc_info, &c_buffer, &buffer.alloc, nullptr);
  buffer.buffer = c_buffer;
  vmaGetAllocationInfo(res.allocators[gpu], buffer.alloc, &buffer.info);
  LunaAssert(c_buffer, "Could not create buffer given input parameters.");
  return index;
}

inline auto map_buffer(int32_t buffer_id, void** ptr) -> void {
  auto& res = global_resources();
  auto& buffer = res.buffers[buffer_id];
  auto& gpu = res.devices[buffer.gpu];
  auto size = vk::DeviceSize(buffer.info.size);
  auto flags = vk::MemoryMapFlags();
  error(gpu.gpu.mapMemory(buffer.info.deviceMemory, buffer.info.offset, size, flags, ptr, gpu.m_dispatch));
}

inline auto unmap_buffer(int32_t buffer_id) -> void {
  auto& res = global_resources();
  auto& buffer = res.buffers[buffer_id];
  auto& gpu = res.devices[buffer.gpu];
  gpu.gpu.unmapMemory(buffer.info.deviceMemory, gpu.m_dispatch);
}

inline auto destroy_buffer(int32_t handle) -> void {
  auto& res  = luna::vulkan::global_resources();
  auto& buffer = res.buffers[handle];
  auto c_buffer = static_cast<VkBuffer>(buffer.buffer);
  
  luna::log_debug("Vulkan -> Creating destroying buffer ", handle);
  LunaAssert(handle >= 0, "Attempting to use an invalid image.");
  vmaDestroyBuffer(res.allocators[buffer.gpu], c_buffer, buffer.alloc);
  buffer.buffer = nullptr;
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

inline auto upload_data_to_image(int32_t image_id, gfx::ImageInfo& info, const unsigned char* initial_data) -> void {
    auto& res = global_resources();
    auto num_channels = 4;
    auto element_size = sizeof(unsigned char);
    switch(info.format) {
      case gfx::ImageFormat::RGBA32F:
        element_size = sizeof(float);
      case gfx::ImageFormat::RGBA8:
      case gfx::ImageFormat::BGRA8:
      default: break;
    }

    auto size = info.width * info.height * num_channels * element_size;
    auto tmp_buffer = make_mappable_buffer(info.gpu, size);
    auto tmp_cmd = create_cmd(info.gpu, nullptr);
    auto cmd = res.cmds[tmp_cmd];

    void* ptr = nullptr;
    map_buffer(tmp_buffer, &ptr);
    std::memcpy(ptr, initial_data, size);
    unmap_buffer(tmp_buffer);

    begin_command_buffer(tmp_cmd);
    copy_buffer_to_image(tmp_cmd, tmp_buffer, image_id);
    end_command_buffer(tmp_cmd);
    submit_command_buffer(tmp_cmd);
    synchronize(tmp_cmd);
}


inline auto create_image(gfx::ImageInfo& in_info, vk::ImageLayout layout, vk::ImageUsageFlags usage, vk::Image import) -> int32_t {
  auto& res = luna::vulkan::global_resources();
  auto index = luna::vulkan::find_valid_entry(res.images);
  auto& gpu = res.devices[in_info.gpu];
  auto& image = res.images[index];
  image.info = in_info;
  image.image = import;
  image.imported = true;
  image.layout = layout;
  image.format = convert(in_info.format);
  image.usage = usage;

  if (in_info.is_cubemap) {
    image.view_type = vk::ImageViewType::eCube;
  } else {
    switch (in_info.layers) {
      case 0:
        image.view_type = vk::ImageViewType::e1D;
        break;
      case 1:
        image.view_type = vk::ImageViewType::e2D;
        break;
      default:
        image.view_type = vk::ImageViewType::e2DArray;
        break;
    }
  }

  image.subresource.setAspectMask(vk::ImageAspectFlagBits::eColor);
  image.subresource.setBaseArrayLayer(0);
  image.subresource.setLayerCount(in_info.layers);
  image.subresource.setMipLevel(0);

  image.layout = layout;
  image.usage = usage;

  create_sampler(gpu, image);
  create_image_view(gpu, image);
  return index;
}

inline auto create_image(gfx::ImageInfo& in_info, vk::ImageLayout layout, vk::ImageUsageFlags usage, const unsigned char* initial_data) -> int32_t {
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

  image.subresource.setAspectMask(vk::ImageAspectFlagBits::eColor);
  image.subresource.setBaseArrayLayer(0);
  image.subresource.setLayerCount(in_info.layers);
  image.subresource.setMipLevel(0);

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
  
  if(initial_data != nullptr) {
    upload_data_to_image(index, in_info, initial_data);
  }
  return index;
}

inline auto destroy_image(int32_t handle) -> void {
  auto& res  = luna::vulkan::global_resources();
  auto& img = res.images[handle];
  if(img.imported) {
    img.imported = false;
    img.image = nullptr;
    return;
  }
  auto c_img = static_cast<VkImage>(img.image);
  vmaDestroyImage(res.allocators[img.info.gpu], c_img, img.alloc);
  img.image = nullptr;
}

inline auto begin_command_buffer(int32_t handle) -> void {
  LunaAssert(handle >= 0, "Attempting to use an invalid command buffer.");
  auto& cmd = luna::vulkan::global_resources().cmds[handle];
  auto& gpu = luna::vulkan::global_resources().devices[cmd.gpu];
  luna::vulkan::error(cmd.cmd.begin(cmd.begin_info, gpu.m_dispatch));
}

inline auto end_command_buffer(int32_t handle) -> void {
  LunaAssert(handle >= 0, "Attempting to use an invalid command buffer.");
  auto& cmd = luna::vulkan::global_resources().cmds[handle];
  auto& gpu = luna::vulkan::global_resources().devices[cmd.gpu];
  luna::vulkan::error(cmd.cmd.end(gpu.m_dispatch));
}

inline auto synchronize(int32_t handle) -> void {
  LunaAssert(handle >= 0, "Attempting to use an invalid command buffer.");
  auto& cmd = luna::vulkan::global_resources().cmds[handle];
  auto& gpu = luna::vulkan::global_resources().devices[cmd.gpu];
  
  if(cmd.fence && cmd.signaled) {
    error(gpu.gpu.waitForFences(1, &cmd.fence, true, UINT64_MAX, gpu.m_dispatch));
    error(gpu.gpu.resetFences(1, &cmd.fence, gpu.m_dispatch));
    cmd.signaled = false;
  }
}

inline auto submit_command_buffer(int32_t handle) -> void {
  auto& cmd = luna::vulkan::global_resources().cmds[handle];
  auto& gpu = luna::vulkan::global_resources().devices[cmd.gpu];
  auto& info = cmd.submit_info;
  auto& queue = gpu.graphics();

  auto vector = std::vector<vk::Semaphore>();
  auto signal_sems = std::vector<vk::Semaphore>();
  auto masks = std::vector<vk::PipelineStageFlags>();
  
  if(cmd.fence && cmd.signaled) {
    error(gpu.gpu.waitForFences(1, &cmd.fence, true, UINT64_MAX, gpu.m_dispatch));
    error(gpu.gpu.resetFences(1, &cmd.fence, gpu.m_dispatch));
  }

  info.setCommandBufferCount(1);
  info.setPCommandBuffers(&cmd.cmd);
  info.setWaitSemaphores(cmd.wait_sems);
  info.setSignalSemaphores(cmd.signal_sems);
  info.setWaitDstStageMask(masks);

  error(queue.queue.submit(1, &info, cmd.fence, gpu.m_dispatch));
  cmd.signaled = true;
}

inline auto transition_image(int32_t cmd_id, int32_t image_id, vk::ImageLayout layout) -> void {
  auto& res = global_resources();
  auto& image = res.images[image_id];
  auto& cmd = res.cmds[cmd_id];
  auto& gpu = res.devices[image.info.gpu];

  auto range = vk::ImageSubresourceRange();
  auto src = vk::PipelineStageFlags();
  auto dst = vk::PipelineStageFlags();
  auto dep_flags = vk::DependencyFlags();
  auto new_layout = vk::ImageLayout();
  auto old_layout = vk::ImageLayout();

  new_layout = layout;
  old_layout = image.layout;

  range.setBaseArrayLayer(0);
  range.setBaseMipLevel(0);
  range.setLevelCount(1);
  range.setLayerCount(image.info.layers);
  if (image.format == vk::Format::eD24UnormS8Uint)
    range.setAspectMask(vk::ImageAspectFlagBits::eDepth |
                        vk::ImageAspectFlagBits::eStencil);
  else
    range.setAspectMask(vk::ImageAspectFlagBits::eColor);

  image.barrier.setOldLayout(old_layout);
  image.barrier.setNewLayout(new_layout);
  image.barrier.setImage(image.image);
  image.barrier.setSubresourceRange(range);
  image.barrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
  image.barrier.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);

  dep_flags = vk::DependencyFlags();
  src = vk::PipelineStageFlagBits::eTopOfPipe;
  dst = vk::PipelineStageFlagBits::eBottomOfPipe;

  LunaAssert(new_layout != vk::ImageLayout::eUndefined, "Attempting to transition an image to an undefined layout, which is not possible");
  cmd.cmd.pipelineBarrier(src, dst, dep_flags, 0, nullptr, 0, nullptr, 1, &image.barrier, gpu.m_dispatch);
  image.layout = new_layout;
}

inline auto copy_buffer_to_image(int32_t cmd_id, int32_t buffer_id, int32_t image_id) -> void {
  auto& res = global_resources();
  auto& src = res.buffers[buffer_id];
  auto& dst = res.images[image_id];
  auto& cmd = res.cmds[cmd_id];
  auto& gpu = res.devices[src.gpu];

  auto info = vk::BufferImageCopy();
  auto extent = vk::Extent3D();

  extent.setWidth(dst.info.width);
  extent.setHeight(dst.info.height);
  extent.setDepth(1);

  info.setImageExtent(extent);
  info.setBufferImageHeight(0);
  info.setBufferRowLength(0);
  info.setImageOffset(0);
  info.setImageSubresource(dst.subresource);

  auto dst_old_layout = dst.layout;

  // Need to handle layout transitions because we're copying
  if (dst.layout != vk::ImageLayout::eTransferDstOptimal)
    transition_image(cmd_id, image_id, vk::ImageLayout::eTransferDstOptimal);

  cmd.cmd.copyBufferToImage(src.buffer, dst.image, vk::ImageLayout::eTransferDstOptimal, 1, &info, gpu.m_dispatch);

  if (dst_old_layout != vk::ImageLayout::eUndefined)
    transition_image(cmd_id, image_id, dst_old_layout);
}
}
}