#pragma once

#include "CommandHandler.hpp"
#include <string>
#include <boost/filesystem/path.hpp>

namespace cmakeg::commands
{
	class WorkspaceCommandHandler : public CommandHandler
	{
	public:
		std::string workspaceName;
		std::string cmakeMinimumRequired = "3.20";
		std::string cppVersion = "20";
		std::string version = "0.0.1";

	public:
		WorkspaceCommandHandler();

		void execute() override;
	};
}