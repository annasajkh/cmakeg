# EXECUTABLE PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS PROJECT

set(PROJECT_NAME cmakeg)

add_subdirectory(dependencies/CLI11)
add_subdirectory(dependencies/boost)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} "${SOURCE_FILES}")

set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}
    
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/release
)

target_link_libraries(${PROJECT_NAME} PRIVATE CLI11::CLI11 Boost::dll Boost::algorithm)

add_custom_target(cmakeg_copy_assets_dir
    COMMAND ${CMAKE_COMMAND} -E rm -rf "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Syncing assets directory"
)

add_dependencies(${PROJECT_NAME} cmakeg_copy_assets_dir)