#pragma once

#include "EditorPage.g.h"

namespace winrt::KrakenUI::implementation
{
    struct EditorPage : EditorPageT<EditorPage>
    {
        EditorPage();

        void myButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::KrakenUI::factory_implementation
{
    struct EditorPage : EditorPageT<EditorPage, implementation::EditorPage>
    {
    };
}
