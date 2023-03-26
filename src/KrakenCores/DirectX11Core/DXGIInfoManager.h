// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include <vector>
#include <string>

#include "KrakenWindows.h"

#include <wrl.h>
#include <dxgidebug.h>

namespace KrakenGraphics
{
	class DXGIInfoManager
	{
	public:
		DXGIInfoManager();
		~DXGIInfoManager() = default;
		DXGIInfoManager(const DXGIInfoManager&) = delete;
		DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;
		void Set() noexcept;
		std::vector<std::wstring> GetMessages() const;
	private:
		unsigned long long next = 0u;
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
	};
}
