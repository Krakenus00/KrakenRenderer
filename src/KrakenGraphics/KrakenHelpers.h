#pragma once

constexpr bool IS_DEBUG =
#ifdef NDEBUG
false;
#else
true;
#endif

#define APPEND_WIDEC_LITERAL(x) L##x
#define WIDEC_PATH_UNWRAP(x) APPEND_WIDEC_LITERAL(x)
// Path to a source file as wchar_t.
#define __WFILE__ WIDEC_PATH_UNWRAP(__FILE__)
