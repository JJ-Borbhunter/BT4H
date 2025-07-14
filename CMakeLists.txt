
# Source and header files
file(GLOB BT4H_SRC CONFIGURE_DEPENDS src/*.cpp src/NES/*.cpp)
file(GLOB BT4H_HDR CONFIGURE_DEPENDS src/*.hpp src/NES/*.hpp)

# Create the BT4H static library
add_library(BT4H STATIC ${BT4H_SRC} ${BT4H_HDR})

# Public headers (so users can include them)
target_include_directories(BT4H
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/SAGO/
)

target_sources(BT4H
    PRIVATE
        deps/SAGO/sago/platform_folders.cpp
)

# Link against SDL3 (from the subdirectory)
target_link_libraries(BT4H PUBLIC SDL3::SDL3)

if(UNIX AND NOT APPLE)
    # This should match Linux (not macOS)
    find_package(X11 REQUIRED)
    if(X11_FOUND)
        target_include_directories(BT4H PUBLIC ${X11_INCLUDE_DIR})
        target_link_libraries(BT4H PUBLIC ${X11_LIBRARIES})
    endif()
endif()

