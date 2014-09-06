PROJECT(tcl)
set(SRC main.cpp)
add_executable(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tools")

