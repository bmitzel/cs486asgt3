/*
 * Copyright (c) 2005-2013 Michael Shafae
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * $Id: Primitive.h 4472 2013-09-29 07:43:28Z mshafae $
 *
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifndef _FACELIST_H_
#define _FACELIST_H_

/*
 * Macro to allocate/free 2D arrays using C constructs
 */
#define msAlloc2D( type, varName, dim_x, dim_y ) \
{ \
	int __i; \
	if( !((varName) = (type**)calloc( (dim_x), sizeof(type*) )) ){ \
		fprintf( stderr, "Could not allocate memory." ); \
	} \
	for( __i = 0; __i < dim_x; __i++ ){ \
		if( !((varName)[__i] = (type*)calloc( (dim_y), sizeof(type) )) ){ \
		fprintf( stderr, "Could not allocate memory." ); \
		} \
	} \
}

#define msFree2D( varName, dim_x, dim_y ) \
{ \
	int __i; \
	for( __i = 0; __i < dim_x; __i++ ){ \
		free( varName[__i] ); \
	} \
	free( varName ); \
}

/*
 * I have usesd arrays for the vertices, colors, normals, and faces
 * but that does not mean that if you have a Vector class that you can not
 * revise this code to use your Vector class.
 *
 */

class FaceList{
public:
  // array of vertices
  double **vertices;
  //array of vertex colors
  double **colors;
  // array of face indices
  int **faces;
  // vertex count
  int vc;
  // face count
  int fc;
  // The face's surface normal
  double **f_normals;
  double **v_normals;

  // bounding sphere
  double radius;
  double center[3];

  FaceList( int vertexCount, int faceCount ){
    vc = vertexCount;
    fc = faceCount;

		msAlloc2D( double, vertices, vc, 3 );

		msAlloc2D( double, colors, vc, 3 );

		msAlloc2D( double, v_normals, vc, 3 );

		msAlloc2D( double, f_normals, fc, 3 );

		msAlloc2D( int, faces, fc, 3 );
  };

  ~FaceList( ){
		msFree2D( vertices, vc, 3 );
		msFree2D( v_normals, vc, 3 );
		msFree2D( f_normals, fc, 3 );
    msFree2D( faces, fc, 3 );
  };
};

/*std::ostream& operator <<( std::ostream &out, FaceList &fl ){
  int i;
  out << "#Vertices: " << fl.vc << std::endl;
  for( i = 0; i < fl.vc; i++ ){
		out << fl.vertices[i][0] << " " << fl.vertices[i][1] << " " <<  fl.vertices[i][2] << std::endl;
  }
  out << "#Faces " << fl.fc << std::endl;
  for( i = 0; i < fl.fc; i++ ){
		out << fl.faces[i][0] << " " << fl.faces[i][1] << " " << fl.faces[i][2] << std::endl;
  }
  return( out );
}*/

#endif
