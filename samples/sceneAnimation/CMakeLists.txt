setup_sample()
CONVERT_TOOL(art/scene.dae data)
COPY_TOOL(art/wall_DISP.png data)
COPY_TOOL(art/wall_OCC.png data)
BLENDERDNA_TOOL(art/scene.blend ${CONVERTER_TOOL_SOURCE_DIR})


