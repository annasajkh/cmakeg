#include "ExecutableWorkspaceProjectCommandHandler.hpp"

using namespace cmakeg::commands;

ExecutableWorkspaceProjectCommandHandler::ExecutableWorkspaceProjectCommandHandler() : CommandHandler(), workspaceCommandHandler(), executableProjectCommandHandler()
{

}

void ExecutableWorkspaceProjectCommandHandler::execute()
{
	workspaceCommandHandler.workspaceName = workspaceName;
	workspaceCommandHandler.cppVersion = cppVersion;
	workspaceCommandHandler.cmakeMinimumRequired = cmakeMinimumRequired;
	workspaceCommandHandler.version = version;

	executableProjectCommandHandler.executableProjectName = executableProjectName;
	executableProjectCommandHandler.isAddAssets = isAddAssets;

	workspaceCommandHandler.execute();
	
	boost::filesystem::path workspacePath = boost::filesystem::current_path() / workspaceName;
	boost::filesystem::current_path(workspacePath);

	executableProjectCommandHandler.execute();
}