// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#ifdef DIRECTX11CORE_EXPORTS
#define DIRECTX11CORE_API __declspec(dllexport)
#else
#define DIRECTX11CORE_API __declspec(dllimport)
#endif
