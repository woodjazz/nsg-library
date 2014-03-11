/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

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
#pragma once

#define 	NSG_RELEASE   	0
#define 	NSG_PRESS   	1
#define 	NSG_REPEAT   	2

#define 	NSG_MOD_SHIFT		0x0001
#define 	NSG_MOD_CONTROL   	0x0002
#define 	NSG_MOD_ALT   		0x0004
#define 	NSG_MOD_SUPER   	0x0008

#define 	NSG_KEY_UNKNOWN   -1
#define 	NSG_KEY_SPACE   32
#define 	NSG_KEY_APOSTROPHE   39 /* ' */
#define 	NSG_KEY_COMMA   44 /* , */
#define 	NSG_KEY_MINUS   45 /* - */
#define 	NSG_KEY_PERIOD   46 /* . */
#define 	NSG_KEY_SLASH   47 /* / */
#define 	NSG_KEY_0   48
#define 	NSG_KEY_1   49
#define 	NSG_KEY_2   50
#define 	NSG_KEY_3   51
#define 	NSG_KEY_4   52
#define 	NSG_KEY_5   53
#define 	NSG_KEY_6   54
#define 	NSG_KEY_7   55
#define 	NSG_KEY_8   56
 
#define 	NSG_KEY_9   57
 
#define 	NSG_KEY_SEMICOLON   59 /* ; */
 
#define 	NSG_KEY_EQUAL   61 /* = */
 
#define 	NSG_KEY_A   65
 
#define 	NSG_KEY_B   66
 
#define 	NSG_KEY_C   67
 
#define 	NSG_KEY_D   68
 
#define 	NSG_KEY_E   69
 
#define 	NSG_KEY_F   70
 
#define 	NSG_KEY_G   71
 
#define 	NSG_KEY_H   72
 
#define 	NSG_KEY_I   73
 
#define 	NSG_KEY_J   74
 
#define 	NSG_KEY_K   75
 
#define 	NSG_KEY_L   76
 
#define 	NSG_KEY_M   77
 
#define 	NSG_KEY_N   78
 
#define 	NSG_KEY_O   79
 
#define 	NSG_KEY_P   80
 
#define 	NSG_KEY_Q   81
 
#define 	NSG_KEY_R   82
 
#define 	NSG_KEY_S   83
 
#define 	NSG_KEY_T   84
 
#define 	NSG_KEY_U   85
 
#define 	NSG_KEY_V   86
 
#define 	NSG_KEY_W   87
 
#define 	NSG_KEY_X   88
 
#define 	NSG_KEY_Y   89
 
#define 	NSG_KEY_Z   90
 
#define 	NSG_KEY_LEFT_BRACKET   91 /* [ */
 
#define 	NSG_KEY_BACKSLASH   92 /* \ */
 
#define 	NSG_KEY_RIGHT_BRACKET   93 /* ] */
 
#define 	NSG_KEY_GRAVE_ACCENT   96 /* ` */
 
#define 	NSG_KEY_WORLD_1   161 /* non-US #1 */
 
#define 	NSG_KEY_WORLD_2   162 /* non-US #2 */
 
#define 	NSG_KEY_ESCAPE   256
 
#define 	NSG_KEY_ENTER   257
 
#define 	NSG_KEY_TAB   258
 
#define 	NSG_KEY_BACKSPACE   259
 
#define 	NSG_KEY_INSERT   260
 
#define 	NSG_KEY_DELETE   261
 
#define 	NSG_KEY_RIGHT   262
 
#define 	NSG_KEY_LEFT   263
 
#define 	NSG_KEY_DOWN   264
 
#define 	NSG_KEY_UP   265
 
#define 	NSG_KEY_PAGE_UP   266
 
#define 	NSG_KEY_PAGE_DOWN   267
 
#define 	NSG_KEY_HOME   268
 
#define 	NSG_KEY_END   269
 
#define 	NSG_KEY_CAPS_LOCK   280
 
#define 	NSG_KEY_SCROLL_LOCK   281
 
#define 	NSG_KEY_NUM_LOCK   282
 
#define 	NSG_KEY_PRINT_SCREEN   283
 
#define 	NSG_KEY_PAUSE   284
 
#define 	NSG_KEY_F1   290
 
#define 	NSG_KEY_F2   291
 
#define 	NSG_KEY_F3   292
 
#define 	NSG_KEY_F4   293
 
#define 	NSG_KEY_F5   294
 
#define 	NSG_KEY_F6   295
 
#define 	NSG_KEY_F7   296
 
#define 	NSG_KEY_F8   297
 
#define 	NSG_KEY_F9   298
 
#define 	NSG_KEY_F10   299
 
#define 	NSG_KEY_F11   300
 
#define 	NSG_KEY_F12   301
 
#define 	NSG_KEY_F13   302
 
#define 	NSG_KEY_F14   303
 
#define 	NSG_KEY_F15   304
 
#define 	NSG_KEY_F16   305
 
#define 	NSG_KEY_F17   306
 
#define 	NSG_KEY_F18   307
 
#define 	NSG_KEY_F19   308
 
#define 	NSG_KEY_F20   309
 
#define 	NSG_KEY_F21   310
 
#define 	NSG_KEY_F22   311
 
#define 	NSG_KEY_F23   312
 
#define 	NSG_KEY_F24   313
 
#define 	NSG_KEY_F25   314
 
#define 	NSG_KEY_KP_0   320
 
#define 	NSG_KEY_KP_1   321
 
#define 	NSG_KEY_KP_2   322
 
#define 	NSG_KEY_KP_3   323
 
#define 	NSG_KEY_KP_4   324
 
#define 	NSG_KEY_KP_5   325
 
#define 	NSG_KEY_KP_6   326
 
#define 	NSG_KEY_KP_7   327
 
#define 	NSG_KEY_KP_8   328
 
#define 	NSG_KEY_KP_9   329
 
#define 	NSG_KEY_KP_DECIMAL   330
 
#define 	NSG_KEY_KP_DIVIDE   331
 
#define 	NSG_KEY_KP_MULTIPLY   332
 
#define 	NSG_KEY_KP_SUBTRACT   333
 
#define 	NSG_KEY_KP_ADD   334
 
#define 	NSG_KEY_KP_ENTER   335
 
#define 	NSG_KEY_KP_EQUAL   336
 
#define 	NSG_KEY_LEFT_SHIFT   340
 
#define 	NSG_KEY_LEFT_CONTROL   341
 
#define 	NSG_KEY_LEFT_ALT   342
 
#define 	NSG_KEY_LEFT_SUPER   343
 
#define 	NSG_KEY_RIGHT_SHIFT   344
 
#define 	NSG_KEY_RIGHT_CONTROL   345
 
#define 	NSG_KEY_RIGHT_ALT   346
 
#define 	NSG_KEY_RIGHT_SUPER   347
 
#define 	NSG_KEY_MENU   348
 
#define 	NSG_KEY_LAST   NSG_KEY_MENU