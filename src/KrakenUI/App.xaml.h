// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include "App.xaml.g.h"

namespace winrt::KrakenUI::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(const Microsoft::UI::Xaml::LaunchActivatedEventArgs&);

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
    };
}
