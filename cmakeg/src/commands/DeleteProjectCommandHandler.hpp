#pragma once

namespace cmakeg::commands
{
	class DeleteProjectCommandHandler : public CommandHandler
	{
	public:
		std::string name;

		DeleteProjectCommandHandler();

		void execute() override;
	};
}