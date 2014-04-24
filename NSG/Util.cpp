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

#include "Util.h"

#ifdef NACL
char* realpath(const char* in, char* out)
{
	return nullptr;
}
#endif

namespace NSG
{
	void DecomposeMatrix(const Matrix4& m, Vertex3& position, Quaternion& q, Vertex3& scale)
	{
	    Vertex3 scaling(glm::length(m[0]), glm::length(m[1]), glm::length(m[2]));

	    Matrix3 tmp1(glm::scale(glm::mat4(1.0f), Vertex3(1)/scaling) * m);

	    q = glm::quat_cast(tmp1);

	    position = Vertex3(m[3]);

	    Matrix3 tmp2(glm::inverse(tmp1) * Matrix3(m));

	    scale = Vertex3(tmp2[0].x, tmp2[1].y, tmp2[2].z);
	}
}
