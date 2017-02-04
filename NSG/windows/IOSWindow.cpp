/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#if defined(IS_TARGET_IOS)
#include "IOSWindow.h"
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
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/CAEAGLLayer.h>

@interface ViewController : UIViewController

@end

@implementation ViewController

- (BOOL)prefersStatusBarHidden
{
    return YES;
}

@end


@interface View : UIView
{
    CADisplayLink* displayLink_;
    EAGLContext* context_;
    GLuint fbo_;
    GLuint colorRbo_;
    GLuint depthStencilRbo_;
}

- (void)createFrameBuffers;
- (void)destroyFrameBuffers;

@end

@implementation View

+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)rect
{
    displayLink_ = nullptr;
    context_ = nullptr;
    fbo_ = 0;
    colorRbo_ = 0;
    depthStencilRbo_ = 0;

    self = [super initWithFrame:rect];
    if (nil == self)
    {
        return nil;
    }
    return self;
}

- (void)layoutSubviews
{
    uint32_t frameW = (uint32_t)(self.contentScaleFactor * self.frame.size.width);
    uint32_t frameH = (uint32_t)(self.contentScaleFactor * self.frame.size.height);
    [self destroyFrameBuffers];
    [self createFrameBuffers];
    NSG::Window::GetMainWindow()->SetSize(frameW, frameH);    
}

