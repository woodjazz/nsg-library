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
#include "Main.h"
#include "Log.h"

#if defined(IS_TARGET_ANDROID) && !defined(SDL)

namespace NSG
{
    struct android_app* androidApp = nullptr;
}

extern int NSG_main(int argc, char* argv[]);
void android_main(struct android_app* app)
{
    LOGI("android_main");
    NSG::androidApp = app;
    NSG_main(0, nullptr);
}

#elif defined(IS_TARGET_OSX) && !defined(SDL)
#import <Cocoa/Cocoa.h>
extern int NSG_main(int argc, char* argv[]);
int main(int argc, char* argv[])
{	
    struct AutoreleasePoolHolder
    {
        AutoreleasePoolHolder() : pool_([[NSAutoreleasePool alloc] init]) {}
        ~AutoreleasePoolHolder() { [pool_ release]; }
    private:
        NSAutoreleasePool* const pool_;
    } pool;

    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp finishLaunching];

    [[NSNotificationCenter defaultCenter]
     postNotificationName:NSApplicationWillFinishLaunchingNotification
     object:NSApp];

    [[NSNotificationCenter defaultCenter]
     postNotificationName:NSApplicationDidFinishLaunchingNotification
     object:NSApp];

    id quitMenuItem = [NSMenuItem new];
    [quitMenuItem
     initWithTitle:@"Quit"
     action:@selector(terminate:)
     keyEquivalent:@"q"];

    id appMenu = [NSMenu new];
    [appMenu addItem:quitMenuItem];

    id appMenuItem = [NSMenuItem new];
    [appMenuItem setSubmenu:appMenu];

    id menubar = [[NSMenu new] autorelease];
    [menubar addItem:appMenuItem];
    [NSApp setMainMenu:menubar];

    NSG_main(argc, argv);
}
#endif
