#include "CommandHandler.hpp"
#include <iostream>
#include <boost/dll.hpp>

using namespace cmakeg::commands;

CommandHandler::CommandHandler()
{
	executablePath = boost::dll::program_location().parent_path();
}