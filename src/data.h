// Copyright (C) 2012--2014 Chi-kwan Chan
// Copyright (C) 2012--2014 Steward Observatory
//
// This file is part of GRay.
//
// GRay is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GRay is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GRay.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DATA_H
#define DATA_H

class Data {
  size_t n, m, gsz, bsz;
#ifdef ENABLE_GL
  void   setup(Para &); // implemented in "src/optional/vis.cc"
  GLuint vbo;
  struct cudaGraphicsResource *res;
#else
  State  *res; // device resource
#endif
  State  *buf; // host buffer
  size_t *count_res;
  size_t *count_buf;
  bool    mapped;

  cudaError_t sync(size_t *); // implemented in "src/core.cu"
  cudaError_t dtoh();         // implemented in "src/memcpy.cc"
  cudaError_t htod();         // implemented in "src/memcpy.cc"
  cudaError_t deactivate();   // implemented in "src/interop.cc"
  State      *device();       // implemented in "src/interop.cc"
  State      *host();         // implemented in "src/interop.cc"

 public:
  Data(size_t, Para &); // implemented in "src/data.cc"
  ~Data();              // implemented in "src/data.cc"

  cudaError_t init  (double);         // implemented in "src/core.cu"
  cudaError_t evolve(double, double); // implemented in "src/core.cu"

  void dump(const char *, double);
  void spec(const char *);
#ifdef ENABLE_GL
  void show(); // implemented in "src/optional/vis.cc"
#endif
};

#endif // DATA_H
