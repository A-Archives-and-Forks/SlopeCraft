
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    install(TARGETS sccl
        EXPORT SlopeCraftTargets
        RUNTIME DESTINATION .
    )

    QD_add_deployqt(sccl
        BUILD_MODE
        FLAGS ${SlopeCraft_windeployqt_flags_build}
    )
    QD_add_deployqt(sccl
        INSTALL_MODE INSTALL_DESTINATION .
        FLAGS ${SlopeCraft_windeployqt_flags_install}
    )
    DLLD_add_deploy(sccl
        BUILD_MODE
        INSTALL_MODE INSTALL_DESTINATION .
        IGNORE SlopeCraftL.dll libSlopeCraftL.dll
    )

    return()
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    install(TARGETS sccl
        EXPORT SlopeCraftTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
    )
    return()
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message(WARNING "No rule to install sccl on darwin, don't know how todo")
    return()
endif ()