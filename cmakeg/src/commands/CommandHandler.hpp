#pragma once

#include <string>
#include <boost/filesystem.hpp>

namespace cmakeg::commands
{
    enum class WorkspaceIntegrityStatus
    {
        NotExist,
        ContainsCMakeButNotIdentifier,
        Valid,
    };

    enum class ProjectIntegrityStatus
    {
        NotExist,
        ContainsCMakeButNotIdentifier,
        Valid,
        Skip
    };
    
	class CommandHandler
	{
	public:
		CommandHandler();
		virtual void execute() = 0;

	protected:
		boost::filesystem::path executablePath;

        WorkspaceIntegrityStatus checkWorkspaceIntegrity();
        ProjectIntegrityStatus checkProjectIntegrity(std::string projectName, std::vector<std::string>& identifiers);
	};
}