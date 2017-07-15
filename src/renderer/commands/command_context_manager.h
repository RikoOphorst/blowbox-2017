#pragma once

#include "util/vector.h"
#include "util/queue.h"
#include "renderer/d3d12_includes.h"

namespace blowbox
{
	class CommandContext;

    /**
    * The CommandContextManager manages all the command contexts in the engine.
    * CommandContext instances are never cleared during runtime, they are only
    * generated and placed into this pool. This makes the management of these
    * CommandContext's a lot more efficient, because creating a new CommandContext
    * becomes a lot more quick because of the low overhead costs of this pooling
    * strategy. Ideally, as the user, you never have to interact with this manager,
    * but instead create new CommandContext's via the CommandContext::Begin()
    * methods.
    *
    * @brief Manages the pool of available CommandContexts.
    */
	class CommandContextManager
	{
	public:
        /** @brief Constructs the CommandContextManager. */
		CommandContextManager();
        /** @brief Destructs the CommandContextManager. */
		~CommandContextManager();

        /** @brief Starts up the CommandContextManager. */
		void Startup();

        /**
        * @brief Allows you to request a new CommandContext, which will be initialized and ready for use when it is returned.
        * @param[in] type The type of CommandContext you want to request
        * @returns A CommandContext for the requested command list type.
        */
		CommandContext* RequestContext(D3D12_COMMAND_LIST_TYPE type);

        /**
        * @brief By calling this function, you indicate that the CommandContext is free for re-use again. It will be released into the pool of available contexts.
        * @param[in] context The context you want to release back into the available context pool.
        */
		void DiscardContext(CommandContext* context);
	private:
		Vector<CommandContext*> contexts_pool_[4];      //!< The complete pool of contexts. This is 4 different arrays of arrays. Every top level array is an array of contexts of a certain type.
        Queue<CommandContext*> available_contexts_[4];  //!< The available pool of contexts. Any context in these arrays is a context that is ready to be re-used.
	};
}