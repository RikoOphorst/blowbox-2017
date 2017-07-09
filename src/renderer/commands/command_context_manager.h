#pragma once

#include "core/eastl_required.h"
#include <EASTL/vector.h>
#include <EASTL/queue.h>
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
		eastl::vector<CommandContext*> contexts_pool_[4];
        eastl::queue<CommandContext*> available_contexts_[4];
	};
}