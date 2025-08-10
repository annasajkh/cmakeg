#include <iostream>
#include <format>
#include "../utils/filesystem_utils.hpp"
#include <boost/algorithm/algorithm.hpp>
#include "StaticLibraryProjectCommandHandler.hpp"

using namespace cmakeg::commands;

StaticLibraryProjectCommandHandler::StaticLibraryProjectCommandHandler() : CommandHandler()
{

}

void StaticLibraryProjectCommandHandler::execute()
{
    bool currentDirHaveCMakeList = boost::filesystem::exists(boost::filesystem::current_path() / "CMakeLists.txt");
	bool isPartOfAWorkspace = currentDirHaveCMakeList;

	if (isPartOfAWorkspace)
	{
		std::string workspaceCMakeFile = filesystem_utils::fileReadText(boost::filesystem::current_path() / "CMakeLists.txt");

		isPartOfAWorkspace = isPartOfAWorkspace && (workspaceCMakeFile.find("# WORKSPACE INDICATOR DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS WORKSPACE") != std::string::npos);
	}

	if (!isPartOfAWorkspace && currentDirHaveCMakeList)
	{
		std::cout << "Cannot find a workspace but found CMakeList.txt, is workspace indicator deleted?\n";
        return;
	}
    else if(!isPartOfAWorkspace && !currentDirHaveCMakeList)
    {
        std::cout << "Cannot find a workspace creating an independent static library project\n";
    }
    
    boost::filesystem::path staticLibraryProjectTemplatePath;

    if (isPartOfAWorkspace)
	{
		staticLibraryProjectTemplatePath = executablePath / "assets" / "StaticLibraryProjectTemplate";
	}
	else
	{
		staticLibraryProjectTemplatePath = executablePath / "assets" / "StaticLibraryProjectWithoutWorkspaceTemplate";
	}

	boost::filesystem::path staticLibraryProjectDestinationPath = boost::filesystem::current_path() / staticLibraryProjectName;

	if (boost::filesystem::is_directory(staticLibraryProjectDestinationPath))
	{
		std::cout << "Directory with the name \"" << staticLibraryProjectName << "\" already exist on the current directory\n";
		return;
	}

	boost::filesystem::copy(staticLibraryProjectTemplatePath, staticLibraryProjectDestinationPath, boost::filesystem::copy_options::recursive);
    
    if (isPartOfAWorkspace)
	{
        boost::filesystem::rename(staticLibraryProjectDestinationPath / "include" / "StaticLibraryProjectTemplate", staticLibraryProjectDestinationPath / "include" / staticLibraryProjectName);
        filesystem_utils::findAndReplaceTextFile(staticLibraryProjectDestinationPath / "src" / "Computer.cpp", "StaticLibraryProjectTemplate", staticLibraryProjectName);
    }
    else
    {
        boost::filesystem::rename(staticLibraryProjectDestinationPath / "include" / "StaticLibraryProjectWithoutWorkspaceTemplate", staticLibraryProjectDestinationPath / "include" / staticLibraryProjectName);
        filesystem_utils::findAndReplaceTextFile(staticLibraryProjectDestinationPath / "src" / "Computer.cpp", "StaticLibraryProjectWithoutWorkspaceTemplate", staticLibraryProjectName);
    }
    
    if (isPartOfAWorkspace)
	{
		std::string executableProjectReferenceStr = std::format("\nadd_subdirectory({})", staticLibraryProjectName);
		std::string workspaceCMakeLists = filesystem_utils::fileReadText(boost::filesystem::current_path() / "CMakeLists.txt");

		filesystem_utils::fileWriteText(boost::filesystem::current_path() / "CMakeLists.txt", workspaceCMakeLists + executableProjectReferenceStr);
		filesystem_utils::findAndReplaceTextFile(staticLibraryProjectDestinationPath / "CMakeLists.txt", "StaticLibraryProjectTemplate", staticLibraryProjectName);
	}
	else
	{
		filesystem_utils::findAndReplaceTextFile(staticLibraryProjectDestinationPath / "CMakeLists.txt", "StaticLibraryProjectWithoutWorkspaceTemplate", staticLibraryProjectName);

		if (cmakeMinimumRequired != "3.20")
		{
			filesystem_utils::findAndReplaceTextFile(staticLibraryProjectDestinationPath / "CMakeLists.txt", "cmake_minimum_required(VERSION 3.20)", "cmake_minimum_required(VERSION " + cmakeMinimumRequired + ")");
		}

		if (cppVersion != "20")
		{
			filesystem_utils::findAndReplaceTextFile(staticLibraryProjectDestinationPath / "CMakeLists.txt", "set(CMAKE_CXX_STANDARD 20)", "set(CMAKE_CXX_STANDARD " + cppVersion + ")");
		}

		if (version != "0.0.1")
		{
			filesystem_utils::findAndReplaceTextFile(staticLibraryProjectDestinationPath / "CMakeLists.txt", "project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)", "project(${PROJECT_NAME} VERSION " + version + " LANGUAGES CXX)");
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

		std::string projectCMakeLists = filesystem_utils::fileReadText(staticLibraryProjectDestinationPath / "CMakeLists.txt");
		filesystem_utils::fileWriteText(staticLibraryProjectDestinationPath / "CMakeLists.txt", projectCMakeLists + assetsCMakeStr);

		boost::filesystem::create_directory(staticLibraryProjectDestinationPath / "assets");
	}
}