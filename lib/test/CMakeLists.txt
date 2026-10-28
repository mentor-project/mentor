include(mentor_test)

message(STATUS "[mentor_lib] Configuring tests...")

add_subdirectory(Codec)
add_subdirectory(Service)

message(STATUS "[mentor_lib] Tests configuration succeed!")
