#include "CommandHandler.hpp"
#include "DeleteProjectCommandHandler.hpp"
#include "../utils/filesystem_utils.hpp"
#include <iostream>

using namespace cmakeg::commands;

DeleteProjectCommandHandler::DeleteProjectCommandHandler() : CommandHandler()
{

}

void DeleteProjectCommandHandler::execute()
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
        std::cout << "Cannot find a workspace\n";
        return;
    }
	
	if (!(boost::filesystem::exists(boost::filesystem::current_path() / name)))
	{
		std::cout << "Project with the name \"" << name << "\" doesn't exist on the current directory";
		return;
	}

    bool projectDirHaveCMakeList = boost::filesystem::exists(boost::filesystem::current_path() / name / "CMakeLists.txt");
	bool isAProject = projectDirHaveCMakeList;

	if (isAProject)
	{
		std::string projectCMakeFile = filesystem_utils::fileReadText(boost::filesystem::current_path() / name / "CMakeLists.txt");

		isAProject = isAProject && 
			         ((projectCMakeFile.find("# PROJECT INDICATOR DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS PROJECT") != std::string::npos) ||
				     (projectCMakeFile.find("# LIBRARY INDICATOR DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS LIBRARY") != std::string::npos));
	}

	if (!isAProject && projectDirHaveCMakeList)
	{
		std::cout << "Cannot find a project but found CMakeList.txt, is project indicator deleted?\n";
        return;
	}
    else if(!isAProject && !projectDirHaveCMakeList)
    {
        std::cout << "Cannot find a project\n";
        return;
    }

	boost::filesystem::remove_all(boost::filesystem::current_path() / name);
	filesystem_utils::findAndReplaceTextFile(boost::filesystem::current_path() / "CMakeLists.txt", "\nadd_subdirectory(" + name + ")", "");
}