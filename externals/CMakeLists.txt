PROJECT(pugixml)

file(GLOB src "pugixml/src/*.cpp")
file(GLOB hdr "pugixml/src/*.hpp")

setup_library ()

PROJECT(imgui)
file(GLOB src "imgui/*.cpp")
file(GLOB hdr "imgui/*.h")
setup_library ()


