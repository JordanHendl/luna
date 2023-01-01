set(luna_files
  luna.cpp
)
add_library(luna SHARED ${luna_files})
target_link_libraries(luna config plugin io)

add_executable(luna_runtime main.cpp)
target_link_libraries(luna_runtime PRIVATE luna)