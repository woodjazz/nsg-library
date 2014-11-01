PROJECT(bulletDynamics)

file (GLOB src 
    Character/*.cpp 
    ConstraintSolver/*.cpp
    Dynamics/*.cpp 
    Vehicle/*.cpp 
    Featherstone/*.cpp 
    MLCPSolvers/*.cpp)

file (GLOB hdr 
    Character/*.h
    ConstraintSolver/*h
    Dynamics/*.h
    Vehicle/*.h
    Featherstone/*.h
    MLCPSolvers/*.h)

setup_library()
target_link_libraries(${PROJECT_NAME} bulletCollision)
