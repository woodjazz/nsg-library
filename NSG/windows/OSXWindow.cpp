/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#if defined(IS_TARGET_OSX) && !defined(SDL)
#include "OSXWindow.h"
#include "Engine.h"
#include "Tick.h"
#include "Keys.h"
#include "Log.h"
#include "Check.h"
#include "UTF8String.h"
#include "AppConfiguration.h"
#include "imgui.h"
#include <memory>
#include <string>
#include <locale>
#include <map>
#ifndef __GNUC__
#include <codecvt>
#endif

@interface AppDelegate : NSObject<NSApplicationDelegate>
{
    bool terminated;
}

+ (AppDelegate*)sharedDelegate;
- (id)init;
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender;
- (bool)applicationHasTerminated;

@end

@interface Window : NSObject<NSWindowDelegate>
{
    uint32_t windowCount;
    NSG::OSXWindow* window_;
}

+ (Window*)sharedDelegate;
- (id)init;
- (void)windowCreated:(NSWindow*) window:(NSG::OSXWindow*) osxWindow;
- (void)windowWillClose:(NSNotification*)notification;
- (BOOL)windowShouldClose:(NSWindow*)window;
- (void)windowDidResize:(NSNotification*)notification;
- (void)windowDidBecomeKey:(NSNotification*)notification;
- (void)windowDidResignKey:(NSNotification*)notification;

@end

@implementation AppDelegate

+ (AppDelegate*)sharedDelegate
{
    static id delegate = [AppDelegate new];
    return delegate;
}

