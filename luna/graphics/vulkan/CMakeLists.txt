add_subdirectory(vma_lib)
add_subdirectory(vk_wrappers)
find_package(SDL2 REQUIRED)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/backend)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/backend)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/backend)
set(DOCUMENTATION_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/backend)

set(vulkan_impl
  interface.cpp
  global_resources.cpp

  window/window.cpp
)

add_library(luna_vulkan SHARED ${vulkan_impl})
target_link_libraries(luna_vulkan PUBLIC vulkan_impl SDL2::SDL2)
