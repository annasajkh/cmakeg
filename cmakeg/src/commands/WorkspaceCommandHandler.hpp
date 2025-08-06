#pragma once

#include "CommandHandler.hpp"
#include <string>
#include <boost/filesystem/path.hpp>

namespace cmakeg::commands
{
	class WorkspaceCommandHandler : public CommandHandler
	{
	public:
		WorkspaceCommandHandler();

		std::string workspaceName;
		std::string cmakeMinimumRequired = "3.20";
		std::string cppVersion = "20";
		std::string version = "0.0.1";

		void execute() override;

	private:
		boost::filesystem::path workspaceTemplatePath;
		boost::filesystem::path workspaceDestinationPath;
	};
}