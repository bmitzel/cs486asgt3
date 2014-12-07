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

/*
 * This is not a high quality PLY parser. It works well enough.
 */


#include "PlyModel.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>

#ifndef SQR
#define SQR( x ) ((x) * (x))
#endif

int vecPrint3d(FILE* f, double *v){
  return fprintf( f, "(%.16f, %.16f, %.16f)\n", v[0], v[1], v[2] );
}

void vecCopy3d(double *dest, double *src){
  int i;
  for(i = 0; i < 3; i++){
    dest[i] = src[i];
  }
}

double vecSquaredLength3d(double *a){
  double accumulate = 0.0;
  int i;
  for(i = 0; i < 3; i++){
    accumulate += SQR( a[i] );
  }
  return( accumulate );
}


double vecLength3d(double *a){
  return(sqrt( vecSquaredLength3d(a) ) );
}

void vecDifference3d(double *c, double *a, double *b){
  int i;
  for(i = 0; i < 3; i++){
    c[i] = a[i] - b[i];
  }
}


void vecCross3d(double *n, double *u, double *v){
  n[0] = u[1] * v[2] - u[2] * v[1];
  n[1] = u[2] * v[0] - u[0] * v[2];
  n[2] = u[0] * v[1] - u[1] * v[0];
}


void vecNormalize3d(double *vout, double* v){
  double length;
  length = vecLength3d(v);
   if( length <= 0.0 ){
     vout[0] = 0.0;
     vout[1] = 0.0;
     vout[2] = 0.0;
   }
   if( length == 1.0 ){
     vecCopy3d(vout, v);
   }else{
    length = 1.0 / length;
    vout[0] = v[0] * length;
    vout[1] = v[1] * length;
    vout[2] = v[2] * length;
  }
}

void vecCalcNormal3d(double *n, double *p, double *p1, double *p2){
  double pa[3],pb[3];

  vecDifference3d(pa, p1, p);
  vecDifference3d(pb, p2, p);

  vecCross3d(n, pa, pb);
  vecNormalize3d(n, n);
}

double vecDistanceBetween3d(double *a, double *b ){
  double c[3];
  vecDifference3d(c, a, b);
  return( vecLength3d(c) );
}

double vecSquaredDistanceBetween3d(double *a, double *b ){
  double c[3];
  vecDifference3d(c, a, b);
  return( vecSquaredLength3d(c) );
}


void vecSum3d(double *c, double *a, double *b){
  int i;
  for(i = 0; i < 3; i++){
    c[i] = a[i] + b[i];
  }
}

double r(){
  return double(rand( ))/double(RAND_MAX);
}

void midpoint(double *m, double *a, double *b){
  double s[3];
  vecSum3d(s, a, b);
  for(int i = 0; i < 3; i++ ){
    m[i] = s[i] * 0.5;
  }
}

void calcBoundingSphere(double *center, double *radius, FaceList *fl){
  double maxDistance = 0.0;
  for( int i = 0; i < fl->vc-1; i++ ){
    for(int j = i + 1; j < fl->vc; j++){
      double *a = fl->vertices[i];
      double *b = fl->vertices[j];
      double distance = vecSquaredDistanceBetween3d(a, b);
      if( distance > maxDistance){
        midpoint(center, a, b);
        *radius = sqrt(distance) * 0.5;
        maxDistance = distance;
      }
    }
  }
}

