if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/lazyq-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package lazyq)

install(
    DIRECTORY
    include/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT lazyq_Development
)

install(
    TARGETS lazyq_lazyq
    EXPORT lazyqTargets
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    lazyq_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE lazyq_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(lazyq_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${lazyq_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT lazyq_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${lazyq_INSTALL_CMAKEDIR}"
    COMPONENT lazyq_Development
)

install(
    EXPORT lazyqTargets
    NAMESPACE lazyq::
    DESTINATION "${lazyq_INSTALL_CMAKEDIR}"
    COMPONENT lazyq_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
