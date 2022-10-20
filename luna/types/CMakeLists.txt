find_package(ohm REQUIRED)
set(types_srcs 
    model.cpp
    image.cpp
)
add_library(types STATIC ${types_srcs})
target_link_libraries(types ohm::io ohm::vulkan)