
file(GLOB PNG_SOURCE "*.c*")
message("file ${PNG_SOURCE}")
add_library(pngdecoder
           ${PNG_SOURCE}
)

target_link_libraries(pngdecoder
                    z
)

