// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include "KrakenException.h"

#include <vector>

#include "KrakenWindows.h"

namespace KrakenGraphics
{
    class WindowsException: virtual public KrakenException
    {
		using KrakenException::KrakenException;
	public: // Constructors
		WindowsException(
			HRESULT hr,
			const std::wstring& description = L"",
			const std::vector<std::wstring>& infoMessages = {},
			size_t line = 0,
			const std::wstring& file = L"") noexcept;

	public: // Members
		HRESULT hr() const noexcept { return _hr; }
		const std::vector<std::wstring>& info() const noexcept { return _info; }
		std::wstring WinAPIErr() const;
		std::wstring ToString() const;

        virtual std::wstring what() const noexcept { return ToString(); }

	protected: // Fields
		HRESULT				      _hr;
		std::vector<std::wstring> _info;
    };
}
