// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#ifndef UNICODE
#define UNICODE // For Windows.h
#endif

#ifndef _UNICODE
#define _UNICODE // For Microsoft CRT
#endif

// target Windows 10 or later
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN

#define NOGDICAPMASKS       // CC_*, LC_*, PC_*, CP_*, TC_*, RC_.
#define NOSYSMETRICS        // SM_*.
#define NOMENUS             // MF_*.
#define NOICONS             // IDI_*.
#define NOSYSCOMMANDS       // SC_*.
#define NORASTEROPS         // Binary and Tertiary raster ops.
#define OEMRESOURCE         // OEM Resource values.
#define NOATOM              // Atom Manager routines.
#define NOCLIPBOARD         // Clipboard routines.
#define NOCOLOR             // Screen colors.
#define NOCTLMGR            // Control and Dialog routines.
#define NODRAWTEXT          // DrawText() and DT_*.
#define NOGDI               // All GDI defines and routines.
#define NOKERNEL            // All KERNEL defines and routines.
#define NONLS               // All NLS defines and routines.
#define NOMB                // MB_ * and MessageBox().
#define NOMEMMGR            // GMEM_*, LMEM_*, GHND, LHND, associated routines.
#define NOMETAFILE          // typedef METAFILEPICT.
#define NOMINMAX            // Macros min(a,b) and max(a,b).
#define NOOPENFILE          // OpenFile(), OemToAnsi, AnsiToOem, and OF_*.
#define NOSCROLL            // SB_* and scrolling routines.
#define NOSERVICE           // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND             // Sound driver routines.
#define NOTEXTMETRIC        // typedef TEXTMETRIC and associated routines.
#define NOWH                // SetWindowsHook and WH_*
#define NOCOMM              // COMM driver routines
#define NOKANJI             // Kanji support stuff.
#define NOHELP              // Help engine interface.
#define NOPROFILER          // Profiler interface.
#define NODEFERWINDOWPOS    // DeferWindowPos routines.
#define NOMCX               // Modem Configuration Extensions.
#define NORPC               // Remote Procedure Call.
#define NOPROXYSTUB         // Proxy and Stub stuff.
#define NOIMAGE             // Image processing. 
#define NOTAPE              // Tape stuff.

#define STRICT              // Strict handle typing