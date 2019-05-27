project(${CMAKE_PROJECT_NAME}_lib)

set(HEADER_FILES bisectionUtilities.hpp iterativeBisection.hpp growingBisection.hpp bisectionResult.hpp )
set(SOURCE_FILES)

if (EXISTS ${SOURCE_FILES})
    add_library(${CMAKE_PROJECT_NAME}_lib STATIC
        ${SOURCE_FILES}
        ${HEADER_FILES})
else()
    add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
    target_include_directories(${CMAKE_PROJECT_NAME}_lib INTERFACE ./)
endif()
