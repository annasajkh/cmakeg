#include "CommandHandler.hpp"
#include "DeleteProjectCommandHandler.hpp"
#include "../utils/filesystem_utils.hpp"
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace cmakeg::commands;

DeleteProjectCommandHandler::DeleteProjectCommandHandler() : CommandHandler()
{

}

void DeleteProjectCommandHandler::execute()
{
    WorkspaceIntegrityStatus workspaceIntegrityStatus = checkWorkspaceIntegrity();

    switch (workspaceIntegrityStatus)
    {
    case WorkspaceIntegrityStatus::NotExist:
        std::cout << "Cannot find the workspace\n";
        return;
    case WorkspaceIntegrityStatus::ContainsCMakeButNotIdentifier:
		std::cout << "Cannot find the workspace but found CMakeList.txt, does workspace identifier get removed?\n";
        return;
    }

    std::vector<std::string> identifiers = {
        "# EXECUTABLE PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS PROJECT",
        "# STATIC LIBRARY PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS LIBRARY",
        "# DYNAMIC LIBRARY PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS LIBRARY"
    };

    ProjectIntegrityStatus projectIntegrityStatus = checkProjectIntegrity(name, identifiers);

    switch (projectIntegrityStatus)
    {
    case ProjectIntegrityStatus::Skip:
        return;
    case ProjectIntegrityStatus::NotExist:
        std::cout << "Cannot find the project\n";
        return;
    case ProjectIntegrityStatus::ContainsCMakeButNotIdentifier:
        std::cout << "Cannot find the project identifier but found CMakeList.txt, is project identifier get removed?\n";
        return;
    }

	boost::filesystem::remove_all(boost::filesystem::current_path() / name);
	filesystem_utils::findAndReplaceTextFile(boost::filesystem::current_path() / "CMakeLists.txt", "\nadd_subdirectory(" + name + ")", "");
}