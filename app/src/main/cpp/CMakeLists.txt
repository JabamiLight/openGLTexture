
cmake_minimum_required(VERSION 3.4.1)





add_subdirectory(./libpng)
add_subdirectory(./librender)


add_library( # Sets the name of the library.
             native-lib

             SHARED
             PngPreviewController.cpp )




target_link_libraries( # Specifies the target library.
                       native-lib
                       render_core
                       pngdecoder
                        )