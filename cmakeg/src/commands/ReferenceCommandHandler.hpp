#pragma once

#include <string>
#include "CommandHandler.hpp"

namespace cmakeg::commands
{
    class ReferenceCommandHandler : public CommandHandler
    {
    public:
        std::string librarySourceName;
        std::string projectDestinationName;

        ReferenceCommandHandler();

        void execute() override;

    };
}