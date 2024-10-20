#pragma once 

#include <filesystem>


namespace Sphynx
{
	struct FileDialog
	{
		[[nodiscard]]static std::filesystem::path Open(const wchar_t* filter = L"All Files (*)\0*.*\0");
		[[nodiscard]]static std::filesystem::path Save(const wchar_t* filter = L"All Files (*)\0*.*\0");
	};
}
