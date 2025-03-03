// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#include "pch.h"
#include "EditorPage.xaml.h"
#if __has_include("EditorPage.g.cpp")
#include "EditorPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::KrakenUI::implementation
{
    EditorPage::EditorPage()
    {
        InitializeComponent();
    }
}
