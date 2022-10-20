find_package(assimp REQUIRED)
add_library(lum_loader STATIC lum_loader.cpp)
target_include_directories(lum_loader PRIVATE ${ASSIMP_INCLUDE_DIRECTORIES})
target_link_libraries(lum_loader PUBLIC ${ASSIMP_LIBRARIES})