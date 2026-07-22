find_package(Vulkan REQUIRED)
find_package(VulkanMemoryAllocator CONFIG)

#if (NOT VulkanMemoryAllocator_FOUND)
#    find_file(vma_header_loc
#        NAMES vk_mem_alloc.h
#        #        REQUIRED
#    )
#    if (NOT vma_header_loc)
#        message(FATAL_ERROR "Failed to find VulkanMemoryAllocator, failed to find vma header file vk_mem_alloc.h \ VMA is not installed, must install it.")
#    endif ()
#    message(STATUS "Found vma header file at ${vma_header_loc}")
#
#    add_library(VulkanMemoryAllocator INTERFACE)
#    cmake_path(GET vma_header_loc PARENT_PATH vma_include_dir)
#    target_include_directories(VulkanMemoryAllocator INTERFACE ${vma_include_dir})
#    add_library(GPUOpen::VulkanMemoryAllocator ALIAS VulkanMemoryAllocator)
#endif ()

include(FetchContent)
FetchContent_Declare(
    VulkanMemoryAllocator-Hpp
    URL https://github.com/YaaZ/VulkanMemoryAllocator-Hpp/releases/download/v3.3.0%2B3/VulkanMemoryAllocator-Hpp-3.3.0.tar.gz
    URL_HASH SHA256=8c2a0573babe1f86f3241d12b4c9df40bb944649b7f4239c1c611797f8af6cd5 # Copy from the asset list on the release page
    OVERRIDE_FIND_PACKAGE
)
message(STATUS "Downloading YaaZ/VulkanMemoryAllocator-Hpp......")
FetchContent_MakeAvailable(VulkanMemoryAllocator-Hpp)
find_package(VulkanMemoryAllocator-Hpp CONFIG REQUIRED)