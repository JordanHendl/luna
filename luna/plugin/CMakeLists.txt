set(plugin_srcs
  plugin.cpp
  scheduler.cpp
)

add_library(plugin STATIC ${plugin_srcs})