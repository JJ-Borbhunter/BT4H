
# Source and header files
file(GLOB BT4H_SRC CONFIGURE_DEPENDS src/*.cpp src/NES/*.cpp)
file(GLOB BT4H_HDR CONFIGURE_DEPENDS src/*.hpp src/NES/*.hpp)

# Create the BT4H static library
add_library(BT4H STATIC ${BT4H_SRC} ${BT4H_HDR})

# Public headers (so users can include them)
target_include_directories(BT4H
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link against SDL3 (from the subdirectory)
target_link_libraries(BT4H PUBLIC SDL3::SDL3)
