#pragma once
#include "CommandHandler.hpp"

namespace cmakeg::commands
{
	class ExecutableProjectCommandHandler : public CommandHandler
	{
	public:
		ExecutableProjectCommandHandler();

		std::string executableProjectName;
		std::string cmakeMinimumRequired = "3.20";
		std::string cppVersion = "20";
		std::string version = "0.0.1";
		bool isAddAssets = false;

		void execute() override;

	private:
		boost::filesystem::path executableProjectTemplatePath;
		boost::filesystem::path executableProjectDestinationPath;

		bool isPartOfAWorkspace;
	};
}