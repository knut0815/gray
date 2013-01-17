// Copyright (C) 2012 Chi-kwan Chan
// Copyright (C) 2012 Steward Observatory
//
// This file is part of geode.
//
// Geode is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Geode is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with geode.  If not, see <http://www.gnu.org/licenses/>.

#include "geode.h"
#include <cstdlib>
#include <cuda_gl_interop.h> // OpenGL interoperability runtime API

Data::Data(size_t n_input)
{
  const size_t sz = sizeof(State) * (n = n_input);
  cudaError_t err = cudaErrorMemoryAllocation; // assume we will have problem

#ifndef DISABLE_GL
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sz, 0, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if(GL_NO_ERROR == glGetError())
    err = cudaGraphicsGLRegisterBuffer(&res, vbo,
                                       cudaGraphicsMapFlagsWriteDiscard);
#else
  err = cudaMalloc((void **)&res, sz);
#endif
  if(cudaSuccess != err)
    error("Data::Data(): fail to allocate device memory\n");

  if(!(buf = (State *)malloc(sz)))
    error("Data::Data(): fail to allocate host memory\n");

  t = 0.0;
}

Data::~Data()
{
#ifndef DISABLE_GL
  cudaGraphicsUnregisterResource(res);
  glDeleteBuffers(1, &vbo);
#else
  cudaFree((void *)res);
#endif
  if(buf)
    free(buf);
}