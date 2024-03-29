macro(ConfigurePlatformPackaging)
  set(CPACK_GENERATOR DEB)
  set( CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME})
  set( CPACK_PACKAGE_INSTALL_DIRECTORY ${CMAKE_PROJECT_NAME})
  set( CPACK_DEBIAN_PACKAGE_DEBUG OFF)
  set( CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
  set( CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
  set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Overcaster")
  set( CPACK_DEB_COMPONENT_INSTALL ON)
endmacro()