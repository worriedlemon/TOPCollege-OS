project(library_linking)

add_executable(dynamic_linking
    linking_test.cpp
)

add_executable(late_linking
    linking_test.cpp
)

add_executable(static_linking
    linking_test.cpp
)

add_library(mytestlib_static STATIC
    mytestlib.cpp
)

add_library(mytestlib_shared SHARED
    mytestlib.cpp
)

if(UNIX)
set(DL_LIBS ${CMAKE_DL_LIBS})
else()
set(DL_LIBS "")
endif()

target_link_libraries(static_linking PRIVATE
    mytestlib_static
)

target_link_libraries(dynamic_linking
    mytestlib_shared
    ${DL_LIBS}
)

target_compile_definitions(late_linking PRIVATE
    _runtime
)

add_dependencies(late_linking
    mytestlib_shared
)
