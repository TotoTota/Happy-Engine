// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com



#include "hzpch.h"
#include "Hazel/Renderer/RenderCommand.h"

namespace Hazel {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}