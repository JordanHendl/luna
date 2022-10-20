set(vulkan_impl_files
  command_buffer.cpp
  device.cpp
  instance.cpp
  swapchain.cpp
  instance.cpp
  swapchain.cpp
)

add_library(vulkan_impl STATIC ${vulkan_impl_files})
target_link_libraries(vulkan_impl PUBLIC vma_lib)