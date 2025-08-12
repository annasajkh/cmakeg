#pragma once

#include <string>
#include "CommandHandler.hpp"
#include "WorkspaceCommandHandler.hpp"
#include "ExecutableProjectCommandHandler.hpp"

namespace cmakeg::commands
{
	class ExecutableWorkspaceProjectCommandHandler : public CommandHandler
	{
	public:
		std::string executableProjectName;
		std::string workspaceName;
		std::string cmakeMinimumRequired = "3.20";
		std::string cppVersion = "20";
		std::string version = "0.0.1";
		bool isAddAssets = false;

	private:
		WorkspaceCommandHandler workspaceCommandHandler;
		ExecutableProjectCommandHandler executableProjectCommandHandler;

	public:
		ExecutableWorkspaceProjectCommandHandler();

		void execute() override;
	};
}