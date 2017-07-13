#pragma once

#include "util/vector.h"
#include "util/queue.h"
#include "renderer/d3d12_includes.h"

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
		Vector<CommandContext*> contexts_pool_[4];
        Queue<CommandContext*> available_contexts_[4];
	};
}