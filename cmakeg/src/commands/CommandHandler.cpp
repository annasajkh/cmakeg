#include "CommandHandler.hpp"
#include <iostream>
#include <boost/dll.hpp>

using namespace cmakeg::commands;

CommandHandler::CommandHandler()
{
	workingDirPath = boost::filesystem::current_path();
	executablePath = boost::dll::program_location().parent_path();
}