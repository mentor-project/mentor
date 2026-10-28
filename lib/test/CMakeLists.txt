include(mentor_test)

message(STATUS "[mentor_lib] Configuring tests...")

add_subdirectory(Codec)
add_subdirectory(Service)
add_subdirectory(Kernel)

message(STATUS "[mentor_lib] Tests configuration succeed!")
