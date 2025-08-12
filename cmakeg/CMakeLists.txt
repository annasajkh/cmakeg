# EXECUTABLE PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS PROJECT

set(PROJECT_NAME cmakeg)

add_subdirectory(dependencies/CLI11)
add_subdirectory(dependencies/boost)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} "${SOURCE_FILES}")

set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}
    
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/Release
)

target_link_libraries(${PROJECT_NAME} PRIVATE CLI11::CLI11 Boost::dll Boost::algorithm)


add_custom_target(copy_assets_dir
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying assets directory"
)

add_dependencies(${PROJECT_NAME} copy_assets_dir)