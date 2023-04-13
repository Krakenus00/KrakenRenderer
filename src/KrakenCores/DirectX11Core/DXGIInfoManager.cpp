// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#include "DXGIInfoManager.h"

#include "DirectXHelpers.h"
#include "WindowsException.h"

#include <memory>

#pragma comment(lib, "dxguid.lib")

namespace KrakenGraphics
{
	DXGIInfoManager::DXGIInfoManager()
	{
		// Define function signature of DXGIGetDebugInterface.
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		// Load the dll that contains the function DXGIGetDebugInterface.
		const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (!hModDxgiDebug)
			throw WinException(GetLastError());

		// Get address of DXGIGetDebugInterface in dll
		const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"));
		if (!DxgiGetDebugInterface)
			throw WinException(GetLastError());

		HRESULT hr;
		WinCheck(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &pDxgiInfoQueue));
	}

	void DXGIInfoManager::Set() noexcept
	{
		// set the index (next) so that the next all to GetMessages()
		// will only get errors generated after this call
		next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}

	std::vector<std::wstring> DXGIInfoManager::GetMessages() const
	{
		std::vector<std::wstring> messages;
		const auto end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for (auto i = next; i < end; i++)
		{
			HRESULT hr;
			SIZE_T messageLength;
			// get the size of message i in bytes
			WinCheck(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
			// allocate memory for message
			auto bytes = std::make_unique<byte[]>(messageLength);
			auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
			// get the message and push its description into the vector
			WinCheck(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
			// Not sure if pDescription is guaranteed to be a wide string if used with GetMessageW
			// TODO: Test if it works :)
			std::wstring message(reinterpret_cast<const wchar_t*>(pMessage->pDescription));
			messages.push_back(message);
		}
		return messages;
	}
}
