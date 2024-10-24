project(fifos)

add_executable(fifo_server
	fifo_server.cpp
)

add_executable(fifo_client
	fifo_client.cpp
)

if(UNIX)
target_link_libraries(fifo_server
	pthread
)
endif()
