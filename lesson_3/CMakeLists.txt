project(LESSON_3)

add_executable(lesson_3 main.cpp utils.h)
target_link_libraries(lesson_3 ${SDL_LIBS})