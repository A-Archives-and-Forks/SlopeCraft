set(CPACK_DEBIAN_PACKAGE_MAINTAINER "SlopeCraft group")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)

set(SlopeCraft_debian_opencl_deps)

if (${SlopeCraft_GPU_API} STREQUAL "OpenCL")
    set(SlopeCraft_debian_opencl_deps
        ocl-icd-libopencl1
    )
endif ()
if (${SlopeCraft_GPU_API} STREQUAL "Vulkan")
    set(SlopeCraft_debian_vulkan_deps
        libvulkan1
    )
endif ()

set(CPACK_DEBIAN_PACKAGE_DEPENDS

    "zlib1g"
    "libzip-dev" # package name like lib*-dev are much more universal in different ubuntu releases
    "libboost-iostreams-dev"
    "libpng-dev"
    "libqt6core6"
    "libqt6gui6"
    "libqt6widgets6"
    "libqt6network6"
    "libopengl0"
    "libqt6opengl6"
    "libqt6openglwidgets6"
    "libqt6dbus6"
    "libglx0"
    "qt6-qpa-plugins"
    ${SlopeCraft_debian_opencl_deps}
    ${SlopeCraft_debian_vulkan_deps}
)

list(JOIN CPACK_DEBIAN_PACKAGE_DEPENDS "," CPACK_DEBIAN_PACKAGE_DEPENDS)

message(STATUS "CPACK_DEBIAN_PACKAGE_DEPENDS = ${CPACK_DEBIAN_PACKAGE_DEPENDS}")

set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
    "Get your Minecraft pixel painting in multiple kinds of forms")