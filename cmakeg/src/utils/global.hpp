#pragma once
#include <string>

namespace global
{
    const std::string assetsCMakeStr = R"(

add_custom_target(ProjectName_copy_assets_dir
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying assets directory"
)

add_dependencies(${PROJECT_NAME} ProjectName_copy_assets_dir))";
}