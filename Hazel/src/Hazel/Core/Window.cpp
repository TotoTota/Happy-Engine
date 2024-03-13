// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#include "hzpch.h"
#include "Hazel/Core/Window.h"

#ifdef HZ_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Hazel
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef HZ_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		HZ_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}

}