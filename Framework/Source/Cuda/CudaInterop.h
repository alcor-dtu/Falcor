/***************************************************************************
# Copyright (c) 2015, NVIDIA CORPORATION. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of NVIDIA CORPORATION nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************/
#pragma once

#include <Falcor.h>
#include <Core/Buffer.h>
#include <Core/Texture.h>


#include "CudaTexture.h"
#include "CudaBuffer.h"

#include <unordered_map>

namespace Falcor {
namespace Cuda { 

	/** Interoperability interface with CUDA.
		Based on the CUDA driver API.
	*/
	class CudaInterop
	{
	private:
		CudaInterop(){}
		CudaInterop(CudaInterop const&);
		void operator=(CudaInterop const&);
	protected:

		template<class FR, class FCR>
		struct CuInteropMapVal 
        {
			CUgraphicsResource		cudaGraphicsResource;

			std::shared_ptr<FR>		pFalcorGraphicsResource		= nullptr;
			std::shared_ptr<FCR>	pFalcorCudaGraphicsResource = nullptr;
			
		};

		std::unordered_map< size_t, CuInteropMapVal<Falcor::Texture, Cuda::CudaTexture> >	mTextureMap;
		std::unordered_map< size_t, CuInteropMapVal<Falcor::Buffer, Cuda::CudaBuffer> >		mBufferMap;

	public:
		~CudaInterop()
        {	
			for (auto it = mTextureMap.begin(); it != mTextureMap.end(); ++it)
				cuGraphicsUnregisterResource(it->second.cudaGraphicsResource);

			for (auto it = mBufferMap.begin(); it != mBufferMap.end(); ++it)
				cuGraphicsUnregisterResource(it->second.cudaGraphicsResource);

		}

		/** Provides singleton instance.
		*/
		static CudaInterop& get();

		/** Map a Falcor texture to Cuda if not mapped already, and provides the representing CudaTexture.
			Texture must be OpenGL based.
		*/
		std::shared_ptr<Cuda::CudaTexture> getMappedCudaTexture(const Falcor::Texture::SharedConstPtr& tex);

		/** Unmap a Falcor texture from Cuda. Must be called before accessing the texture again in GL.
		*/
        void unmapCudaTexture(const Falcor::Texture::SharedConstPtr& tex);
	
		/** Map a Falcor buffer to Cuda if not mapped already, and provides the representing CudaTexture.
			Buffer must be OpenGL based.
		*/
		std::shared_ptr<Cuda::CudaBuffer> getMappedCudaBuffer(const Falcor::Buffer::SharedConstPtr& buff);

		/** Unmap a Falcor texture from Cuda. Must be called before accessing the texture again in GL.
		*/
        void unmapCudaBuffer(const Falcor::Buffer::SharedConstPtr& buff);
	};

}
}