- (id)init
{
    self = [super init];

    if (nil == self)
    {
        return nil;
    }

    self->terminated = false;
    return self;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender
{
    self->terminated = true;
    return NSTerminateCancel;
}

- (bool)applicationHasTerminated
{
    return self->terminated;
}

@end

@implementation Window

+ (Window*)sharedDelegate
{
    static id windowDelegate = [Window new];
    return windowDelegate;
}

- (id)init
{
    self = [super init];
    if (nil == self)
    {
        return nil;
    }

    self->windowCount = 0;
    return self;
}

- (void)windowCreated:(NSWindow*) window:(NSG::OSXWindow*) osxWindow
{
    assert(window);

    [window setDelegate:self];

    assert(self->windowCount < ~0u);
    self->windowCount += 1;
    self->window_ = osxWindow;
}

- (void)windowWillClose:(NSNotification*)notification
{
}

- (BOOL)windowShouldClose:(NSWindow*)window
{
    assert(window);

    [window setDelegate:nil];

    assert(self->windowCount);
    self->windowCount -= 1;

    if (self->windowCount == 0)
    {
        [NSApp terminate:self];
        return false;
    }

    return true;
}

- (void)windowDidResize:(NSNotification*)notification
{
    auto window = window_->GetNSWindow();
    NSRect originalFrame = [window frame];
    NSRect rect = [window contentRectForFrameRect:originalFrame];
    uint32_t width  = uint32_t(rect.size.width);
    uint32_t height = uint32_t(rect.size.height);
    window_->SetSize(width, height);
    // Make sure mouse button state is 'up' after resize.
    //m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, MouseButton::Left,  false);
    //m_eventQueue.postMouseEvent(s_defaultWindow, m_mx, m_my, m_scroll, MouseButton::Right, false);

}

- (void)windowDidBecomeKey:(NSNotification*)notification
{
}

- (void)windowDidResignKey:(NSNotification*)notification
{
}

@end


namespace NSG
{
    NSOpenGLView* OSXWindow::view_ = nullptr;
    NSOpenGLContext* OSXWindow::context_ = nullptr;

    OSXWindow::OSXWindow(const std::string& name, WindowFlags flags)
        : Window(name),
          flags_(0),
          style_(0),
          window_(nullptr)
    {
        const AppConfiguration& conf = Engine::GetPtr()->GetAppConfiguration();
        Initialize(conf.x_, conf.y_, conf.width_, conf.height_, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    OSXWindow::OSXWindow(const std::string& name, int x, int y, int width, int height, WindowFlags flags)
        : Window(name),
          flags_(0),
          style_(0),
          window_(nullptr)
    {
        Initialize(x, y, width, height, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    OSXWindow::~OSXWindow()
    {
        Close();
    }

    static id dg = 0;
    static id quitMenuItem = 0;
    static id appMenu = 0;
    static id appMenuItem = 0;
    static id menubar = 0;
    void OSXWindow::Initialize(int x, int y, int width, int height, WindowFlags flags)
    {
        static std::once_flag onceFlag_;
        std::call_once(onceFlag_, [&]()
        {
            [NSApplication sharedApplication];

            dg = [AppDelegate sharedDelegate];
            [NSApp setDelegate:dg];
            [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
            [NSApp activateIgnoringOtherApps:YES];
            [NSApp finishLaunching];

            [[NSNotificationCenter defaultCenter]
             postNotificationName:NSApplicationWillFinishLaunchingNotification
             object:NSApp];

            [[NSNotificationCenter defaultCenter]
             postNotificationName:NSApplicationDidFinishLaunchingNotification
             object:NSApp];

            quitMenuItem = [NSMenuItem new];
            [quitMenuItem
             initWithTitle:@"Quit"
             action:@selector(terminate:)
             keyEquivalent:@"q"];

            appMenu = [NSMenu new];
            [appMenu addItem:quitMenuItem];

            appMenuItem = [NSMenuItem new];
            [appMenuItem setSubmenu:appMenu];

            menubar = [[NSMenu new] autorelease];
            [menubar addItem:appMenuItem];
            [NSApp setMainMenu:menubar];
        });

        style_ = 0
                 | NSTitledWindowMask
                 | NSClosableWindowMask
                 | NSMiniaturizableWindowMask
                 | NSResizableWindowMask
                 ;

        NSRect screenRect = [[NSScreen mainScreen] frame];
        const float centerX = (screenRect.size.width  - (float)width ) * 0.5f;
        const float centerY = (screenRect.size.height - (float)height) * 0.5f;

        NSRect rect = NSMakeRect(centerX, centerY, width, height);
        window_ = [[NSWindow alloc]
                   initWithContentRect:rect
                   styleMask:style_
                   backing:NSBackingStoreBuffered defer:NO
                  ];
        NSString* appName = [[NSProcessInfo processInfo] processName];
        [window_ setTitle:appName];
        [window_ makeKeyAndOrderFront:window_];
        [window_ setAcceptsMouseMovedEvents:YES];
        [window_ setBackgroundColor:[NSColor blackColor]];
        [[::Window sharedDelegate] windowCreated:window_:this];
        windowFrame_ = [window_ frame];

        if (Window::mainWindow_)
        {
            isMainWindow_ = false;
            // Do not create a new context. Instead, share the main window's context.
            SetContext();
        }
        else
        {
            CreateContext();
            Window::SetMainWindow(this);
        }

        SetSize(width, height);

    }

    void OSXWindow::Close()
    {
        Window::Close();
    }

    void OSXWindow::CreateContext()
    {
        struct AutoreleasePoolHolder
        {
            AutoreleasePoolHolder() : pool_([[NSAutoreleasePool alloc] init]) {}
            ~AutoreleasePoolHolder() { [pool_ release]; }
        private:
            NSAutoreleasePool* const pool_;
        } pool;

        #if defined(MAC_OS_X_VERSION_MAX_ALLOWED) && (MAC_OS_X_VERSION_MAX_ALLOWED >= 1070)
        NSOpenGLPixelFormatAttribute profile = NSOpenGLProfileVersion3_2Core;
        //            NSOpenGLProfileVersionLegacy
        #endif

        NSOpenGLPixelFormatAttribute pixelFormatAttributes[] =
        {
            #if defined(MAC_OS_X_VERSION_MAX_ALLOWED) && (MAC_OS_X_VERSION_MAX_ALLOWED >= 1070)
            NSOpenGLPFAOpenGLProfile, profile,
            #endif // defined(MAC_OS_X_VERSION_MAX_ALLOWED) && (MAC_OS_X_VERSION_MAX_ALLOWED >= 1070)
            NSOpenGLPFAColorSize,     24,
            NSOpenGLPFAAlphaSize,     8,
            NSOpenGLPFADepthSize,     24,
            NSOpenGLPFAStencilSize,   8,
            NSOpenGLPFADoubleBuffer,  true,
            NSOpenGLPFAAccelerated,   true,
            NSOpenGLPFANoRecovery,    true,
            0,                        0,
        };

        NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:pixelFormatAttributes];
        CHECK_CONDITION(nullptr != pixelFormat && "Failed to initialize pixel format.");

        NSRect glViewRect = [[window_ contentView] bounds];
        OSXWindow::view_ = [[NSOpenGLView alloc] initWithFrame:glViewRect pixelFormat:pixelFormat];

        [pixelFormat release];
        [window_ setContentView:OSXWindow::view_];

        OSXWindow::context_ = [OSXWindow::view_ openGLContext];
        CHECK_CONDITION(nullptr != OSXWindow::context_ && "Failed to initialize GL context.");

        [OSXWindow::context_ makeCurrentContext];
        GLint interval = 0;
        [OSXWindow::context_ setValues:&interval forParameter:NSOpenGLCPSwapInterval];
    }

    void OSXWindow::SetContext()
    {
        [window_ setContentView:OSXWindow::view_];
        [OSXWindow::context_ makeCurrentContext];
    }

    void OSXWindow::Destroy()
    {
        if (!isClosed_)
        {
            isClosed_ = true;
            Window::NotifyOneWindow2Remove();
            if (isMainWindow_)
            {
                Window::SetMainWindow(nullptr);
            }
        }
    }

    void OSXWindow::SwapWindowBuffers()
    {
        [context_ makeCurrentContext];
        [context_ flushBuffer];
    }

    static NSEvent* PeekEvent()
    {
        return [NSApp
                nextEventMatchingMask:NSAnyEventMask
                untilDate:[NSDate distantPast] // do not wait for event
                inMode:NSDefaultRunLoopMode
                dequeue:YES
               ];
    }


    static bool DispatchEvent(NSEvent* event)
    {
        if (event)
        {
            NSEventType eventType = [event type];

            switch (eventType)
            {
                case NSMouseMoved:
                case NSLeftMouseDragged:
                case NSRightMouseDragged:
                case NSOtherMouseDragged:
                    {
                        break;
                    }

                case NSLeftMouseDown:
                    {
                        break;
                    }

                case NSLeftMouseUp:
                    {
                        break;
                    }

                case NSRightMouseDown:
                    {
                        break;
                    }

                case NSRightMouseUp:
                    {
                        break;
                    }

                case NSOtherMouseDown:
                    {
                        break;
                    }

                case NSOtherMouseUp:
                    {
                        break;
                    }

                case NSScrollWheel:
                    {
                        break;
                    }

                case NSKeyDown:
                    {
                        break;
                    }

                case NSKeyUp:
                    {
                        break;
                    }
            }

            [NSApp sendEvent:event];
            [NSApp updateWindows];

            return true;
        }

        return false;
    }


    void OSXWindow::HandleEvents()
    {
        while (DispatchEvent(PeekEvent()))
        {
        }

    }

    void OSXWindow::Show()
    {
    }

    void OSXWindow::Hide()
    {
    }

    void OSXWindow::Raise()
    {
    }

    static const char* ImGuiGetClipboardText()
    {
        return "";
    }

    static void ImGuiSetClipboardText(const char* text)
    {
    }

    void OSXWindow::SetupImgui()
    {
        Window::SetupImgui();
        ImGuiIO& io = ImGui::GetIO();
        io.SetClipboardTextFn = ImGuiSetClipboardText;
        io.GetClipboardTextFn = ImGuiGetClipboardText;
    }

    void OSXWindow::BeginImguiRender()
    {
        ImGuiIO& io = ImGui::GetIO();
        //io.ImeWindowHandle = (void*)hwnd_;
        // Hide OS mouse cursor if ImGui is drawing it
        //ShowCursor(io.MouseDrawCursor ? FALSE : TRUE);
    }
}
#endif
