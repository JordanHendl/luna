#include "luna/graphics/vulkan/vulkan_defines.hpp"
#include "luna/graphics/vulkan/global_resources.hpp"
#include "luna/error/error.hpp"
#include <memory>
#include <utility>
#include <vector>
namespace luna {
namespace vulkan {
constexpr auto MAX_OBJECT_AMT = 1024;
constexpr auto MAX_CMD_AMT = 64;

auto create_pool(Device& device, int queue_family) -> vk::CommandPool {
  const vk::CommandPoolCreateFlags flags =
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer;  // TODO make this
                                                           // configurable.
  static auto pool_map = std::map<vk::Device, std::map<int, vk::CommandPool>>();
  auto info = vk::CommandPoolCreateInfo();
  info.setFlags(flags);
  info.setQueueFamilyIndex(queue_family);

  auto& queue_map = pool_map[device.gpu];
  auto iter = queue_map.find(queue_family);
  if (iter == queue_map.end()) {
    auto pool = error(device.gpu.createCommandPool(
        info, device.allocate_cb, device.m_dispatch));
    iter = queue_map.insert(iter, {queue_family, pool});
  };
  return iter->second;
}

inline auto GlobalResources::make_instance() -> void {
  auto& loader = this->vulkan_loader;
#ifdef _WIN32
  loader.load("vulkan-1.dll");
#elif __linux__
  loader.load("libvulkan.so.1");
#endif

  LunaAssert(loader, "Failed to load the vulkan dynamic library (libvulkan.so/vulkan-1.dll).");
  this->instance = std::move(Instance(loader, nullptr, "luna_temp_app_name"));
}

GlobalResources::GlobalResources() {
  this->make_instance();
  this->buffers.resize(MAX_OBJECT_AMT);
  this->images.resize(MAX_OBJECT_AMT);
  this->cmds.resize(MAX_CMD_AMT);
}

GlobalResources::~GlobalResources() {
  for(auto& alloc : this->allocators) {
    if(alloc) vmaDestroyAllocator(alloc);
  }
}

auto global_resources() -> GlobalResources& {
  static auto global = GlobalResources();
  return global;
}
}
}