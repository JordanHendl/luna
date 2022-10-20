#pragma once 
#include "luna/io/dlloader.hpp"
#include <vulkan/vulkan.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vector>
#include <iostream>
namespace luna {
namespace vulkan {
struct Instance {
  Instance() {}
  Instance(io::Dlloader& loader, vk::AllocationCallbacks* callback, std::string_view app_name);
  Instance(Instance&& mv);
  ~Instance();
  auto operator=(Instance&& mv) -> Instance&;
  std::vector<vk::PhysicalDevice> m_devices;
  std::array<unsigned, 3> version;
  std::string app_name;
  vk::DispatchLoaderDynamic m_dispatch;
  vk::Instance m_instance;
  std::vector<std::string> extensions;
  std::vector<std::string> validation;

  private:
    inline vk::ApplicationInfo makeAppInfo();
    inline vk::DebugUtilsMessengerCreateInfoEXT makeDebugInfo();
    inline std::vector<const char*> makeExtensionList();
    inline std::vector<const char*> makeValidationList();
};
}
}