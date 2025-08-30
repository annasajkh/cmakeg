#include <iostream>
#include <format>
#include "../utils/filesystem_utils.hpp"
#include <boost/algorithm/algorithm.hpp>
#include "StaticLibraryProjectCommandHandler.hpp"
#include "boost/algorithm/string.hpp"
#include "../utils/global.hpp"

using namespace cmakeg::commands;

StaticLibraryProjectCommandHandler::StaticLibraryProjectCommandHandler() : CommandHandler()
{

}

void StaticLibraryProjectCommandHandler::execute()
{
    WorkspaceIntegrityStatus workspaceIntegrityStatus = checkWorkspaceIntegrity();

    switch (workspaceIntegrityStatus)
    {
    case WorkspaceIntegrityStatus::NotExist:
        std::cout << "Cannot find a workspace creating an independent static library project\n";
        break;
    case WorkspaceIntegrityStatus::ContainsCMakeButNotIdentifier:
        std::cout << "Cannot find the workspace but found CMakeList.txt, is workspace identifier deleted?\n";
        return;
    }

    
    boost::filesystem::path staticLibraryProjectTemplatePath;

    if (workspaceIntegrityStatus == WorkspaceIntegrityStatus::Valid)
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
    
    if (workspaceIntegrityStatus == WorkspaceIntegrityStatus::Valid)
	{
        boost::filesystem::rename(staticLibraryProjectDestinationPath / "include" / "StaticLibraryProjectTemplate", staticLibraryProjectDestinationPath / "include" / staticLibraryProjectName);
        filesystem_utils::findAndReplaceTextFile(staticLibraryProjectDestinationPath / "src" / "Computer.cpp", "StaticLibraryProjectTemplate", staticLibraryProjectName);
    }
    else
    {
        boost::filesystem::rename(staticLibraryProjectDestinationPath / "include" / "StaticLibraryProjectWithoutWorkspaceTemplate", staticLibraryProjectDestinationPath / "include" / staticLibraryProjectName);
        filesystem_utils::findAndReplaceTextFile(staticLibraryProjectDestinationPath / "src" / "Computer.cpp", "StaticLibraryProjectWithoutWorkspaceTemplate", staticLibraryProjectName);
    }
    
    if (workspaceIntegrityStatus == WorkspaceIntegrityStatus::Valid)
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
		std::string assetsCMakeStrReplaced = global::assetsCMakeStr;

		boost::algorithm::replace_all(assetsCMakeStrReplaced, "ProjectName", staticLibraryProjectName);

		std::string projectCMakeLists = filesystem_utils::fileReadText(staticLibraryProjectDestinationPath / "CMakeLists.txt");
		filesystem_utils::fileWriteText(staticLibraryProjectDestinationPath / "CMakeLists.txt", projectCMakeLists + assetsCMakeStrReplaced);

		boost::filesystem::create_directory(staticLibraryProjectDestinationPath / "assets");
	}
}