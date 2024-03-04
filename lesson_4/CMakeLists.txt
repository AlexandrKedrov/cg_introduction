project(LESSON_4)

add_executable(lesson_4 main.cpp ../lesson_3/utils.h)
target_link_libraries(lesson_4 ${SDL_LIBS})

add_custom_command(TARGET lesson_4 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL_BIN_PATH}"
        $<TARGET_FILE_DIR:lesson_4>)