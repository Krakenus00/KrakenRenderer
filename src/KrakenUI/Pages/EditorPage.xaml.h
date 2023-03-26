// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include "EditorPage.g.h"

namespace winrt::KrakenUI::implementation
{
    struct EditorPage : EditorPageT<EditorPage>
    {
        EditorPage();
    };
}

namespace winrt::KrakenUI::factory_implementation
{
    struct EditorPage : EditorPageT<EditorPage, implementation::EditorPage>
    {
    };
}
