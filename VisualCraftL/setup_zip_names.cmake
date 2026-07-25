set(VCL_app_files
    ${CMAKE_SOURCE_DIR}/VisualCraftL/VCL_blocks_fixed.json)

#message(STATUS "VCL_version_names: ${VCL_version_names}")

foreach (mcver ${VCL_version_names})
    set(VCL_current_var_name VCL_resource_${mcver})

    if (NOT DEFINED ${VCL_current_var_name})
        message(WARNING "${VCL_current_var_name} is not defined")
    endif ()

    list(APPEND VCL_app_files ${${VCL_current_var_name}})
endforeach ()
unset(mcver)

#set(VCL_app_files ${VCL_app_files} PARENT_SCOPE)