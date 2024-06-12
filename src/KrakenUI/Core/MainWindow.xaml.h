// Copyright 2023 Mykhailo Mushynskyi. All rights reserved.
#pragma once

#include "MainWindow.g.h"

namespace winrt::KrakenUI::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
    public:
        Microsoft::UI::Windowing::AppWindow GetNativeWindow() const;

    private:
        void GetAppWindow();
        double GetScaleAdjustment();
        void SetupSystemBackdropConfiguration();
        void SetBackdrop(Microsoft::UI::Composition::SystemBackdrops::MicaKind type = Microsoft::UI::Composition::SystemBackdrops::MicaKind::Base);
        Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme ConvertToSystemBackdropTheme(const Microsoft::UI::Xaml::ElementTheme& theme);
        Windows::System::DispatcherQueueController CreateSystemDispatcherQueueController();
        Windows::Foundation::IReference<Windows::UI::Color> GetColor(const winrt::hstring& brushName);

    public: // Events
        void OnClosed(const Windows::Foundation::IInspectable&, const Microsoft::UI::Xaml::WindowEventArgs&);
        void OnActivated(const Windows::Foundation::IInspectable&, const Microsoft::UI::Xaml::WindowActivatedEventArgs& args);
        void OnThemeChanged(const Microsoft::UI::Xaml::FrameworkElement& sender, const Windows::Foundation::IInspectable&);
        void OnTitleBarLoaded(const Windows::Foundation::IInspectable&, const Microsoft::UI::Xaml::RoutedEventArgs&);
        void OnTitleBarSizeChanged(const Windows::Foundation::IInspectable&, const Microsoft::UI::Xaml::SizeChangedEventArgs& args);
        void OnTabCloseRequested(const Microsoft::UI::Xaml::Controls::TabView& sender, const Microsoft::UI::Xaml::Controls::TabViewTabCloseRequestedEventArgs& args);

    private:
        HWND handler{ 0 };
        Microsoft::UI::WindowId windowId{ 0 };
        Microsoft::UI::Windowing::AppWindow appWindow{ nullptr };
        Microsoft::UI::Windowing::AppWindowTitleBar titleBar{ nullptr };
        Microsoft::UI::Xaml::FrameworkElement rootElement{ nullptr };
        // Backdrop
        Microsoft::UI::Composition::SystemBackdrops::SystemBackdropConfiguration configuration{ nullptr };
        Microsoft::UI::Composition::SystemBackdrops::MicaController backdropController{ nullptr };
        Windows::System::DispatcherQueueController dispatcherQueueController{ nullptr };
        // Event revokers
        Microsoft::UI::Xaml::Window::Activated_revoker activatedRevoker;
        Microsoft::UI::Xaml::Window::Closed_revoker closedRevoker;
        Microsoft::UI::Xaml::FrameworkElement::ActualThemeChanged_revoker themeChangedRevoker;
        Microsoft::UI::Xaml::Controls::Grid::Loaded_revoker titleBarLoadedRevoker;
        Microsoft::UI::Xaml::Controls::Grid::SizeChanged_revoker titleBarSizeChangedRevoker;
    };
}

namespace winrt::KrakenUI::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
