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
#include <memory>
#include "Core/FBO.h"
#include "Core/Texture.h"
#include "glm/vec2.hpp"
#include <vector>
#include "OpenVR/VRDisplay.h"

namespace Falcor
{
    class VrFbo
    {
    public:
        using UniquePtr = std::unique_ptr<VrFbo>;
        /** Create a new VrFbo. It will create array resources for color and depth. It will also create views into each array-slice
            \param[in] colorFormat Valid resource format for the color texture
            \param[in] depthFormat Either a valid depth format or ResourceFormat::Unknown if depth is not required
        */
        static UniquePtr create(ResourceFormat colorFormat[], uint32_t rtCount, ResourceFormat depthFormat, uint32_t sampleCount = 0, const glm::ivec2& rtSize = glm::ivec2(0, 0));

        /** Submit the color target into the HMD
        */
        void submitToHmd(uint32_t rtIndex = 0) const;

        /** Pushs a viewport matching the FBO size into the render context
        */
        void pushViewport(RenderContext* pContext, uint32_t vpIndex);

        /** Get the FBO
        */
        Fbo::SharedPtr getFbo() const { return mpFbo; }

        /** Get the resource view to an eye's resource view
        */
        Texture::SharedConstPtr getEyeResourceView(VRDisplay::Eye eye, uint32_t rtIndex = 0) const { return (eye == VRDisplay::Eye::Left) ? mpLeftView[rtIndex] : mpRightView[rtIndex]; }

    private:
        Fbo::SharedPtr mpFbo;
        std::vector<Texture::SharedPtr> mpLeftView;
        std::vector<Texture::SharedPtr> mpRightView;
    };
}