#include "VersionCommandHandler.hpp"
#include <iostream>

using namespace cmakeg::commands;

VersionCommandHandler::VersionCommandHandler()
{

}

void VersionCommandHandler::execute()
{
    std::cout << "cmakeg " << version << std::endl;
}