- (void)start
{
    if (nil == displayLink_)
    {
        displayLink_ = [self.window.screen displayLinkWithTarget:self selector:@selector(renderFrame)];
        //[displayLink_ setFrameInterval:1];
        //[displayLink_ addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        //      [displayLink_ addToRunLoop:[NSRunLoop currentRunLoop]];
        [displayLink_ addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    }
}

- (void)stop
{
    if (nil != displayLink_)
    {
        [displayLink_ invalidate];
        displayLink_ = nil;
    }
}

- (void)renderFrame
{
    NSG::Engine::GetPtr()->PerformTicks();
    [context_ presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];

    //s_ctx->m_eventQueue.postMouseEvent(s_defaultWindow, touchLocation.x, touchLocation.y, 0);
    //s_ctx->m_eventQueue.postMouseEvent(s_defaultWindow, touchLocation.x, touchLocation.y, 0, MouseButton::Left, true);
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    //s_ctx->m_eventQueue.postMouseEvent(s_defaultWindow, touchLocation.x, touchLocation.y, 0, MouseButton::Left, false);
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    //s_ctx->m_eventQueue.postMouseEvent(s_defaultWindow, touchLocation.x, touchLocation.y, 0);
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* touch = [[event allTouches] anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    //s_ctx->m_eventQueue.postMouseEvent(s_defaultWindow, touchLocation.x, touchLocation.y, 0, MouseButton::Left, false);
}

- (void)createContext
{
    //float scaleFactor = [[UIScreen mainScreen] scale]; // should use this, but ui is too small on ipad retina
    float scaleFactor = 1.0f;
    [self setContentScaleFactor:scaleFactor ];

    CAEAGLLayer* layer = (CAEAGLLayer*)self.layer;
    layer.opaque = true;

    layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys
                                :[NSNumber numberWithBool:false]
                                , kEAGLDrawablePropertyRetainedBacking
                                , kEAGLColorFormatRGBA8
                                , kEAGLDrawablePropertyColorFormat
                                , nil
                               ];

    context_ = [ [EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    CHECK_ASSERT(nullptr != context_ && "Failed to create kEAGLRenderingAPIOpenGLES2 context.");
    [EAGLContext setCurrentContext:context_];
    [self createFrameBuffers];

}

- (void)createFrameBuffers
{
    CAEAGLLayer* layer = (CAEAGLLayer*)self.layer;
    CHECK_GL_STATUS();
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    glGenRenderbuffers(1, &colorRbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRbo_);

    [context_ renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRbo_);

    GLint width;
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    LOGI("Screen size: %d x %d", width, height);

    glGenRenderbuffers(1, &depthStencilRbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height); // from OES_packed_depth_stencil
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRbo_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRbo_);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (GL_FRAMEBUFFER_COMPLETE != status)
    {
        LOGE("Frame buffer failed with error = 0x%x", status);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, colorRbo_);
    CHECK_GL_STATUS();
}

- (void)destroyFrameBuffers
{
    if (0 != fbo_)
    {
        glDeleteFramebuffers(1, &fbo_);
        fbo_ = 0;
    }

    if (0 != colorRbo_)
    {
        glDeleteRenderbuffers(1, &colorRbo_);
        colorRbo_ = 0;
    }

    if (0 != depthStencilRbo_)
    {
        glDeleteRenderbuffers(1, &depthStencilRbo_);
        depthStencilRbo_ = 0;
    }
}

@end

@interface AppDelegate : UIResponder<UIApplicationDelegate>
{
    UIWindow* window_;
    View* view_;
}

@property (nonatomic, retain) UIWindow* window_;
@property (nonatomic, retain) View* view_;

@end

@implementation AppDelegate

@synthesize window_;
@synthesize view_;

- (BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    CGRect rect = [ [UIScreen mainScreen] bounds];
    window_ = [ [UIWindow alloc] initWithFrame:rect];
    view_ = [ [View alloc] initWithFrame:rect];

    [window_ addSubview:view_];

    UIViewController* viewController = [[ViewController alloc] init];
    viewController.view = view_;

    [window_ setRootViewController:viewController];
    [window_ makeKeyAndVisible];

    [window_ makeKeyAndVisible];
    
    [view_ createContext];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication*)application
{
    //s_ctx->m_eventQueue.postSuspendEvent(s_defaultWindow, Suspend::WillSuspend);
    [view_ stop];
}

- (void)applicationDidEnterBackground:(UIApplication*)application
{
    //s_ctx->m_eventQueue.postSuspendEvent(s_defaultWindow, Suspend::DidSuspend);
}

- (void)applicationWillEnterForeground:(UIApplication*)application
{
    //s_ctx->m_eventQueue.postSuspendEvent(s_defaultWindow, Suspend::WillResume);
}

- (void)applicationDidBecomeActive:(UIApplication*)application
{
    //s_ctx->m_eventQueue.postSuspendEvent(s_defaultWindow, Suspend::DidResume);
    [view_ start];
}

- (void)applicationWillTerminate:(UIApplication*)application
{
    [view_ stop];
}

- (void)dealloc
{
    [window_ release];
    [view_ release];
    [super dealloc];
}

@end


namespace NSG
{
    IOSWindow::IOSWindow(const std::string& name, WindowFlags flags)
        : Window(name),
          flags_(0)
    {
        const AppConfiguration& conf = Engine::GetPtr()->GetAppConfiguration();
        Initialize(conf.x_, conf.y_, conf.width_, conf.height_, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    IOSWindow::IOSWindow(const std::string& name, int x, int y, int width, int height, WindowFlags flags)
        : Window(name),
          flags_(0)
    {
        Initialize(x, y, width, height, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    IOSWindow::~IOSWindow()
    {
        Close();
    }

    void IOSWindow::Initialize(int x, int y, int width, int height, WindowFlags flags)
    {
        Window::SetMainWindow(this);
    }

    void IOSWindow::Close()
    {
        Window::Close();
    }

    void IOSWindow::SetContext()
    {
    }

    void IOSWindow::Destroy()
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

    void IOSWindow::SwapWindowBuffers()
    {
    }

    void IOSWindow::HandleEvents()
    {
    }

    void IOSWindow::Show()
    {
    }

    void IOSWindow::Hide()
    {
    }

    void IOSWindow::Raise()
    {
    }

    void IOSWindow::SetupImgui()
    {
        Window::SetupImgui();
    }

    void IOSWindow::BeginImguiRender()
    {
        ImGuiIO& io = ImGui::GetIO();
        //io.ImeWindowHandle = (void*)hwnd_;
        // Hide OS mouse cursor if ImGui is drawing it
        //ShowCursor(io.MouseDrawCursor ? FALSE : TRUE);
    }

    int IOSWindow::RunApplication()
    {
        NSAutoreleasePool* pool = [ [NSAutoreleasePool alloc] init];
        int exitCode = UIApplicationMain(0, nullptr, @"UIApplication", NSStringFromClass([AppDelegate class]) );
        [pool release];
        return exitCode;
    }
}
#endif
