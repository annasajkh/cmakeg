#include <iostream>
#include <CLI/CLI.hpp>
#include "commands/WorkspaceCommandHandler.hpp"
#include "commands/ExecutableProjectCommandHandler.hpp"
#include "commands/ExecutableWorkspaceProjectCommandHandler.hpp"

using namespace cmakeg::commands;

int main(int argc, char* argv[])
{
    CLI::App app { "Generate cmake projects based on a template" };
    argv = app.ensure_utf8(argv);

    auto workspaceCommandHandler = std::make_shared<WorkspaceCommandHandler>();
    auto executableProjectCommandHandler = std::make_shared<ExecutableProjectCommandHandler>();
    auto executableWorkspaceProjectCommandHandler = std::make_shared<ExecutableWorkspaceProjectCommandHandler>();
    
    CLI::App* workspaceSubCommand = app.add_subcommand("workspace", "Create a new workspace");
    workspaceSubCommand->add_option("--name", workspaceCommandHandler->workspaceName, "The name of the workspace")->required();
    workspaceSubCommand->add_option("--cmake-minimum-required", workspaceCommandHandler->cmakeMinimumRequired, "The cmake minimum version required for the workspace")->default_str("3.20");
    workspaceSubCommand->add_option("--cpp-version", workspaceCommandHandler->cppVersion, "The c++ standard version")->default_str("20");
    workspaceSubCommand->add_option("--version", workspaceCommandHandler->version, "The workspace version")->default_str("0.0.1");

    CLI::App* executableProjectSubCommand = app.add_subcommand("executable-project", "Create a new executable project");
    executableProjectSubCommand->add_option("--name", executableProjectCommandHandler->executableProjectName, "The name of the project")->required();
    executableProjectSubCommand->add_option("--cmake-minimum-required", executableProjectCommandHandler->cmakeMinimumRequired, "The cmake minimum version required for the project (this option only work if this project is not part of a workspace)")->default_str("3.20");
    executableProjectSubCommand->add_option("--cpp-version", executableProjectCommandHandler->cppVersion, "The c++ standard version (this option only work if this project is not part of a workspace)")->default_str("20");
    executableProjectSubCommand->add_option("--version", executableProjectCommandHandler->version, "The executable project version (this option only work if this project is not part of a workspace)")->default_str("0.0.1");
    executableProjectSubCommand->add_flag("--add-assets", executableProjectCommandHandler->isAddAssets, "Add assets directory, this directory will get copied to the executable directory when building useful if the program need to load external resources");

    CLI::App* executableWorkspaceProjectSubCommand = app.add_subcommand("executable-workspace-project", "Create a new workspace with executable project inside of it");
    executableWorkspaceProjectSubCommand->add_option("--project-name", executableWorkspaceProjectCommandHandler->executableProjectName, "The name of the workspace")->required();
    executableWorkspaceProjectSubCommand->add_option("--workspace-name", executableWorkspaceProjectCommandHandler->executableWorkspaceName, "The name of the project")->required();
    executableWorkspaceProjectSubCommand->add_option("--cmake-minimum-required", executableWorkspaceProjectCommandHandler->cmakeMinimumRequired, "The cmake minimum version required")->default_str("3.20");
    executableWorkspaceProjectSubCommand->add_option("--cpp-version", executableWorkspaceProjectCommandHandler->cppVersion, "The c++ standard version")->default_str("20");
    executableWorkspaceProjectSubCommand->add_option("--version", executableWorkspaceProjectCommandHandler->version, "The executable version")->default_str("0.0.1");
    executableWorkspaceProjectSubCommand->add_flag("--add-assets", executableWorkspaceProjectCommandHandler->isAddAssets, "Add assets directory, this directory will get copied to the executable directory when building useful if the program need to load external resources");

    CLI11_PARSE(app, argc, argv);

    if (app.got_subcommand("workspace"))
    {
        workspaceCommandHandler->execute();
    }
    else if (app.got_subcommand("executable-project"))
    {
        executableProjectCommandHandler->execute();
    }
    else if (app.got_subcommand("executable-workspace-project"))
    {
        executableWorkspaceProjectCommandHandler->execute();
    }
}