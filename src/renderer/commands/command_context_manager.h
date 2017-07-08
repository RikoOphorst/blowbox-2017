#pragma once

namespace blowbox
{
	class CommandContext;

	class CommandContextManager
	{
	public:
		CommandContextManager();
		~CommandContextManager();

		void Startup();

		CommandContext* RequestContext(D3D12_COMMAND_LIST_TYPE type);
		void DiscardContext(CommandContext* context);
	private:
		std::vector<CommandContext*> contexts_pool_[4];
		std::queue<CommandContext*> available_contexts_[4];
	};
}