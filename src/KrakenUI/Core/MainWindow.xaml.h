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
        Microsoft::UI::Composition::SystemBackdrops::SystemBackdropTheme ConvertToSystemBackdropTheme(Microsoft::UI::Xaml::ElementTheme const& theme);
        Windows::System::DispatcherQueueController CreateSystemDispatcherQueueController();
        Windows::Foundation::IReference<Windows::UI::Color> GetColor(winrt::hstring const& brushName);

    public: // Events
        void OnClosed(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::WindowEventArgs const&);
        void OnActivated(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::WindowActivatedEventArgs const& args);
        void OnThemeChanged(Microsoft::UI::Xaml::FrameworkElement const& sender, Windows::Foundation::IInspectable const&);
        void OnTitleBarLoaded(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void OnTitleBarSizeChanged(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::SizeChangedEventArgs const& args);
        void OnTabCloseRequested(Microsoft::UI::Xaml::Controls::TabView const& sender, Microsoft::UI::Xaml::Controls::TabViewTabCloseRequestedEventArgs const& args);

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
