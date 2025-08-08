#include "WorkspaceCommandHandler.hpp"
#include <iostream>
#include <format>
#include "../utils/filesystem_utils.hpp"

using namespace cmakeg::commands;

WorkspaceCommandHandler::WorkspaceCommandHandler() : CommandHandler(), workspaceTemplatePath(executablePath / "assets" / "WorkspaceTemplate")
{
	
}

void WorkspaceCommandHandler::execute()
{
	workspaceDestinationPath = boost::filesystem::current_path() / workspaceName;

	if (boost::filesystem::is_directory(workspaceDestinationPath))
	{
		std::cout << "Directory with the name \"" << workspaceName << "\" already exist on the current directory\n";
		return;
	}

	boost::filesystem::copy(workspaceTemplatePath, workspaceDestinationPath, boost::filesystem::copy_options::recursive);

	filesystem_utils::findAndReplaceTextFile(workspaceDestinationPath / "CMakeLists.txt", "WorkspaceTemplate", workspaceName);

	if (cmakeMinimumRequired != "3.20")
	{
		filesystem_utils::findAndReplaceTextFile(workspaceDestinationPath / "CMakeLists.txt", "cmake_minimum_required(VERSION 3.20)", "cmake_minimum_required(VERSION " + cmakeMinimumRequired + ")");
	}

	if (cppVersion != "20")
	{
		filesystem_utils::findAndReplaceTextFile(workspaceDestinationPath / "CMakeLists.txt", "set(CMAKE_CXX_STANDARD 20)", "set(CMAKE_CXX_STANDARD " + cppVersion + ")");
	}

	if (version != "0.0.1")
	{
		filesystem_utils::findAndReplaceTextFile(workspaceDestinationPath / "CMakeLists.txt", "project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)", "project(${PROJECT_NAME} VERSION " + version + " LANGUAGES CXX)");
	}
}