include_guard(GLOBAL)

function(mentor_add_test target)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs SOURCES)

  cmake_parse_arguments(MENTOR_TEST
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
  )

  if (NOT MENTOR_TEST_SOURCES)
    message(FATAL_ERROR "mentor_add_test(${target}) requires SOURCES")
  endif()

  add_executable(${target})
  target_sources(${target} PRIVATE ${MENTOR_TEST_SOURCES})
  target_link_libraries(${target} PRIVATE mentor_lib)

  add_test(
    NAME ${target}
    COMMAND ${target}
  )
endfunction()
