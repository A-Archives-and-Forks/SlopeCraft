set(VCL_zip_names
    "Vanilla_1_12_2.zip"
    "Vanilla_1_13_2.zip"
    "Vanilla_1_14_4.zip"
    "Vanilla_1_15_2.zip"
    "Vanilla_1_16_5.zip"
    "Vanilla_1_17_1.zip"
    "Vanilla_1_18_2.zip"
    "Vanilla_1_19_3.zip"
    "Vanilla_1_20_6.zip"
    "Vanilla_1_21_11.zip"
    "Vanilla_26_1_2.zip"
    "Vanilla_26_2.zip"
)
set(VCL_version_names
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    26_1_2
    26_2
)

list(LENGTH VCL_zip_names vcl_zip_num)
list(LENGTH VCL_version_names vcl_version_num)

if (NOT ${vcl_zip_num} EQUAL ${vcl_version_num})
    message(FATAL_ERROR "VCL zip filenames and versions not match: ${VCL_zip_names} and ${VCL_version_names}")
endif ()

#https://github.com/SlopeCraft/VisualCraft-binaries/releases/download/resource-packs/Vanilla_1_20_1.zip
set(VCL_url_prefix "https://github.com/SlopeCraft/VisualCraft-binaries/releases/download/resource-packs/")

math(EXPR VCL_resource_idx_max "${vcl_zip_num}-1")
foreach (VCL_resource_idx RANGE 0 ${VCL_resource_idx_max})
    # message(STATUS "VCL_resource_idx = ${VCL_resource_idx}")
    list(GET VCL_zip_names ${VCL_resource_idx} VCL_current_zip_name)
    list(GET VCL_version_names ${VCL_resource_idx} mcver)

    set(VCL_resource_${mcver} ${CMAKE_SOURCE_DIR}/binaries/${VCL_current_zip_name} CACHE FILEPATH "")

    if (EXISTS ${CMAKE_SOURCE_DIR}/binaries/${VCL_current_zip_name})
        message(STATUS "Found resource pack for MC${mcver} (${VCL_current_zip_name}), skip downloading.")
        continue()
    endif ()

    message(STATUS "${VCL_current_zip_name} not found. Downloading...")

    string(CONCAT VCL_current_download_url ${VCL_url_prefix} ${VCL_current_zip_name})
    file(DOWNLOAD
        ${VCL_current_download_url}
        ${CMAKE_SOURCE_DIR}/binaries/${VCL_current_zip_name})

    if (EXISTS ${CMAKE_SOURCE_DIR}/binaries/${VCL_current_zip_name})
        message(STATUS "Downloaded resoruce pack for MC${mcver}.")
    else ()
        message(FATAL_ERROR "Failed to download resource pack for MC${mcver} from ${VCL_current_download_url}.")
    endif ()
endforeach ()

#foreach (mcver RANGE 12 21)
#    math(EXPR VCL_resource_idx "${mcver} - 12")
#
#    # message(STATUS "VCL_resource_idx = ${VCL_resource_idx}")
#    list(GET VCL_zip_names ${VCL_resource_idx} VCL_current_zip_name)
#
#    set(VCL_resource_${mcver} ${CMAKE_SOURCE_DIR}/binaries/${VCL_current_zip_name} CACHE FILEPATH "")
#
#    if (EXISTS ${CMAKE_SOURCE_DIR}/binaries/${VCL_current_zip_name})
#        # message(STATUS "Found resoruce pack for MC${mcver} (${VCL_current_zip_name}).")
#        continue()
#    endif ()
#
#    message(STATUS "${VCL_current_zip_name} not found. Downloading...")
#
#    string(CONCAT VCL_current_download_url ${VCL_url_prefix} ${VCL_current_zip_name})
#    file(DOWNLOAD
#        ${VCL_current_download_url}
#        ${CMAKE_SOURCE_DIR}/binaries/${VCL_current_zip_name})
#
#    if (EXISTS ${CMAKE_SOURCE_DIR}/binaries/${VCL_current_zip_name})
#        message(STATUS "Downloaded resoruce pack for MC${mcver}.")
#    else ()
#        message(FATAL_ERROR "Failed to download resource pack for MC${mcver} from ${VCL_current_download_url}.")
#    endif ()
#endforeach ()