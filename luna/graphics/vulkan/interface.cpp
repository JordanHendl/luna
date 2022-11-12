#include "luna/graphics/vulkan/vulkan_defines.hpp"
#include "luna/graphics/vulkan/global_resources.hpp"
#include "luna/error/error.hpp"
#include "luna/graphics/types.hpp"
#include "luna/io/dlloader.hpp"
#include "luna/log/log.hpp"
#include <iostream>

namespace luna {
namespace vulkan {
auto make_devices() -> void {
  auto& res = global_resources();
  res.devices.reserve(res.instance.m_devices.size());
  res.allocators.resize(res.instance.m_devices.size());
  for (auto& p_device : res.instance.m_devices) {
    auto device = Device();
    device = std::move(Device(res.vulkan_loader, nullptr,
                        p_device, res.instance.m_dispatch));

    res.devices.emplace_back(std::move(device));
  }

  // Need to sort the devices by score now.
  auto compare = [](Device& a, Device& b) {
    return a.score() > b.score();
  };

  std::sort(res.devices.begin(), res.devices.end(), compare);

  //Now, we need to create the allocators for each valid device.
  for(auto index = 0u; index < res.devices.size(); index++) {
    auto alloc_create_info = VmaAllocatorCreateInfo();
    auto vulkan_funcs = VmaVulkanFunctions();
    auto sym = res.vulkan_loader.symbol("vkGetInstanceProcAddr");
    auto sym2 = res.vulkan_loader.symbol("vkGetDeviceProcAddr");
    vulkan_funcs = {};
    vulkan_funcs.vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(sym);
    vulkan_funcs.vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(sym2);

    alloc_create_info = {};
    alloc_create_info.vulkanApiVersion = VK_API_VERSION_1_2;
    alloc_create_info.physicalDevice = res.devices[index].physical_device;
    alloc_create_info.device = res.devices[index].gpu;
    alloc_create_info.pVulkanFunctions = &vulkan_funcs;
    alloc_create_info.instance = res.instance.m_instance;

    if(alloc_create_info.device && alloc_create_info.physicalDevice)
      vmaCreateAllocator(&alloc_create_info, &res.allocators[index++]);
  }
}
}
}

LUNA_C_API auto initialize() -> void {
  luna::log_debug("Vulkan -> Initializing Luna Implementation!");
  luna::vulkan::make_devices();

  for(auto& dev : luna::vulkan::global_resources().devices) {
    luna::log_debug("Vulkan -> GPU Found: ", dev.properties.deviceName.data());
  }
}

LUNA_C_API auto make_vertex_buffer(int gpu, size_t size) -> int32_t {
  auto& res  = luna::vulkan::global_resources();
  auto info = vk::BufferCreateInfo();
  auto alloc_info = VmaAllocationCreateInfo{};
  auto index = luna::vulkan::find_valid_entry(res.buffers);
  auto& buffer = res.buffers[index];
  
  luna::log_debug("Vulkan -> Creating vertex buffer on gpu ", gpu, " with size ", size);
  alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
  info.size = size;
  info.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eVertexBuffer;
  buffer.valid = true;
  buffer.gpu = gpu;
  auto& c_info = static_cast<VkBufferCreateInfo&>(info);
  auto c_buffer = static_cast<VkBuffer>(buffer.buffer);
  vmaCreateBuffer(res.allocators[gpu], &c_info, &alloc_info, &c_buffer, &buffer.alloc, nullptr);
  buffer.buffer = c_buffer;
  return index;
}

LUNA_C_API auto make_uniform_buffer(int gpu, size_t size) -> int32_t {
  auto& res  = luna::vulkan::global_resources();
  auto info = vk::BufferCreateInfo();
  auto alloc_info = VmaAllocationCreateInfo{};
  auto index = luna::vulkan::find_valid_entry(res.buffers);
  auto& buffer = res.buffers[index];

  luna::log_debug("Vulkan -> Creating uniform buffer on gpu ", gpu, " with size ", size);
  alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
  info.size = size;
  info.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eStorageBuffer;
  buffer.valid = true;
  buffer.gpu = gpu;
  auto& c_info = static_cast<VkBufferCreateInfo&>(info);
  auto c_buffer = static_cast<VkBuffer>(buffer.buffer);
  vmaCreateBuffer(res.allocators[gpu], &c_info, &alloc_info, &c_buffer, &buffer.alloc, nullptr);
  buffer.buffer = c_buffer;
  return index;
}

LUNA_C_API auto make_mappable_buffer(int gpu, size_t size) -> int32_t {
  auto& res  = luna::vulkan::global_resources();
  auto info = vk::BufferCreateInfo();
  auto alloc_info = VmaAllocationCreateInfo{};
  auto index = luna::vulkan::find_valid_entry(res.buffers);
  auto& buffer = res.buffers[index];

  luna::log_debug("Vulkan -> Creating mappable buffer on gpu ", gpu, " with size ", size);
  buffer.valid = true;
  buffer.gpu = gpu;
  alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
  alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
  info.size = size;
  info.usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eStorageBuffer;

  auto& c_info = static_cast<VkBufferCreateInfo&>(info);
  auto c_buffer = static_cast<VkBuffer>(buffer.buffer);
  vmaCreateBuffer(res.allocators[gpu], &c_info, &alloc_info, &c_buffer, &buffer.alloc, nullptr);
  buffer.buffer = c_buffer;
  return index;
}

LUNA_C_API auto destroy_buffer(int32_t handle) -> void {
  auto& res  = luna::vulkan::global_resources();
  auto& buffer = res.buffers[handle];
  auto c_buffer = static_cast<VkBuffer>(buffer.buffer);
  
  luna::log_debug("Vulkan -> Creating destroying buffer ", handle);
  LunaAssert(handle >= 0, "Attempting to use an invalid image.");
  vmaDestroyBuffer(res.allocators[buffer.gpu], c_buffer, buffer.alloc);
  buffer.valid = false;
}

LUNA_C_API auto make_image(luna::gfx::ImageInfo in_info, const unsigned char* pInitialData) -> int32_t {
  return luna::vulkan::create_image(in_info, pInitialData);
}

LUNA_C_API auto destroy_image(int32_t handle) -> void {
  LunaAssert(handle >= 0, "Attempting to use an invalid image.");
  luna::vulkan::destroy_image(handle);
}

LUNA_C_API auto make_command_buffer(int gpu) -> int32_t {
  return luna::vulkan::create_cmd(gpu, nullptr);
}

LUNA_C_API auto destroy_command_buffer(int32_t handle) -> void {
  LunaAssert(handle >= 0, "Attempting to use an invalid command buffer.");
  luna::vulkan::destroy_cmd(handle);
}

LUNA_C_API auto set_descriptor(int32_t cmd_id, int32_t desc_id) -> void {
  auto& res = luna::vulkan::global_resources();
  auto& cmd = res.cmds[cmd_id].cmd;
  auto& gpu = res.devices[res.cmds[cmd_id].gpu];
  auto& desc = res.descriptors[desc_id];
  auto& pipeline = res.pipelines[desc.pipeline];
  LunaAssert(cmd_id >= 0, "Attempting to use an invalid command buffer."); 
  cmd.bindPipeline(pipeline.bind_point, pipeline.pipeline, gpu.m_dispatch);  
}