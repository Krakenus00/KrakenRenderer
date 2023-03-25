#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <DispatcherQueue.h>
#include <microsoft.ui.xaml.window.h>
#include <shellscalingapi.h>

using namespace winrt;
using namespace Microsoft::UI;
using namespace Microsoft::UI::Composition;
using namespace Microsoft::UI::Composition::SystemBackdrops;
using namespace Microsoft::UI::Windowing;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;
using namespace Windows::Foundation;
using namespace Windows::Graphics;
using namespace Windows::System;

#pragma comment(lib, "Shcore.lib")
namespace winrt::KrakenUI::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        GetAppWindow();

        if (!AppWindowTitleBar::IsCustomizationSupported())
        {
            // Why? Because I don't fucking care
            throw std::runtime_error("Unsupported OS version.");
        }

        titleBar = appWindow.TitleBar();
        titleBar.ExtendsContentIntoTitleBar(true);
        IReference<Windows::UI::Color> btnColor(Colors::Transparent());
        // Make background transparent so it matches my custom title bar
        titleBar.BackgroundColor(btnColor);
        titleBar.ButtonBackgroundColor(btnColor);
        titleBar.InactiveBackgroundColor(btnColor);
        titleBar.ButtonInactiveBackgroundColor(btnColor);

        SetBackdrop(MicaKind::BaseAlt);
        closedRevoker = this->Closed(auto_revoke, { this, &MainWindow::OnClosed });
    }

    AppWindow MainWindow::GetNativeWindow() const
    {
        return appWindow;
    }

    void MainWindow::SetBackdrop(MicaKind type)
    {
        if (MicaController::IsSupported())
        {
            // Check if there is a DispatcherQueue on my thread
            // NOTE: You can't create multiple DispatcherQueues on one thread
            if (!DispatcherQueue::GetForCurrentThread() && !dispatcherQueueController)
            {
                dispatcherQueueController = CreateSystemDispatcherQueueController();
            }

            // Setup the SystemBackdropConfiguration object.
            SetupSystemBackdropConfiguration();

            // Setup Mica on the current Window.
            backdropController = MicaController();
            backdropController.Kind(type);
            backdropController.SetSystemBackdropConfiguration(configuration);
            backdropController.AddSystemBackdropTarget(this->try_as<ICompositionSupportsSystemBackdrop>());
        }
        else
        {
            // Just fuck you
            throw std::runtime_error("Unsupported OS version.");
        }
    }

    void MainWindow::GetAppWindow()
    {
        // There is no way to do it in WinUI, need to play with handlers.
        auto windowNative{ this->try_as<IWindowNative>() };
        check_bool(windowNative);
        windowNative->get_WindowHandle(&handler);
        windowId = GetWindowIdFromWindow(handler);
        appWindow = AppWindow::GetFromWindowId(windowId);
    }

    double MainWindow::GetScaleAdjustment()
    {
        DisplayArea displayArea = DisplayArea::GetFromWindowId(windowId, DisplayAreaFallback::Primary);
        HMONITOR hMonitor = GetMonitorFromDisplayId(displayArea.DisplayId());

        // Get DPI
        UINT dpiX, dpiY;
        check_hresult(GetDpiForMonitor(hMonitor, MDT_DEFAULT, &dpiX, &dpiY));
        // Magic numbers from Microsoft. Have no idea what is going on.
        UINT scaleFactorPercent = (dpiX * 100 + 48) / 96;
        return scaleFactorPercent / 100.;
    }

    void MainWindow::SetupSystemBackdropConfiguration()
    {
        configuration = SystemBackdropConfiguration();

        // Activation state
        activatedRevoker = this->Activated(auto_revoke, { this, &MainWindow::OnActivated });

        // Initial state
        configuration.IsInputActive(true);

        // Application theme
        rootElement = this->Content().try_as<FrameworkElement>();
        if (rootElement)
        {
            // Initial state
            OnThemeChanged(rootElement, { nullptr });

            themeChangedRevoker = rootElement.ActualThemeChanged(auto_revoke, { this, &MainWindow::OnThemeChanged });
        }
    }

    void MainWindow::OnTitleBarLoaded(const IInspectable&, const RoutedEventArgs&)
    {
        titleBarSizeChangedRevoker = CustomDragRegion().SizeChanged(auto_revoke, { this, &MainWindow::OnTitleBarSizeChanged });
        // Doesn't do shit in XAML
        TitleBar().CloseButtonOverlayMode(TabViewCloseButtonOverlayMode::OnPointerOver);
    }

    void MainWindow::OnTitleBarSizeChanged(const IInspectable&, const SizeChangedEventArgs& args)
    {
        Size newSize = args.NewSize();
        auto scale = GetScaleAdjustment();

        // Completely not partable but whatever
        // Maybe these properties will be available somewhere in the future
        int tabViewTopEmptySpace = 11;
        int invisibleButtonSpace = 16;

        RectInt32 tabViewFooter
        {
            .X = static_cast<int>((appWindow.Size().Width - newSize.Width - invisibleButtonSpace) * scale),
            .Y = 0,
            .Width  = static_cast<int>(newSize.Width * scale),
            .Height = static_cast<int>(newSize.Height * scale)
        };

        RectInt32 tabViewTopSpace
        {
            .X = 0,
            .Y = 0,
            .Width  = static_cast<int>((appWindow.Size().Width - tabViewFooter.Width) * scale),
            .Height = static_cast<int>(tabViewTopEmptySpace * scale),
        };

        titleBar.SetDragRectangles({ tabViewTopSpace, tabViewFooter });
    }

    void MainWindow::OnTabCloseRequested(const TabView& sender, const TabViewTabCloseRequestedEventArgs& args)
    {
        unsigned int index;
        auto tabs = sender.TabItems();
        tabs.IndexOf(args.Tab(), index);
        tabs.RemoveAt(index);
    }

    DispatcherQueueController MainWindow::CreateSystemDispatcherQueueController()
    {
        namespace abi = ABI::Windows::System;

        DispatcherQueueOptions options
        {
            .dwSize        = sizeof(DispatcherQueueOptions),
            .threadType    = DQTYPE_THREAD_CURRENT,
            .apartmentType = DQTAT_COM_NONE
        };

        abi::IDispatcherQueueController* ptr{ nullptr };
        check_hresult(CreateDispatcherQueueController(options, &ptr));
        return { ptr, take_ownership_from_abi };
    }

    IReference<Windows::UI::Color> MainWindow::GetColor(const winrt::hstring& brushName)
    {
        return Application::Current().Resources().Lookup(box_value(brushName)).as<SolidColorBrush>().Color();
    }

    SystemBackdropTheme MainWindow::ConvertToSystemBackdropTheme(const ElementTheme& theme)
    {
        switch (theme)
        {
            case ElementTheme::Dark:    return SystemBackdropTheme::Dark;
            case ElementTheme::Light:   return SystemBackdropTheme::Light;
            default:                    return SystemBackdropTheme::Default;
        }
    }

    void MainWindow::OnClosed(const IInspectable&, const WindowEventArgs&)
    {
        if (backdropController)
        {
            backdropController.Close();
            backdropController = nullptr;
        }
        activatedRevoker.revoke();
        if (dispatcherQueueController)
        {
            dispatcherQueueController.ShutdownQueueAsync();
            dispatcherQueueController = nullptr;
        }
        configuration = nullptr;
    }

    void MainWindow::OnActivated(const IInspectable&, const WindowActivatedEventArgs& args)
    {
        configuration.IsInputActive(WindowActivationState::Deactivated != args.WindowActivationState());
    }

    void MainWindow::OnThemeChanged(const Microsoft::UI::Xaml::FrameworkElement& sender, const Windows::Foundation::IInspectable&)
    {
        configuration.Theme(ConvertToSystemBackdropTheme(sender.ActualTheme()));
        // Update caption buttons
        titleBar.ButtonForegroundColor(GetColor(L"TextFillColorPrimaryBrush"));
        titleBar.ButtonHoverBackgroundColor(GetColor(L"SubtleFillColorSecondaryBrush"));
        titleBar.ButtonPressedBackgroundColor(GetColor(L"SubtleFillColorTertiaryBrush"));
    }
}
