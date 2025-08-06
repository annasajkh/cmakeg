#include "ExecutableProjectCommandHandler.hpp"
#include <iostream>
#include <format>
#include "../utils/filesystem_utils.hpp"

using namespace cmakeg::commands;

ExecutableProjectCommandHandler::ExecutableProjectCommandHandler() : CommandHandler()
{
	isPartOfAWorkspace = boost::filesystem::exists(workingDirPath / "CMakeLists.txt") && boost::filesystem::exists(workingDirPath / ".gitignore");

	if (isPartOfAWorkspace)
	{
		executableProjectTemplatePath = executablePath / "assets" / "ExecutableProjectTemplate";
	}
	else
	{
		executableProjectTemplatePath = executablePath / "assets" / "ExecutableProjectWithoutWorkspaceTemplate";
	}
}

void ExecutableProjectCommandHandler::execute()
{
	executableProjectDestinationPath = workingDirPath / executableProjectName;

	if (boost::filesystem::is_directory(executableProjectDestinationPath))
	{
		std::cout << "Directory with the name \"" << executableProjectName << "\" already exist on the current directory\n";
		return;
	}

	boost::filesystem::copy(executableProjectTemplatePath, executableProjectDestinationPath, boost::filesystem::copy_options::recursive);

	if (isPartOfAWorkspace)
	{
		std::string executableProjectReferenceStr = std::format("\nadd_subdirectory({})", executableProjectName);
		std::string workspaceCMakeLists = filesystem_utils::fileReadText(workingDirPath / "CMakeLists.txt");

		filesystem_utils::fileWriteText(workingDirPath / "CMakeLists.txt", workspaceCMakeLists + executableProjectReferenceStr);

		filesystem_utils::findAndReplaceTextFile(executableProjectDestinationPath / "CMakeLists.txt", "ExecutableProjectTemplate", executableProjectName);
	}
	else
	{
		filesystem_utils::findAndReplaceTextFile(executableProjectDestinationPath / "CMakeLists.txt", "ExecutableProjectWithoutWorkspaceTemplate", executableProjectName);

		if (cmakeMinimumRequired != "3.20")
		{
			filesystem_utils::findAndReplaceTextFile(executableProjectDestinationPath / "CMakeLists.txt", "cmake_minimum_required(VERSION 3.20)", "cmake_minimum_required(VERSION " + cmakeMinimumRequired + ")");
		}

		if (cppVersion != "20")
		{
			filesystem_utils::findAndReplaceTextFile(executableProjectDestinationPath / "CMakeLists.txt", "set(CMAKE_CXX_STANDARD 20)", "set(CMAKE_CXX_STANDARD " + cppVersion + ")");
		}

		if (version != "0.0.1")
		{
			filesystem_utils::findAndReplaceTextFile(executableProjectDestinationPath / "CMakeLists.txt", "project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)", "project(${PROJECT_NAME} VERSION " + version + " LANGUAGES CXX)");
		}
	}

	if (isAddAssets)
	{
		std::string assetsCMakeStr = R"(

add_custom_target(copy_assets_dir
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying assets directory"
)

add_dependencies(${PROJECT_NAME} copy_assets_dir))";

		std::string projectCMakeLists = filesystem_utils::fileReadText(executableProjectDestinationPath / "CMakeLists.txt");
		filesystem_utils::fileWriteText(executableProjectDestinationPath / "CMakeLists.txt", projectCMakeLists + assetsCMakeStr);

		boost::filesystem::create_directory(executableProjectDestinationPath / "assets");
	}
}