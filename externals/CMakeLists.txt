PROJECT(pugixml)
file(GLOB src "pugixml/src/*.cpp")
file(GLOB hdr "pugixml/src/*.hpp")
setup_library ()

PROJECT(imgui)
file(GLOB src "imgui/*.cpp")
file(GLOB hdr "imgui/*.h")
setup_library ()

PROJECT(bulletLinearMath)
file(GLOB_RECURSE src "bullet/src/LinearMath/*.cpp")
file(GLOB_RECURSE hdr "bullet/src/LinearMath/*.h")
setup_library ()

PROJECT(bulletCollision)
file(GLOB_RECURSE src "bullet/src/BulletCollision/*.cpp")
file(GLOB_RECURSE hdr "bullet/src/BulletCollision/*.h")
setup_library ()

PROJECT(bulletDynamics)
file(GLOB_RECURSE src "bullet/src/BulletDynamics/*.cpp")
file(GLOB_RECURSE hdr "bullet/src/BulletDynamics/*.h")
setup_library ()
target_link_libraries(${PROJECT_NAME} bulletCollision)


PROJECT(bulletSoftBody)
file(GLOB_RECURSE src "bullet/src/BulletSoftBody/*.cpp")
file(GLOB_RECURSE hdr "bullet/src/BulletSoftBody/*.h")
setup_library ()