FaceList* readPlyModel( const char* filename ){
  char buffer[255], type[128], c;
  std::ifstream inputfile;
  unsigned int i;
  int k;
  unsigned int nv;
  unsigned int nf;
  FaceList *fl;
  assert( filename );
  inputfile.open( filename, std::ios::in );
  if( inputfile.fail( ) ){
    std::cerr << "File \"" << filename << "\" not found." << std::endl;
    exit( 1 );
  }
  // Parse the header
  if(inputfile.getline(buffer, sizeof(buffer), '\n') != NULL){
    if( strcmp(buffer, "ply") != 0){
      std::cerr << "Error: Input file is not of .ply type." << std::endl;
      exit(1);
    }
  }else{
    std::cerr << "End of input?" << std::endl;
    exit( 1 );
  }
  if(inputfile.getline(buffer, sizeof(buffer), '\n') != NULL){
    if( strncmp(buffer, "format ascii", 12) != 0){
      std::cerr << "Error: Input file is not in ASCII format." << std::endl;
      exit(1);
    }
  }else{
    std::cerr << "End of input?" << std::endl;
    exit( 1 );
  }
  if(inputfile.getline(buffer, sizeof(buffer), '\n') != NULL){
    while (strncmp(buffer, "comment", 7) == 0){
      inputfile.getline(buffer, sizeof(buffer), '\n');
  }
  }else{
    std::cerr << "End of input?" << std::endl;
    exit( 1 );
  }

  if (strncmp(buffer, "element vertex", 14) == 0){
    sscanf(buffer, "element vertex %u\n", &nv);
  }else{
    std::cerr << "Error: number of vertices expected." << std::endl;
    exit(1);
  }

  i = 0;
  inputfile.getline(buffer, sizeof(buffer), '\n');
  while (strncmp(buffer, "property", 8) == 0) {
    if (i < 3) {
      sscanf(buffer, "property %s %c\n", type, &c);
      switch (i) {
      case 0:
        if (c != 'x') {
          std::cerr << "Error: first coordinate is not x." << std::endl;
          exit(1);
        }
        break;
      case 1:
        if (c != 'y') {
          std::cerr << "Error: first coordinate is not y." << std::endl;
          exit(1);
        }
        break;
      case 2:
        if (c != 'z') {
          std::cerr << "Error: first coordinate is not z." << std::endl;
          exit(1);
        }
        break;
      default:
        break;
      }
      i++;
    }
    inputfile.getline(buffer, sizeof(buffer), '\n');
  }

  if (strncmp(buffer, "element face", 12) == 0)
    sscanf(buffer, "element face %u\n", &nf);
  else {
    std::cerr << "Error: number of faces expected." << std::endl;
    exit(1);
  }

  inputfile.getline(buffer, sizeof(buffer), '\n');
  if (strncmp(buffer, "property list", 13) != 0) {
    std::cerr << "Error: property list expected." << std::endl;
    exit(1);
  }

  inputfile.getline(buffer, sizeof(buffer), '\n');
  while (strncmp(buffer, "end_header", 10) != 0){
    inputfile.getline(buffer, sizeof(buffer), '\n');
  }

  // Allocate FaceList object
  if( !(fl = new FaceList( nv, nf)) ){
    std::cerr << "Could not allocate a new face list for the model." << std::endl;
    exit(1);
  }

  /* Process the body of the input file*/

  // read vertex data from PLY file
  for (i = 0; i < nv; i++) {
    inputfile.getline(buffer, sizeof(buffer), '\n');
    sscanf(buffer,"%lf %lf %lf", &(fl->vertices[i][0]), &(fl->vertices[i][1]), &(fl->vertices[i][2]));
  }

  // read face data from PLY file
  for (i = 0; i < nf; i++) {
    inputfile.getline(buffer, sizeof(buffer), '\n');
    sscanf(buffer, "%d %d %d %d", &k, &(fl->faces[i][0]), &(fl->faces[i][1]), &(fl->faces[i][2]) );
    if (k != 3) {
      fprintf(stderr, "Error: not a triangular face.\n");
      exit(1);
    }
  }

  inputfile.close( );

  calcBoundingSphere(fl->center, &(fl->radius), fl);
  for( i = 0; i < nv; i++){
    vecDifference3d(fl->vertices[i], fl->vertices[i], fl->center);
  }

  // compute face normals
  for( i = 0; i < nf; i++){
    vecCalcNormal3d(fl->f_normals[i],
      fl->vertices[fl->faces[i][0]],
      fl->vertices[fl->faces[i][1]],
      fl->vertices[fl->faces[i][2]]);

    for(int j = 0; j < 3; j++){
      vecSum3d(fl->v_normals[fl->faces[i][j]],
        fl->v_normals[fl->faces[i][j]], fl->f_normals[i] );
    }
  }

  // normalize the v_normals
  for( i = 0; i < nv; i++ ){
    for(int j = 0; j < 3; j++){
      // set some colors
      fl->colors[i][j] = r( );
    }
    vecNormalize3d(fl->v_normals[i], fl->v_normals[i]);
  }

  puts("Done");
  return( fl );
}

