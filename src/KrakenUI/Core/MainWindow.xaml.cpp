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

        SetBackdrop();
        closedRevoker = this->Closed(winrt::auto_revoke, { this, &MainWindow::OnClosed });

        titleBar = appWindow.TitleBar();
        titleBar.ExtendsContentIntoTitleBar(true);
        IReference<Windows::UI::Color> btnColor(Colors::Transparent());
        // Make background transparent so it matches my custom title bar
        titleBar.BackgroundColor(btnColor);
        titleBar.ButtonBackgroundColor(btnColor);
        titleBar.InactiveBackgroundColor(btnColor);
        titleBar.ButtonInactiveBackgroundColor(btnColor);
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
        auto windowNative{ this->try_as<::IWindowNative>() };
        winrt::check_bool(windowNative);
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

    void MainWindow::OnTitleBarLoaded(IInspectable const&, RoutedEventArgs const&)
    {
        titleBarSizeChangedRevoker = CustomDragRegion().SizeChanged(auto_revoke, { this, &MainWindow::OnTitleBarSizeChanged });
    }

    void MainWindow::OnTitleBarSizeChanged(IInspectable const&, SizeChangedEventArgs const& args)
    {
        Size floatSize = args.NewSize();
        SizeInt32 newSize
        { 
            .Width  = static_cast<int32_t>(floatSize.Width),
            .Height = static_cast<int32_t>(floatSize.Height)
        };

        titleBar.SetDragRectangles({ { 
            .X = appWindow.Size().Width - newSize.Width,
            .Y = 0,
            .Width  = newSize.Width,
            .Height = newSize.Height
        } });
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

    IReference<Windows::UI::Color> MainWindow::GetColor(winrt::hstring const& brushName)
    {
        return Application::Current().Resources().Lookup(box_value(brushName)).as<SolidColorBrush>().Color();
    }

    SystemBackdropTheme MainWindow::ConvertToSystemBackdropTheme(ElementTheme const& theme)
    {
        switch (theme)
        {
            case ElementTheme::Dark:    return SystemBackdropTheme::Dark;
            case ElementTheme::Light:   return SystemBackdropTheme::Light;
            default:                    return SystemBackdropTheme::Default;
        }
    }

    void MainWindow::OnClosed(IInspectable const&, WindowEventArgs const&)
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

    void MainWindow::OnActivated(IInspectable const&, WindowActivatedEventArgs const& args)
    {
        configuration.IsInputActive(WindowActivationState::Deactivated != args.WindowActivationState());
    }

    void MainWindow::OnThemeChanged(Microsoft::UI::Xaml::FrameworkElement const& sender, Windows::Foundation::IInspectable const&)
    {
        configuration.Theme(ConvertToSystemBackdropTheme(sender.ActualTheme()));
        // Update caption buttons
        titleBar.ButtonForegroundColor(GetColor(L"TextFillColorPrimaryBrush"));
        titleBar.ButtonHoverBackgroundColor(GetColor(L"SubtleFillColorSecondaryBrush"));
        titleBar.ButtonPressedBackgroundColor(GetColor(L"SubtleFillColorTertiaryBrush"));
    }
}
