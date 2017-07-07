#include <iostream>

#include "core/blowbox_core.h"
#include "core/blowbox_config.h"

#include "core/eastl_required.h"
#include <EASTL/string.h>
#include <assimp/postprocess.h>
#include <GLFW/glfw3.h>

using namespace blowbox;

int ticks = 0; 
BlowboxCore blowbox_instance;

void Update()
{
	

	if (++ticks > 10000)
	{
		blowbox_instance.Shutdown();
	}
}

void PostUpdate()
{
	
}

void Render()
{
	
}

void PostRender()
{
	
}

int main(int argc, char** argv)
{
	BlowboxConfig config;
	config.window_title = "dja moedahs toeltip";
	config.window_resolution = Resolution(1280, 720);

	blowbox_instance = BlowboxCore(&config);

	blowbox_instance.SetUpdateProcedure(Update);
	blowbox_instance.SetPostUpdateProcedure(PostUpdate);
	blowbox_instance.SetRenderProcedure(Render);
	blowbox_instance.SetPostRenderProcedure(PostRender);

	blowbox_instance.Run();

	return 0;
}