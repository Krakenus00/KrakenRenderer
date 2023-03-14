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
        titleBarLoadedRevoker = TitleBar().Loaded(auto_revoke, {this, &MainWindow::OnTitleBarLoaded});
        titleBarSizeChangedRevoker = TitleBar().SizeChanged(auto_revoke, { this, &MainWindow::OnTitleBarSizeChanged });
        titleBar.PreferredHeightOption(TitleBarHeightOption::Tall);
        // Buggy boyo, let it sit here for a while
        //SetTitleBar(TitleBar());

        SetBackdrop();
        closedRevoker = this->Closed(winrt::auto_revoke, { this, &MainWindow::OnClosed });
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
            configuration.Theme(ConvertToSystemBackdropTheme(rootElement.ActualTheme()));
            auto onThemeChanged = [this](auto&&, auto&&)
            {
                configuration.Theme(ConvertToSystemBackdropTheme(rootElement.ActualTheme()));
            };
            themeChangedRevoker = rootElement.ActualThemeChanged(auto_revoke, onThemeChanged);
        }
    }

    void MainWindow::OnTitleBarLoaded(IInspectable const&, RoutedEventArgs const&)
    {
        if (AppWindowTitleBar::IsCustomizationSupported())
            SetDragRegionForCustomTitleBar(appWindow);
    }

    void MainWindow::OnTitleBarSizeChanged(IInspectable const&, SizeChangedEventArgs const&)
    {
        // Useless if ExtendsContentIntoTitleBar is false.
        if (AppWindowTitleBar::IsCustomizationSupported() && titleBar.ExtendsContentIntoTitleBar())
            SetDragRegionForCustomTitleBar(appWindow);
    }

    void MainWindow::SetDragRegionForCustomTitleBar(AppWindow apw)
    {
        if (titleBar.ExtendsContentIntoTitleBar())
            return;

        double scaleAdjustment = GetScaleAdjustment();

        RightPaddingColumn().Width(GridLength(appWindow.TitleBar().RightInset() / scaleAdjustment));
        LeftPaddingColumn().Width(GridLength(appWindow.TitleBar().LeftInset() / scaleAdjustment));

        RectInt32 dragRectL
        {
            .X = static_cast<int32_t>(LeftPaddingColumn().ActualWidth() * scaleAdjustment),
            .Y = 0,
            // Count elements width
            .Width = static_cast<int32_t>((IconColumn().ActualWidth() +
                      TitleColumn().ActualWidth() +
                      LeftDragColumn().ActualWidth()) * scaleAdjustment),
            .Height = static_cast<int32_t>(TitleBar().ActualHeight() * scaleAdjustment),
        };

        /* Non-draggable space */

        RectInt32 dragRectR
        {
            .X = dragRectL.X + dragRectL.Width,
            .Y = 0,
            .Width = static_cast<int32_t>(RightDragColumn().ActualWidth() * scaleAdjustment),
            .Height = dragRectL.Height,
        };

        appWindow.TitleBar().SetDragRectangles({ dragRectL, dragRectR });
    }

    DispatcherQueueController MainWindow::CreateSystemDispatcherQueueController()
    {
        namespace abi = ABI::Windows::System;

        DispatcherQueueOptions options
        {
            .dwSize = sizeof(DispatcherQueueOptions),
            .threadType = DQTYPE_THREAD_CURRENT,
            .apartmentType = DQTAT_COM_NONE
        };

        abi::IDispatcherQueueController* ptr{ nullptr };
        check_hresult(CreateDispatcherQueueController(options, &ptr));
        return { ptr, take_ownership_from_abi };
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
}
