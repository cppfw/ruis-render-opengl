cmake_minimum_required(VERSION 3.20)

set(name ruis-render-opengl)
project(${name})

set(dir_name ruis)

# !!! find_package must go after project() declaration !!!
# Otherwise VCPKG does not set the CMAKE_PREFIX_PATH to find packages.
find_package(myci CONFIG REQUIRED)

set(srcs)
myci_add_source_files(srcs
    DIRECTORY
        ../../src/${dir_name}
    RECURSIVE
)

myci_declare_library(${name}
    SOURCES
        ${srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        ../../src
    INSTALL_INCLUDE_DIRECTORIES
        ../../src/${dir_name}
    DEPENDENCIES
        rasterimage
        r4
        papki
        tml
        utki
        agg
        veg
        mikroxml
        cssom
        svgdom
        svgren
        ruis
        Freetype
    EXTERNAL_DEPENDENCIES
        JPEG
        GLEW
)
