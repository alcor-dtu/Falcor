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
#include "Framework.h"
#ifdef FALCOR_GL
#include "Core/GpuTimer.h"

namespace Falcor
{
    GpuTimer::SharedPtr GpuTimer::create()
    {
        return SharedPtr(new GpuTimer);
    }

    GpuTimer::GpuTimer()
    {
        gl_call(glGenQueries(arraysize(mApiHandle), mApiHandle));
    }

    GpuTimer::~GpuTimer()
    {
        glDeleteQueries(arraysize(mApiHandle), mApiHandle);
    }

    void GpuTimer::begin()
    {
        if(mBeginCalled)
        {
            Logger::log(Logger::Level::Warning, "CGpuTimer::begin() was called before CGpuTimer::End(). Ignoring call");
            return;
        }
        mBeginCalled = true;
        mEndCalled = false;
        gl_call(glQueryCounter(mApiHandle[0], GL_TIMESTAMP));
    }

    void GpuTimer::end()
    {
        if(mBeginCalled == false)
        {
            Logger::log(Logger::Level::Warning, "CGpuTimer::end() was called without a prior call to CGpuTimer::Begin(). Ignoring call");
            return;
        }
        mEndCalled = true;
        mBeginCalled = false;
        gl_call(glQueryCounter(mApiHandle[1], GL_TIMESTAMP));
    }

    bool GpuTimer::getElapsedTime(bool waitForResult, float& elapsedTime)
    {
        if(mEndCalled == false)
        {
            Logger::log(Logger::Level::Error, "CGpuTimer::getElapsedTime() was called before CGpuTimer::End(). Ignoring call");
            return false;
        }

        GLint isDone;
        do
        {
            gl_call(glGetQueryObjectiv(mApiHandle[1], GL_QUERY_RESULT_AVAILABLE, &isDone));
        } while(waitForResult && (isDone == GL_FALSE));

        if(isDone)
        {
            uint64_t start, end;
            gl_call(glGetQueryObjectui64v(mApiHandle[0], GL_QUERY_RESULT, &start));
            gl_call(glGetQueryObjectui64v(mApiHandle[1], GL_QUERY_RESULT, &end));
            elapsedTime = (float)((double)(end - start) * (1.0e-6f));
        }

        return (isDone != GL_FALSE);
    }
}
#endif //#ifdef FALCOR_GL
