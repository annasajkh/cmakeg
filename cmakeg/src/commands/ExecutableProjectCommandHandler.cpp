#include "ExecutableProjectCommandHandler.hpp"
#include <iostream>
#include <format>
#include "../utils/filesystem_utils.hpp"
#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string/replace.hpp>
#include "../utils/global.hpp"

using namespace cmakeg::commands;

ExecutableProjectCommandHandler::ExecutableProjectCommandHandler() : CommandHandler()
{

}

void ExecutableProjectCommandHandler::execute()
{
    WorkspaceIntegrityStatus workspaceIntegrityStatus = checkWorkspaceIntegrity();

    switch (workspaceIntegrityStatus)
    {
    case WorkspaceIntegrityStatus::NotExist:
        std::cout << "Cannot find the workspace creating an independent executable project\n";
        break;
    case WorkspaceIntegrityStatus::ContainsCMakeButNotIdentifier:
        std::cout << "Cannot find a workspace but found CMakeList.txt, is workspace identifier deleted?\n";
        return;
    }
	
    boost::filesystem::path executableProjectTemplatePath;
	
    if (workspaceIntegrityStatus == WorkspaceIntegrityStatus::Valid)
	{
		executableProjectTemplatePath = executablePath / "assets" / "ExecutableProjectTemplate";
	}
	else
	{
		executableProjectTemplatePath = executablePath / "assets" / "ExecutableProjectWithoutWorkspaceTemplate";
	}

	boost::filesystem::path executableProjectDestinationPath = boost::filesystem::current_path() / executableProjectName;

	if (boost::filesystem::is_directory(executableProjectDestinationPath))
	{
		std::cout << "Directory with the name \"" << executableProjectName << "\" already exist on the current directory\n";
		return;
	}

	boost::filesystem::copy(executableProjectTemplatePath, executableProjectDestinationPath, boost::filesystem::copy_options::recursive);

	if (workspaceIntegrityStatus == WorkspaceIntegrityStatus::Valid)
	{
		std::string executableProjectReferenceStr = std::format("\nadd_subdirectory({})", executableProjectName);
		std::string workspaceCMakeLists = filesystem_utils::fileReadText(boost::filesystem::current_path() / "CMakeLists.txt");

		filesystem_utils::fileWriteText(boost::filesystem::current_path() / "CMakeLists.txt", workspaceCMakeLists + executableProjectReferenceStr);

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
		std::string assetsCMakeStrReplaced = global::assetsCMakeStr;

		boost::algorithm::replace_all(assetsCMakeStrReplaced, "ProjectName", executableProjectName);

		std::string projectCMakeLists = filesystem_utils::fileReadText(executableProjectDestinationPath / "CMakeLists.txt");
		filesystem_utils::fileWriteText(executableProjectDestinationPath / "CMakeLists.txt", projectCMakeLists + assetsCMakeStrReplaced);

		boost::filesystem::create_directory(executableProjectDestinationPath / "assets");
	}
};