include_guard(GLOBAL)

function(mentor_add_test target)
  cmake_parse_arguments(MT
    ""
    ""
    "SOURCES"
    ${ARGN}
  )

  add_executable(${target})
  target_sources(${target} PRIVATE ${MT_SOURCES})
  target_link_libraries(${target} PRIVATE mentor_lib)

  add_test(
    NAME ${target}
    COMMAND ${target}
  )

  set_tests_properties(${target} PROPERTIES
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:${target}>"
  )
endfunction()

function(mentor_copy_dir target src)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_CURRENT_SOURCE_DIR}/${src}
      $<TARGET_FILE_DIR:${target}>/${src}
    VERBATIM
  )
endfunction()

function(mentor_copy_file target src)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/${src}
      $<TARGET_FILE_DIR:${target}>/${src}
    VERBATIM
  )
endfunction()
