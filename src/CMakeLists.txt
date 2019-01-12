project(${CMAKE_PROJECT_NAME}_lib)

set(HEADER_FILES

)
set(SOURCE_FILES
        bisection.hpp
)

if (EXISTS ${SOURCE_FILES})

    add_library(${CMAKE_PROJECT_NAME}_lib STATIC
            ${SOURCE_FILES}
            ${HEADER_FILES}
    )
else()
    add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
endif()