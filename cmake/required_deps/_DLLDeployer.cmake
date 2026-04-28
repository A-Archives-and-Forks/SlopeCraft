#function(SC_download url filename)
#    if (EXISTS ${filename})
#        file(SIZE ${filename} size)
#        if (${size} GREATER 0)
#            return()
#        endif ()
#
#        file(REMOVE ${filename})
#    endif ()
#
#    message(STATUS "Downloading ${url} ...")
#    file(DOWNLOAD ${url} ${filename}
#        SHOW_PROGRESS
#        REQUIRED)
#
#    file(SIZE ${filename} size)
#    if (${size} LESS_EQUAL 0)
#        message(FATAL_ERROR "Failed to download ${filename} from ${url}, downloaded file is empty")
#    endif ()
#endfunction(SC_download)

set(DLLD_download_dir ${CMAKE_BINARY_DIR}/3rdParty/DLLDeployer)
set(DLLD_file ${DLLD_download_dir}/DLLDeployer.cmake)
set(QD_file ${DLLD_download_dir}/QtDeployer.cmake)
if (${WIN32})
    file(DOWNLOAD https://github.com/SlopeCraft/SharedLibDeployer/releases/download/v1.6.0/DLLDeployer.cmake
        ${DLLD_file}
        EXPECTED_HASH SHA256=a3d05d2174488f9373af02990254c47a75c24ee628c6763ea76b1f97fe02bee3
    )
    include(${DLLD_file})
endif ()

if (WIN32 OR APPLE)
    file(DOWNLOAD https://github.com/SlopeCraft/SharedLibDeployer/releases/download/v1.6.0/QtDeployer.cmake
        ${QD_file}
        EXPECTED_HASH SHA256=ad420f8df9cec5c8df81d9e394f15de81f5095d1472ebae6f487e9000bcc956a
    )
    include(${QD_file})
endif ()
