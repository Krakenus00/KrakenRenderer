// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

constexpr bool IS_DEBUG =
#ifdef NDEBUG
false;
#else
true;
#endif

#define APPEND_LITERAL_MACRO(x) L##x
#define WIDEC_PATH_UNWRAP(x) APPEND_LITERAL_MACRO(x)
// Path to a source file as wchar_t.
#define __WFILE__ WIDEC_PATH_UNWRAP(__FILE__)
