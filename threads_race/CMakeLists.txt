project(threads_race)

add_executable(threads_race
    threads_race.cpp
)

if(UNIX)
target_link_libraries(threads_race
    pthread
)
endif()

