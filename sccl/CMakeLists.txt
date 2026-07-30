project(sccl VERSION ${SlopeCraft_version} LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(sccl
    main.cpp
)

find_package(Qt6 COMPONENTS LinguistTools REQUIRED)
find_package(magic_enum REQUIRED)
find_package(OpenMP REQUIRED)

target_link_libraries(sccl PRIVATE
    SlopeCraftL
    Qt6::Core
    OpenMP::OpenMP_CXX
    magic_enum::magic_enum
    MCDataVersion
)
target_include_directories(sccl PRIVATE
    ${cli11_include_dir}
    ${SlopeCraft_Nlohmann_json_include_dir}
    ${CMAKE_SOURCE_DIR}/utilities
)
set_target_properties(sccl PROPERTIES
    VERSION ${PROJECT_VERSION}

    # MACOSX_BUNDLE_ICON_FILE imageCutterIconNew.icns
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.github.ToKiNoBug.SlopeCraft"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
)