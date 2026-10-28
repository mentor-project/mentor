include(mentor_test)

message(STATUS "[mentor_lib] Configuring Kernel tests...")

mentor_add_test(init SOURCES init.cpp)
mentor_add_test(load_test_file SOURCES load_test_file.cpp)
mentor_copy_file(load_test_file test_file_valid.json)
mentor_copy_file(load_test_file test_file_invalid.json)

message(STATUS "[mentor_lib] Kernel tests configuration succeed!")
