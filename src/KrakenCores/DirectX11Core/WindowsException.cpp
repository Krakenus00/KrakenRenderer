// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#include "WindowsException.h"

#include <numeric>
#include <sstream>
#include <format>

namespace KrakenGraphics
{
	WindowsException::WindowsException(
		HRESULT hr, 
		const std::wstring& description, 
		const std::vector<std::wstring>& infoMessages, 
		size_t line, const std::wstring& file) noexcept
		: KrakenException(description, line, file)
		, _hr(hr)
		, _info(infoMessages)
	{}

	std::wstring WindowsException::WinAPIErr() const
	{
		std::wstring errDesc{ 512, L'\0' };
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, _hr, 0, errDesc.data(), static_cast<DWORD>(errDesc.size()), nullptr);
		return errDesc;
	}

	std::wstring WindowsException::ToString() const
	{
		constexpr std::wstring_view descTemplate =
			L"KrakenGraphics::WindowsException\n"
			L"[Error Code] 0x{:#08x} ({:d})\n"
			L"[Description] {:s}\n";
		constexpr std::wstring_view infoTemplate =
			L"\n[Error Info]\n"
			L"{:s}\n";
		constexpr std::wstring_view commentTemplate =
			L"\n[Comment] {:s}\n";
		constexpr std::wstring_view originTemplate =
			L"\n[File] {:s}\n"
			L"[Line] {:d}\n";


		std::wstring winErrDesc = static_cast<bool>(_hr) ? WinAPIErr() : L"The problematic function returns void.";

		std::wstring output = std::format(descTemplate, _hr, _hr, winErrDesc);

		if (!_info.empty())
			output += std::format(infoTemplate, std::accumulate(_info.begin(), _info.end(), std::wstring{}));
		if (!_description.empty())
			output += std::format(commentTemplate, _description);
		if (_line)
			output += std::format(originTemplate, _file, _line);

		return output;
	}
}
