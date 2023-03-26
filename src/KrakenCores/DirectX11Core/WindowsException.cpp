// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#include "WindowsException.h"

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
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, _hr, 0, errDesc.data(), errDesc.size(), nullptr);
	}

	std::wstring WindowsException::ToString() const
	{
		static const std::wstring descTemplate =
			L"KrakenGraphics::WindowsException\n"
			L"[Error Code] 0x{:#08x} ({:d})\n"
			L"[Description] {:s}\n";
		static const std::wstring infoTemplate =
			L"\n[Error Info]\n"
			L"{:s}\n";
		static const std::wstring descTemplate =
			L"\n[Comment] {:s}\n";
		static const std::wstring originTemplate =
			L"\n[File] {:s}\n"
			L"[Line] {:d}\n";

		std::wstring errTemplate = descTemplate;
		if (!_info.empty()) errTemplate += infoTemplate;
		if (!_description.empty()) errTemplate += descTemplate;
		if (_line) errTemplate += originTemplate;

		std::wstring infoUnpacked(_info.begin(), _info.end());
		std::wstring winErrDesc = _hr ? WinAPIErr() : L"The problematic function returns void.";

		return std::format(errTemplate, _hr, _hr, WinAPIErr(), infoUnpacked, _file, _line, _description);
	}
}
