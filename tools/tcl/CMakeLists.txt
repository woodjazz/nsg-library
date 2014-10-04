PROJECT(tcl)
set(SRC Main.cpp)
add_executable(${PROJECT_NAME} ${SRC})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "tools")

