add_subdirectory(thirdparty)

add_executable(mentor_tui)
target_sources(mentor_tui PRIVATE main.cpp)
target_link_libraries(mentor_tui
  PRIVATE
    mentor_lib
    ftxui::ftxui
)
