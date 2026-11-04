include(mentor_test)

message(STATUS "[mentor_lib] Configuring Kernel tests...")

mentor_add_test(load_test_file SOURCES load_test_file.cpp)
mentor_copy_file(load_test_file test_file_valid.json)
mentor_copy_file(load_test_file test_file_invalid.json)
mentor_add_test(transition SOURCES transition.cpp)
mentor_copy_file(transition test_file_valid.json)
mentor_copy_file(transition test_file_invalid.json)
mentor_add_test(request_question SOURCES request_question.cpp)
mentor_copy_file(request_question test_file_valid.json)

message(STATUS "[mentor_lib] Kernel tests configuration succeed!")
