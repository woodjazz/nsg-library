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
#if defined(IS_TARGET_OSX)
#include "OSXWindow.h"
#include "Engine.h"
#include "Tick.h"
#include "Keys.h"
#include "Log.h"
#include "Check.h"
#include "UTF8String.h"
#include "AppConfiguration.h"
#include "imgui.h"
#include "Maths.h"
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

+ (AppDelegate*)sharedInstance;
- (id)init;
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender;
- (bool)applicationHasTerminated;

@end

@interface Window : NSObject<NSWindowDelegate>
{
    NSG::OSXWindow* window_;
}

+ (Window*)sharedInstance;
- (id)init;
- (void)windowCreated:(NSWindow*)window osxWindow:(NSG::OSXWindow*)myWindow;
- (void)windowWillClose:(NSNotification*)notification;
- (BOOL)windowShouldClose:(NSWindow*)window;
- (void)windowDidResize:(NSNotification*)notification;
- (void)windowDidBecomeKey:(NSNotification*)notification;
- (void)windowDidResignKey:(NSNotification*)notification;

@end

@implementation AppDelegate

+ (AppDelegate*)sharedInstance
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

+ (Window*)sharedInstance
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
    return self;
}

- (void)windowCreated:(NSWindow*)window osxWindow:(NSG::OSXWindow*)myWindow
{
    CHECK_ASSERT(window);
    [window setDelegate:self];
    self->window_ = myWindow;
}

- (void)windowWillClose:(NSNotification*)notification
{
}

