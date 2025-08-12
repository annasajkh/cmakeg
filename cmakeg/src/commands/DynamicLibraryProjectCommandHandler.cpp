#include <iostream>
#include <format>
#include "../utils/filesystem_utils.hpp"
#include <boost/algorithm/algorithm.hpp>
#include "DynamicLibraryProjectCommandHandler.hpp"

using namespace cmakeg::commands;

DynamicLibraryProjectCommandHandler::DynamicLibraryProjectCommandHandler() : CommandHandler()
{

}

void DynamicLibraryProjectCommandHandler::execute()
{
    WorkspaceIntegrityStatus workspaceIntegrityStatus = checkWorkspaceIntegrity();

    switch (workspaceIntegrityStatus)
    {
    case WorkspaceIntegrityStatus::NotExist:
        std::cout << "Cannot find a workspace creating an independent dynamic library project\n";
        break;
    case WorkspaceIntegrityStatus::ContainsCMakeButNotIdentifier:
        std::cout << "Cannot find a workspace but found CMakeList.txt, is workspace identifier deleted?\n";
        return;
    }
    
    boost::filesystem::path dynamicLibraryProjectTemplatePath;

    if (workspaceIntegrityStatus == WorkspaceIntegrityStatus::Valid)
	{
		dynamicLibraryProjectTemplatePath = executablePath / "assets" / "DynamicLibraryProjectTemplate";
	}
	else
	{
		dynamicLibraryProjectTemplatePath = executablePath / "assets" / "DynamicLibraryProjectWithoutWorkspaceTemplate";
	}

	boost::filesystem::path dynamicLibraryProjectDestinationPath = boost::filesystem::current_path() / dynamicLibraryProjectName;

	if (boost::filesystem::is_directory(dynamicLibraryProjectDestinationPath))
	{
		std::cout << "Directory with the name \"" << dynamicLibraryProjectName << "\" already exist on the current directory\n";
		return;
	}

	boost::filesystem::copy(dynamicLibraryProjectTemplatePath, dynamicLibraryProjectDestinationPath, boost::filesystem::copy_options::recursive);
    
    if (workspaceIntegrityStatus == WorkspaceIntegrityStatus::Valid)
	{
        boost::filesystem::rename(dynamicLibraryProjectDestinationPath / "include" / "DynamicLibraryProjectTemplate", dynamicLibraryProjectDestinationPath / "include" / dynamicLibraryProjectName);
        filesystem_utils::findAndReplaceTextFile(dynamicLibraryProjectDestinationPath / "include" / dynamicLibraryProjectName / "Computer.hpp", "DynamicLibraryProjectTemplate", dynamicLibraryProjectName);
        filesystem_utils::findAndReplaceTextFile(dynamicLibraryProjectDestinationPath / "src" / "Computer.cpp", "DynamicLibraryProjectTemplate", dynamicLibraryProjectName);
    }
    else
    {
        boost::filesystem::rename(dynamicLibraryProjectDestinationPath / "include" / "DynamicLibraryProjectWithoutWorkspaceTemplate", dynamicLibraryProjectDestinationPath / "include" / dynamicLibraryProjectName);
        filesystem_utils::findAndReplaceTextFile(dynamicLibraryProjectDestinationPath / "include" / dynamicLibraryProjectName / "Computer.hpp", "DynamicLibraryProjectWithoutWorkspaceTemplate", dynamicLibraryProjectName);
        filesystem_utils::findAndReplaceTextFile(dynamicLibraryProjectDestinationPath / "src" / "Computer.cpp", "DynamicLibraryProjectWithoutWorkspaceTemplate", dynamicLibraryProjectName);
    }
    
    if (workspaceIntegrityStatus == WorkspaceIntegrityStatus::Valid)
	{
		std::string executableProjectReferenceStr = std::format("\nadd_subdirectory({})", dynamicLibraryProjectName);
		std::string workspaceCMakeLists = filesystem_utils::fileReadText(boost::filesystem::current_path() / "CMakeLists.txt");

		filesystem_utils::fileWriteText(boost::filesystem::current_path() / "CMakeLists.txt", workspaceCMakeLists + executableProjectReferenceStr);
		filesystem_utils::findAndReplaceTextFile(dynamicLibraryProjectDestinationPath / "CMakeLists.txt", "DynamicLibraryProjectTemplate", dynamicLibraryProjectName);
	}
	else
	{
		filesystem_utils::findAndReplaceTextFile(dynamicLibraryProjectDestinationPath / "CMakeLists.txt", "DynamicLibraryProjectWithoutWorkspaceTemplate", dynamicLibraryProjectName);

		if (cmakeMinimumRequired != "3.20")
		{
			filesystem_utils::findAndReplaceTextFile(dynamicLibraryProjectDestinationPath / "CMakeLists.txt", "cmake_minimum_required(VERSION 3.20)", "cmake_minimum_required(VERSION " + cmakeMinimumRequired + ")");
		}

		if (cppVersion != "20")
		{
			filesystem_utils::findAndReplaceTextFile(dynamicLibraryProjectDestinationPath / "CMakeLists.txt", "set(CMAKE_CXX_STANDARD 20)", "set(CMAKE_CXX_STANDARD " + cppVersion + ")");
		}

		if (version != "0.0.1")
		{
			filesystem_utils::findAndReplaceTextFile(dynamicLibraryProjectDestinationPath / "CMakeLists.txt", "project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)", "project(${PROJECT_NAME} VERSION " + version + " LANGUAGES CXX)");
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

		std::string projectCMakeLists = filesystem_utils::fileReadText(dynamicLibraryProjectDestinationPath / "CMakeLists.txt");
		filesystem_utils::fileWriteText(dynamicLibraryProjectDestinationPath / "CMakeLists.txt", projectCMakeLists + assetsCMakeStr);

		boost::filesystem::create_directory(dynamicLibraryProjectDestinationPath / "assets");
	}
}