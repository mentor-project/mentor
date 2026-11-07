include(mentor_test)

message(STATUS "[mentor_lib] Configuring Base tests...")

mentor_add_test(payload SOURCES payload.cpp)

message(STATUS "[mentor_lib] Base tests configuration succeed!")
