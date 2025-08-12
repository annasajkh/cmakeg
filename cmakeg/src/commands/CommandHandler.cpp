#include "CommandHandler.hpp"
#include <iostream>
#include <boost/dll.hpp>
#include "../utils/filesystem_utils.hpp"

using namespace cmakeg::commands;

CommandHandler::CommandHandler()
{
	executablePath = boost::dll::program_location().parent_path();
}

WorkspaceIntegrityStatus CommandHandler::checkWorkspaceIntegrity()
{
    bool currentDirHaveCMakeList = boost::filesystem::exists(boost::filesystem::current_path() / "CMakeLists.txt");
	bool isPartOfAWorkspace = currentDirHaveCMakeList;

	if (isPartOfAWorkspace)
	{
		std::string workspaceCMakeFile = filesystem_utils::fileReadText(boost::filesystem::current_path() / "CMakeLists.txt");

		isPartOfAWorkspace = isPartOfAWorkspace && (workspaceCMakeFile.find("# WORKSPACE IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS WORKSPACE") != std::string::npos);
	}

	if (!isPartOfAWorkspace && currentDirHaveCMakeList)
	{
        return WorkspaceIntegrityStatus::ContainsCMakeButNotIdentifier;
	}
    else if(!isPartOfAWorkspace && !currentDirHaveCMakeList)
    {
        return WorkspaceIntegrityStatus::NotExist;
    }

    return WorkspaceIntegrityStatus::Valid;
}

ProjectIntegrityStatus CommandHandler::checkProjectIntegrity(std::string projectName, std::vector<std::string>& identifiers)
{
    if (projectName.find(".") != std::string::npos || projectName.find("/") != std::string::npos || projectName.find("\\") != std::string::npos)
    {
        std::cout << "Name of the project shouldn't contains . or / or \\\n";
        return ProjectIntegrityStatus::Skip;
    }

	if (!(boost::filesystem::exists(boost::filesystem::current_path() / projectName)))
	{		
        std::cout << "Project with the name \"" << projectName << "\" doesn't exist on the current directory";
		return ProjectIntegrityStatus::Skip;
	}

    bool projectDirHaveCMakeList = boost::filesystem::exists(boost::filesystem::current_path() / projectName / "CMakeLists.txt");
	bool isAProject = projectDirHaveCMakeList;

	if (isAProject)
	{
		std::string projectCMakeFile = filesystem_utils::fileReadText(boost::filesystem::current_path() / projectName / "CMakeLists.txt");

        bool containsIdentifier = false;

        for (size_t i = 0; i < identifiers.size(); i++)
        {
            containsIdentifier = containsIdentifier || (projectCMakeFile.find(identifiers[i]) != std::string::npos);
        }
        
		isAProject = isAProject && containsIdentifier;
	}

	if (!isAProject && projectDirHaveCMakeList)
	{
        return ProjectIntegrityStatus::ContainsCMakeButNotIdentifier;
	}
    else if(!isAProject && !projectDirHaveCMakeList)
    {
        return ProjectIntegrityStatus::NotExist;
    }

    return ProjectIntegrityStatus::Valid;
}