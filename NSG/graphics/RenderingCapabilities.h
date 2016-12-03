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
#pragma once
#include "Types.h"
#include "Singleton.h"
#include "GLIncludes.h"

namespace NSG
{
	// Keeps OpenGL's context
	class RenderingCapabilities : public Singleton<RenderingCapabilities>
	{
	public:
		~RenderingCapabilities();
		bool CheckExtension(const std::string& name);
		bool HasVertexArrayObject() const { return has_vertex_array_object_ext_; }
		bool HasMapBufferRange() const { return has_map_buffer_range_ext_; }
		bool HasDepthTexture() const { return has_depth_texture_ext_; }
		bool HasDepthComponent24() const { return has_depth_component24_ext_; }
		bool HasNonPowerOfTwo() const { return has_texture_non_power_of_two_ext_; }
		bool HasInstancedArrays() const { return has_instanced_arrays_ext_; }
		bool HasPackedDepthStencil() const { return has_packed_depth_stencil_ext_; }
		bool HasTextureCompressionDXT1() const { return has_texture_compression_dxt1_ext_; }
		bool HasTextureCompressionDXT3() const { return has_texture_compression_dxt3_ext_; }
		bool HasTextureCompressionDXT5() const { return has_texture_compression_dxt5_ext_; }
		bool HasTextureCompressionETC() const { return has_compressed_ETC1_RGB8_texture_ext_; }
		bool HasTextureCompressionPVRTC() const { return has_texture_compression_pvrtc_ext_; }
		bool HasDiscardFramebuffer() const { return has_discard_framebuffer_ext_; }
		GLint GetMaxVaryingVectors() const { return maxVaryingVectors_; }
		GLint GetMaxTexturesCombined() const { return maxTexturesCombined_; }
		GLint GetMaxVertexUniformVectors() const { return maxVertexUniformVectors_; }
		GLint GetMaxFragmentUniformVectors() const { return maxFragmentUniformVectors_; }
		GLint GetMaxVertexAttribs() const { return maxVertexAttribs_; }
		int GetMaxTextureSize() const { return maxTextureSize_; }
		static GLenum GetTexelDataType();
		static GLenum GetTexelFormatType();
		static std::string GetExtensions();
	private:
		RenderingCapabilities();
		bool has_discard_framebuffer_ext_;
		bool has_vertex_array_object_ext_;
		bool has_map_buffer_range_ext_;
		bool has_depth_texture_ext_;
		bool has_depth_component24_ext_;
		bool has_texture_non_power_of_two_ext_;
		bool has_instanced_arrays_ext_;
		bool has_packed_depth_stencil_ext_;
		bool has_texture_compression_dxt1_ext_;
		bool has_texture_compression_dxt3_ext_;
		bool has_texture_compression_dxt5_ext_;
		bool has_compressed_ETC1_RGB8_texture_ext_;
		bool has_texture_compression_pvrtc_ext_;
		GLint maxVaryingVectors_;
		GLint maxTexturesCombined_;
		GLint maxVertexUniformVectors_;
		GLint maxFragmentUniformVectors_;
		GLint maxVertexAttribs_;
		int maxTextureSize_;
		std::string extensions_;
		friend class Singleton<RenderingCapabilities>;
	};
}