- (BOOL)windowShouldClose:(NSWindow*)window
{
    CHECK_ASSERT(window);
    [window setDelegate:nil];
    self->window_->Close();
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
      window_(nullptr),
      keyModifier_(0),
      flagStates_(0)
{
    const AppConfiguration& conf = Engine::GetPtr()->GetAppConfiguration();
    Initialize(conf.x_, conf.y_, conf.width_, conf.height_, flags);
    LOGI("Window %s created.", name_.c_str());
}

OSXWindow::OSXWindow(const std::string& name, int x, int y, int width, int height, WindowFlags flags)
    : Window(name),
      flags_(0),
      style_(0),
      window_(nullptr),
      keyModifier_(0),
      flagStates_(0)
{
    Initialize(x, y, width, height, flags);
    LOGI("Window %s created.", name_.c_str());
}

OSXWindow::~OSXWindow()
{
    Close();
}

void OSXWindow::Initialize(int x, int y, int width, int height, WindowFlags flags)
{
    static std::once_flag onceFlag_;
    std::call_once(onceFlag_, [&]()
    {
        id dg = [AppDelegate sharedInstance];
        [NSApp setDelegate:dg];
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
    [window_ setAcceptsMouseMovedEvents:YES];
    [window_ setBackgroundColor:[NSColor blackColor]];

    [[::Window sharedInstance] windowCreated:window_ osxWindow:this];
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

    if (flags & (int)WindowFlag::SHOWN)
        Show();
    else if (flags & (int)WindowFlag::HIDDEN)
        Hide();
}

void OSXWindow::Close()
{
    Window::Close();
}

void OSXWindow::CreateContext()
{
    static_assert(MAC_OS_X_VERSION_MAX_ALLOWED && MAC_OS_X_VERSION_MAX_ALLOWED >= 1070, "Cannot compile in this old OSX!!!");

    NSOpenGLPixelFormatAttribute profile = NSOpenGLProfileVersionLegacy;
    //NSOpenGLPixelFormatAttribute profile = NSOpenGLProfileVersion3_2Core;
    NSOpenGLPixelFormatAttribute pixelFormatAttributes[] =
    {
        NSOpenGLPFAOpenGLProfile, profile,
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
            [view_ release];
            view_ = nullptr;
            context_ = nullptr;
            Window::SetMainWindow(nullptr);
            //[NSApp terminate:self];

        }
    }
}

void OSXWindow::SwapWindowBuffers()
{
    [context_ makeCurrentContext];
    [context_ flushBuffer];
}

void OSXWindow::GetMousePos(int& outX, int& outY) const
{
    NSRect originalFrame = [window_ frame];
    NSPoint location = [window_ mouseLocationOutsideOfEventStream];
    NSRect adjustFrame = [window_ contentRectForFrameRect:originalFrame];

    int x = location.x;
    int y = (int)adjustFrame.size.height - (int)location.y;

    outX = Clamp(x, 0, (int)adjustFrame.size.width);
    outY = Clamp(y, 0, (int)adjustFrame.size.height);
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

static int MapKeyCode(int keyCode)
{
    switch (keyCode)
    {
    case 27:
        keyCode = NSG_KEY_ESC;
        break;
    case uint8_t(' '):
        keyCode = NSG_KEY_SPACE;
        break;
    case uint8_t('\t'):
        keyCode = NSG_KEY_TAB;
        break;
    case 127:
        keyCode = NSG_KEY_BACKSPACE;
        break;
    case uint8_t('\r'):
        keyCode = NSG_KEY_ENTER;
        break;
    case NSRightArrowFunctionKey:
        keyCode = NSG_KEY_RIGHT;
        break;
    case NSLeftArrowFunctionKey:
        keyCode = NSG_KEY_LEFT;
        break;
    case NSUpArrowFunctionKey:
        keyCode = NSG_KEY_UP;
        break;
    case NSDownArrowFunctionKey:
        keyCode = NSG_KEY_DOWN;
        break;
    case NSHomeFunctionKey:
        keyCode = NSG_KEY_HOME;
        break;
    case NSEndFunctionKey:
        keyCode = NSG_KEY_END;
        break;
    case NSPageUpFunctionKey:
        keyCode = NSG_KEY_PAGEUP;
        break;
    case NSPageDownFunctionKey:
        keyCode = NSG_KEY_PAGEDOWN;
        break;
    }
    return keyCode;
}

bool OSXWindow::DispatchEvent(NSEvent* event)
{
    if (event)
    {
        NSEventType eventType = [event type];
        auto window = static_cast<OSXWindow*>(NSG::Window::GetMainWindow());

        switch (eventType)
        {
        case NSMouseMoved:
        case NSLeftMouseDragged:
        case NSRightMouseDragged:
        case NSOtherMouseDragged:
        {
            int x, y;
            window->GetMousePos(x, y);
            window->OnMouseMove(x, y);
            break;
        }

        case NSLeftMouseDown:
        {
            int x, y;
            window->GetMousePos(x, y);
            auto button = ([event modifierFlags] & NSCommandKeyMask) ? NSG_BUTTON_MIDDLE : NSG_BUTTON_LEFT;
            window->OnMouseDown(button, x, y);
            break;
        }

        case NSLeftMouseUp:
        {
            int x, y;
            window->GetMousePos(x, y);
            auto button = ([event modifierFlags] & NSCommandKeyMask) ? NSG_BUTTON_MIDDLE : NSG_BUTTON_LEFT;
            window->OnMouseUp(button, x, y);
            break;
        }

        case NSRightMouseDown:
        {
            int x, y;
            window->GetMousePos(x, y);
            window->OnMouseDown(NSG_BUTTON_RIGHT, x, y);
            break;
        }

        case NSRightMouseUp:
        {
            int x, y;
            window->GetMousePos(x, y);
            window->OnMouseUp(NSG_BUTTON_RIGHT, x, y);
            break;
        }

        case NSOtherMouseDown:
        {
            int x, y;
            window->GetMousePos(x, y);
            window->OnMouseDown(NSG_BUTTON_MIDDLE, x, y);
            break;
        }

        case NSOtherMouseUp:
        {
            int x, y;
            window->GetMousePos(x, y);
            window->OnMouseUp(NSG_BUTTON_MIDDLE, x, y);
            break;
        }

        case NSScrollWheel:
        {
            window->OnMouseWheel([event deltaX], [event deltaY]);
            break;
        }

        case NSFlagsChanged:
        {
            int flags = [event modifierFlags];

            if(flags & NSShiftKeyMask)
                window->OnKey(NSG_KEY_LSHIFT, NSG_KEY_PRESS, 0);
            else if(flagStates_ & NSShiftKeyMask)
                window->OnKey(NSG_KEY_LSHIFT, NSG_KEY_RELEASE, 0);

            if(flags & NSAlternateKeyMask)
                window->OnKey(NSG_KEY_LALT, NSG_KEY_PRESS, 0);
            else if(flagStates_ & NSAlternateKeyMask)
                window->OnKey(NSG_KEY_LALT, NSG_KEY_RELEASE, 0);

            flagStates_ = flags;
            break;
        }

        case NSKeyDown:
        {
            NSString* key = [event charactersIgnoringModifiers];
            if ([key length] != 0)
            {
                std::string text = [key UTF8String];
                window->OnText(text);
                UTF8String utf8(text.c_str());
                unsigned unicode = utf8.AtUTF8(0);
                if (unicode)
                    window->OnChar(unicode);
                int action = NSG_KEY_PRESS;
                int keyCode = MapKeyCode([key characterAtIndex:0]);
                int flags = [event modifierFlags];
                window->keyModifier_ |= flags & NSShiftKeyMask ? NSG_KEY_MOD_SHIFT : 0;
                window->keyModifier_ |= flags & NSControlKeyMask ? NSG_KEY_MOD_CONTROL : 0;
                window->keyModifier_ |= flags & NSAlternateKeyMask ? NSG_KEY_MOD_ALT : 0;
                window->OnKey(keyCode, action, window->keyModifier_);
                break;
            }
            break;
        }

        case NSKeyUp:
        {
            NSString* key = [event charactersIgnoringModifiers];
            if ([key length] != 0)
            {
                int action = NSG_KEY_RELEASE;
                int keyCode = MapKeyCode([key characterAtIndex:0]);
                int flags = [event modifierFlags];
                window->keyModifier_ &= flags & NSShiftKeyMask ? ~NSG_KEY_MOD_SHIFT : 0;
                window->keyModifier_ &= flags & NSControlKeyMask ? ~NSG_KEY_MOD_CONTROL : 0;
                window->keyModifier_ &= flags & NSAlternateKeyMask ? ~NSG_KEY_MOD_ALT : 0;
                window->OnKey(keyCode, action, window->keyModifier_);
                break;
            }
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
    while (OSXWindow::DispatchEvent(PeekEvent()))
    {
    }

}

void OSXWindow::Show()
{
    [window_ makeKeyAndOrderFront:window_];
    [NSApp activateIgnoringOtherApps:YES];

}

void OSXWindow::Hide()
{
    [window_ orderOut:window_];
}

void OSXWindow::Raise()
{
}

void OSXWindow::SetupImgui()
{
    Window::SetupImgui();
    ImGuiIO& io = ImGui::GetIO();
    io.KeyRepeatDelay = 1;
    io.OSXBehaviors = true;
}

void OSXWindow::BeginImguiRender()
{
    //ImGuiIO& io = ImGui::GetIO();
    //io.ImeWindowHandle = (void*)hwnd_;
    // Hide OS mouse cursor if ImGui is drawing it
    //ShowCursor(io.MouseDrawCursor ? FALSE : TRUE);
}
}
#endif
