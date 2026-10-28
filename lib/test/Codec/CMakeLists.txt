include(mentor_test)

message(STATUS "[mentor_lib] Configuring Codec tests...")

mentor_add_test(model SOURCES model.cpp)
mentor_add_test(test_file SOURCES test_file.cpp)

message(STATUS "[mentor_lib] Codec tests configuration succeed!")
