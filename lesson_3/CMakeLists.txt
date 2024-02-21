project(LESSON_3)

add_executable(lesson_3 main.cpp utils.h)
target_link_libraries(lesson_3 ${SDL_LIBS})

add_custom_command(TARGET lesson_3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL_BIN_PATH}"
        $<TARGET_FILE_DIR:lesson_3>)