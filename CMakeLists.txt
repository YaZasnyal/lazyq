cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    lazyq
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    lazyq_lazyq
    INTERFACE
)
add_library(lazyq::lazyq ALIAS lazyq_lazyq)

include(GenerateExportHeader)
generate_export_header(
    lazyq_lazyq
    BASE_NAME lazyq
    EXPORT_FILE_NAME export/lazyq/lazyq_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(lazyq_lazyq INTERFACE LAZYQ_STATIC_DEFINE)
endif()

set_target_properties(
    lazyq_lazyq PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME lazyq
    OUTPUT_NAME lazyq
)

target_include_directories(
    lazyq_lazyq ${warning_guard}
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    lazyq_lazyq SYSTEM
    INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(lazyq_lazyq INTERFACE cxx_std_17)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${lazyq_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT lazyq_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of lazyq"
  )
endif()

include(cmake/dev-mode.cmake)
