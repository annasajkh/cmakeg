#include "WorkspaceCommandHandler.hpp"
#include <iostream>
#include <format>
#include "../utils/filesystem_utils.hpp"

using namespace cmakeg::commands;

WorkspaceCommandHandler::WorkspaceCommandHandler() : CommandHandler()
{
	
}

void WorkspaceCommandHandler::execute()
{
    boost::filesystem::path workspaceTemplatePath = executablePath / "assets" / "WorkspaceTemplate";
	
    if (boost::filesystem::is_directory(boost::filesystem::current_path() / workspaceName))
	{
		std::cout << "Directory with the name \"" << workspaceName << "\" already exist on the current directory\n";
		return;
	}

	boost::filesystem::copy(workspaceTemplatePath, boost::filesystem::current_path() / workspaceName, boost::filesystem::copy_options::recursive);

	filesystem_utils::findAndReplaceTextFile(boost::filesystem::current_path() / workspaceName / "CMakeLists.txt", "WorkspaceTemplate", workspaceName);

	if (cmakeMinimumRequired != "3.20")
	{
		filesystem_utils::findAndReplaceTextFile(boost::filesystem::current_path() / workspaceName / "CMakeLists.txt", "cmake_minimum_required(VERSION 3.20)", "cmake_minimum_required(VERSION " + cmakeMinimumRequired + ")");
	}

	if (cppVersion != "20")
	{
		filesystem_utils::findAndReplaceTextFile(boost::filesystem::current_path() / workspaceName / "CMakeLists.txt", "set(CMAKE_CXX_STANDARD 20)", "set(CMAKE_CXX_STANDARD " + cppVersion + ")");
	}

	if (version != "0.0.1")
	{
		filesystem_utils::findAndReplaceTextFile(boost::filesystem::current_path() / workspaceName / "CMakeLists.txt", "project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)", "project(${PROJECT_NAME} VERSION " + version + " LANGUAGES CXX)");
	}
}