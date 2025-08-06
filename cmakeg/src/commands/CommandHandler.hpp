#pragma once

#include <string>
#include <boost/filesystem.hpp>

namespace cmakeg::commands
{
	class CommandHandler
	{
	public:
		CommandHandler();
		virtual void execute() = 0;

	protected:
		boost::filesystem::path workingDirPath;
		boost::filesystem::path executablePath;
	};
}