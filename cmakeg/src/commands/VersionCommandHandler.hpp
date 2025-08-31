#pragma once

#include <string>
#include "CommandHandler.hpp"

namespace cmakeg::commands
{
    class VersionCommandHandler : public CommandHandler
    {
    public:
        std::string version;

        VersionCommandHandler();

        void execute() override;
    };
}