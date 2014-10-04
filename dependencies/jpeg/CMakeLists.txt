PROJECT(jpeg)
# https://code.google.com/p/jpeg-compressor/
# needed to load progressive jpeg (stb does not support progressive jpeg)
file(GLOB src "*.cpp")
file(GLOB hdr "*.h")
setup_library()