add_library(mentor_lib STATIC)
target_compile_features(mentor_lib PUBLIC cxx_std_20)
target_include_directories(mentor_lib
  PUBLIC
    include
    thirdparty
)
target_sources(mentor_lib
  PRIVATE
    src/Codec/Model.cpp
    src/Codec/TestFile.cpp
    src/Service/FileService.cpp
)

if(BUILD_MENTOR_TEST)
  target_compile_definitions(mentor_lib PRIVATE MENTOR_LIB_TEST)
  add_subdirectory(test)
endif()
