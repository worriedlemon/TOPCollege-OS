cmake_minimum_required(VERSION 3.12)
project(college_os C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Enabling strict warning processing
if(UNIX)
add_compile_options(-Wall -Wextra -Wpedantic)
else()
add_compile_options(/W4 /permissive-)
endif()

set(PROJECTS
    # IPC
    signals
    fifos
    sockets
    
    # Threads
    threads_race
    
    # Memory
    address_test
    segments

    # Linking
    library_linking
)

if(UNIX)
list(APPEND PROJECTS
    pipes
    shared_memory
)
else()
message(NOTICE "Skipped 'pipes' and 'shared_memory' - unsupported OS")
endif()

foreach(project ${PROJECTS})

if(EXISTS ${CMAKE_SOURCE_DIR}/${project})
add_subdirectory(${CMAKE_SOURCE_DIR}/${project})
message(STATUS "Added project ${project}")
else()
message(WARNING "Project ${project} does not exist!")
endif()

endforeach()

