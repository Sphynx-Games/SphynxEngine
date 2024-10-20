#ifdef SPX_PLATFORM_WINDOWS
#include "spxpch.h"
#include "Dialogs/FileDialog.h"

#include <Windows.h>
#include <commdlg.h>
#include "Core/Application.h"
#include "Renderer/Window.h"
#include "Platform/Windows/WindowsWindow.h"


namespace Sphynx
{
	std::filesystem::path FileDialog::Open(const wchar_t* filter)
	{
		OPENFILENAMEW ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAMEW));

		WCHAR buffer[MAX_PATH]{ 0 };

		ofn.lStructSize = sizeof(OPENFILENAMEW);
		ofn.hwndOwner = static_cast<const WindowsWindow*>(Application::GetInstance()->GetWindow())->GetHWND();
		ofn.lpstrFile = buffer;
		ofn.nMaxFile = sizeof(buffer);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = nullptr;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameW(&ofn) == TRUE)
			return std::filesystem::path{ ofn.lpstrFile };

		return std::filesystem::path{};
	}

	std::filesystem::path FileDialog::Save(const wchar_t* filter)
	{
		OPENFILENAMEW ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));

		WCHAR buffer[MAX_PATH]{ 0 };

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = static_cast<const WindowsWindow*>(Application::GetInstance()->GetWindow())->GetHWND();
		ofn.lpstrFile = buffer;
		ofn.nMaxFile = sizeof(buffer);

		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrInitialDir = nullptr;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = wcschr(filter, L'\0') + 1;

		if (GetSaveFileNameW(&ofn) == TRUE)
			return std::filesystem::path{ ofn.lpstrFile };

		return std::filesystem::path{};
	}
}
#endif