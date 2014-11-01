PROJECT(bulletCollision)

file (GLOB src 
    BroadphaseCollision/*.cpp 
    CollisionDispatch/*.cpp 
    CollisionShapes/*.cpp 
    Gimpact/*.cpp 
    NarrowPhaseCollision/*.cpp)

file (GLOB hdr 
    BroadphaseCollision/*.h 
    CollisionDispatch/*.h 
    CollisionShapes/*.h 
    Gimpact/*.h 
    NarrowPhaseCollision/*.h)

setup_library()
