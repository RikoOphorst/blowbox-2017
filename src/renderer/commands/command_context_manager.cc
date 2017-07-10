#include "command_context_manager.h"

#include "command_context.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	CommandContextManager::CommandContextManager()
	{
		
	}
	
	//------------------------------------------------------------------------------------------------------
	CommandContextManager::~CommandContextManager()
	{
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < contexts_pool_[i].size(); j++)
            {
                delete contexts_pool_[i][j];
            }
        }
	}

	//------------------------------------------------------------------------------------------------------
	void CommandContextManager::Startup()
	{

	}
	
	//------------------------------------------------------------------------------------------------------
	CommandContext* CommandContextManager::RequestContext(D3D12_COMMAND_LIST_TYPE type)
	{
		auto& available_contexts = available_contexts_[type];

		CommandContext* ret = nullptr;

		if (available_contexts.empty())
		{
			ret = new CommandContext(type);
			contexts_pool_[type].push_back(ret);
			ret->Initialize();
		}
		else
		{
			ret = available_contexts.front();
			available_contexts.pop();
			ret->Reset();
		}

		assert(ret != nullptr);
		assert(ret->type_ == type);

		return ret;
	}
	
	//------------------------------------------------------------------------------------------------------
	void CommandContextManager::DiscardContext(CommandContext* context)
	{
		assert(context != nullptr);
		available_contexts_[context->type_].push(context);
	}
}