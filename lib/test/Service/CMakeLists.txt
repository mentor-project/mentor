include(mentor_test)

message(STATUS "[mentor_lib] Configuring Service tests...")

mentor_add_test(file_service SOURCES file_service.cpp)
mentor_copy_dir(file_service dummy)
mentor_copy_file(file_service fstest_invalid.json)
mentor_copy_file(file_service fstest_valid1.json)
mentor_copy_file(file_service fstest_valid2.json)

message(STATUS "[mentor_lib] Service tests configuration succeed!")
