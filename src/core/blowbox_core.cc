#include "blowbox_core.h"

#include "util/assert.h"
#include "core/blowbox_config.h"

namespace blowbox
{
	//------------------------------------------------------------------------------------------------------
	BlowboxCore::BlowboxCore()
	{

	}
	
	//------------------------------------------------------------------------------------------------------
	BlowboxCore::BlowboxCore(BlowboxConfig* config) :
		alive_(true),
		config_(config)
	{
		BLOWBOX_ASSERT(config_ != nullptr);
	}

	//------------------------------------------------------------------------------------------------------
	BlowboxCore::~BlowboxCore()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void BlowboxCore::Run()
	{
		while (IsBlowboxAlive())
		{
			user_procedure_update_();
			user_procedure_post_update_();

			user_procedure_render_();
			user_procedure_post_render_();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void BlowboxCore::Shutdown()
	{
		alive_ = false;
	}

	//------------------------------------------------------------------------------------------------------
	bool BlowboxCore::IsBlowboxAlive()
	{
		return alive_;
	}

	//------------------------------------------------------------------------------------------------------
	void BlowboxCore::SetUpdateProcedure(const eastl::function<void(void)>& update_procedure)
	{
		user_procedure_update_ = update_procedure;
	}

	//------------------------------------------------------------------------------------------------------
	void BlowboxCore::SetPostUpdateProcedure(const eastl::function<void(void)>& post_update_procedure)
	{
		user_procedure_post_update_ = post_update_procedure;
	}

	//------------------------------------------------------------------------------------------------------
	void BlowboxCore::SetRenderProcedure(const eastl::function<void(void)>& render_procedure)
	{
		user_procedure_render_ = render_procedure;
	}

	//------------------------------------------------------------------------------------------------------
	void BlowboxCore::SetPostRenderProcedure(const eastl::function<void(void)>& post_render_procedure)
	{
		user_procedure_post_render_ = post_render_procedure;
	}
}