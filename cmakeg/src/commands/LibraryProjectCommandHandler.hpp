#pragma once

#include "CommandHandler.hpp"

namespace cmakeg::commands
{
	class LibraryProjectCommandHandler : public CommandHandler
	{
	public:
        std::string libraryProjectName;
		std::string cmakeMinimumRequired = "3.20";
		std::string cppVersion = "20";
		std::string version = "0.0.1";
		bool isAddAssets = false;
    
    public:
		LibraryProjectCommandHandler();

        void execute() override;
	};
}