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
#include "TextureCube.h"
#include "Check.h"
#include "Util.h"

namespace NSG {
TextureCube::TextureCube(const std::string& name) : Texture(name) {
    SetWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
}

TextureCube::~TextureCube() {}

GLenum TextureCube::GetTarget() const { return GL_TEXTURE_CUBE_MAP; }

void TextureCube::Define() {
    CHECK_GL_STATUS();

    for (unsigned i = 0; i < (unsigned)CubeMapFace::MAX_CUBEMAP_FACES; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format_, width_,
                     height_, 0, format_, type_, nullptr);

        CHECK_GL_STATUS();
    }

    CHECK_GL_STATUS();
}
}
