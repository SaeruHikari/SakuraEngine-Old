Module(
    NAME TestGLTFMesh
    TYPE Test
    SRC_PATH  /#Default as Source
    DEPS
    DEPS_PUBLIC GLTFMesh
    INCLUDES_PUBLIC
    LINKS
    LINKS_PUBLIC
)

file(COPY scene.gltf scene.bin DESTINATION ${ENGINE_BIN_DIR}/Debug)
file(COPY scene.gltf scene.bin DESTINATION ${ENGINE_BIN_DIR}/Release)