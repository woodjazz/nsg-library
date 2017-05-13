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
#pragma once
#include "Check.h"
#include <memory>

namespace NSG {
template <typename T> std::shared_ptr<Node> SharedFromPointerNode(T* p) {
    try {
        return p->shared_from_this();
    } catch (...) {
        CHECK_CONDITION(!"shared_from_this has failed!!!");
    }
    return nullptr;
}

template <typename T>
std::shared_ptr<Resource> SharedFromPointerResource(T* p) {
    try {
        return p->shared_from_this();
    } catch (...) {
        CHECK_CONDITION(!"shared_from_this has failed!!!");
    }
    return nullptr;
}

template <typename T> std::shared_ptr<T> SharedFromPointer(T* p) {
    try {
        return p->shared_from_this();
    } catch (...) {
        // CHECK_CONDITION(!"shared_from_this has failed!!!");
    }
    return nullptr;
}
}
