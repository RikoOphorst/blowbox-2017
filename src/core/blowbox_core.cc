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
		if (user_procedure_run_)
		{
			user_procedure_run_();
		}

		while (IsBlowboxAlive())
		{
			if (user_procedure_update_)
			{
				user_procedure_update_();
			}

			if (user_procedure_post_update_)
			{
				user_procedure_post_update_();
			}

			if (user_procedure_render_)
			{
				user_procedure_render_();
			}

			if (user_procedure_post_render_)
			{
				user_procedure_post_render_();
			}
		}

		if (user_procedure_shutdown_)
		{
			user_procedure_shutdown_();
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
	void BlowboxCore::SetRunProcedure(const eastl::function<void(void)>& run_procedure)
	{
		user_procedure_run_ = run_procedure;
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
	
	//------------------------------------------------------------------------------------------------------
	void BlowboxCore::SetShutdownProcedure(const eastl::function<void(void)>& shutdown_procedure)
	{
		user_procedure_shutdown_ = shutdown_procedure;
	}
}