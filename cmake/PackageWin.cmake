macro(ConfigurePlatformPackaging)
  set(CPACK_GENRATOR "WIX")
  set(CPACK_PACKAGING_INSTALL_PREFIX "")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CMAKE_PROJECT_NAME})
endmacro()