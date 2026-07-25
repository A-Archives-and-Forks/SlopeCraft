set(SlopeCraft_readme_and_license_files

    LICENSE
    README.md
    README-en.md
    license-translations/LICENSE-zh.md
)

include(${CMAKE_SOURCE_DIR}/cmake/add_compiler_path_to_prefix.cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/SlopeCraftConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
install(FILES ${CMAKE_BINARY_DIR}/SlopeCraftConfigVersion.cmake
    DESTINATION lib/cmake/SlopeCraft
)

install(EXPORT SlopeCraftTargets
    FILE SlopeCraftTargets.cmake
    NAMESPACE SlopeCraft::
    DESTINATION lib/cmake/SlopeCraft
)

if (${WIN32})
    #include(${CMAKE_SOURCE_DIR}/cmake/scan_deps_for_lib.cmake)
    install(FILES ${SlopeCraft_readme_and_license_files}
        DESTINATION .)
    return()
endif ()

if (${LINUX})
    install(FILES ${SlopeCraft_readme_and_license_files}
        DESTINATION share/SlopeCraft
    )
    return()
endif ()

if (${APPLE})
    install(FILES ${SlopeCraft_readme_and_license_files}
        DESTINATION .
    )
    return()
endif ()

message(WARNING "No rule to install readme and license files. Unknown system name.")