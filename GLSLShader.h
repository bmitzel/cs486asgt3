/*
 * Copyright (c) 2005-2008 Michael Shafae.
 * All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $Id: GLSLShader.h 4926 2014-04-16 20:30:28Z mshafae $
 *
 * Utility functions for loading a shader.
 *
 * STUDENTS DO NOT NEED TO MAKE ANY CHANGES TO THIS FILE.
 *
 */

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __APPLE__
/* Apple's weird location of their OpenGL & GLUT implementation */
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif


#ifndef _GLSLSHADER_H_
#define _GLSLSHADER_H_

#define msglError( ) _msglError( stderr, __FILE__, __LINE__ )

static bool _msglError( FILE *out, const char *filename, int line ){
  bool ret = false;
  GLenum err = glGetError( );
  while( err != GL_NO_ERROR ) {
    ret = true;
    fprintf( out, "GL ERROR:%s:%d: %s\n",
      filename, line, (char*)gluErrorString( err ) );
    err = glGetError( );
  }
  return( ret );
}

static char* file2strings( const char *filename ){
  long fileLength, numberOfCharsRead;
  char *strings;
  FILE *fhandle = fopen( filename, "r" );
  if( !fhandle ){
    fprintf( stderr, "%s: Can't open file %s for reading\n", __FUNCTION__, filename );
    return NULL;
  }
  fseek( fhandle, 0, SEEK_END );
  fileLength = ftell( fhandle );

  fseek( fhandle, 0, SEEK_SET );

  if( !(strings = (char*)calloc( (fileLength+1), sizeof(char) )) ){
    fprintf( stderr, "%s: Can't malloc space for string buffer %s\n", __FUNCTION__, filename );
    return NULL;
  }

  numberOfCharsRead = (long)fread( strings, sizeof(char), fileLength, fhandle );

  strings[numberOfCharsRead] = '\0';

  fclose( fhandle );

  return( strings );
}

class Shader{
public:
  GLuint _object;
  char *_srcFileName;
  
  Shader( const char *srcFileName ){
    _srcFileName = strdup(srcFileName);
  };

  ~Shader( ){
    glDeleteShader( _object );
    free(_srcFileName);
    msglError( );
  }

  bool compileShader( const GLchar *src ){
    GLint compiled_ok;
    char *msg;
    GLint length = (GLint)strlen(src);
    glShaderSource( _object, 1, &src, &length );
    glCompileShader( _object );
    msglError( );
    glGetShaderiv( _object, GL_COMPILE_STATUS, &compiled_ok );
    msglError( );
    if( !compiled_ok ){
      fprintf( stderr, "Compilation failed for shader %s\n",
        _srcFileName );
      msg = getInfoLog( );
      fprintf( stderr, "Info Log:\n%s\n", msg );
      free( msg );
    }
    return( compiled_ok == 1 );
  }
  
  char* getInfoLog( ){
    GLint info_log_length;
    glGetShaderiv( _object, GL_INFO_LOG_LENGTH, &info_log_length );
    char *info_log = (char*)malloc( info_log_length );
    glGetShaderInfoLog( _object, info_log_length, NULL, info_log);
    return( info_log );
  }
};

class VertexShader : public Shader{

public:
  VertexShader( const char *srcFileName ) : Shader(srcFileName){
    char *src = file2strings( _srcFileName );
    if( (Shader::_object = glCreateShader( GL_VERTEX_SHADER )) == 0 ){
      fprintf( stderr, "Can't generate vertex shader name\n" );
    }
    msglError( );
    compileShader( src );
    msglError( );
    free( src );
  }

  GLuint object( ){
    return Shader::_object;
  }

};


class FragmentShader : public Shader{

public:
  FragmentShader( const char *srcFileName ) : Shader(srcFileName){
    char *src = file2strings( _srcFileName );
    if( (Shader::_object = glCreateShader( GL_FRAGMENT_SHADER )) == 0 ){
      fprintf( stderr, "Can't generate fragment shader name\n" );
      exit(1);
    }
      compileShader( src );
      free( src );
    }

    GLuint object( ){
      return Shader::_object;
    }
};


class GLSLProgram{

public:
  GLuint _object;

  GLSLProgram( ){
    if( (_object = glCreateProgram( )) == 0 ){
      fprintf( stderr, "Can't generate program name\n" );
      exit(1);
    }
  }

  ~GLSLProgram( ){
    detachAll( );
    glDeleteProgram( _object );
  }

  GLuint id( ){
    return _object;
  }
  
  bool attach( FragmentShader &fs ){
    glAttachShader( _object, fs.object( ) );
    return( !msglError( ) );
  }

  bool attach( VertexShader &vs ){
    glAttachShader( _object, vs.object( ) );
    return( !msglError( ) );
  }

  bool detachAll( ){
    bool ret = false;
    GLsizei const maxCount = 32;
    GLsizei count;
    GLuint shaders[32];
    glGetAttachedShaders( _object, maxCount, &count, shaders );
    for(int i = 0; i < count; i++ ){
      if( !(detach( shaders[i] )) ){
        ret = true;
      }
    }
    return( ret );
  }

  bool detach( GLuint shaderName ){
    glDetachShader( _object, shaderName );
    return( !msglError( ) );
  }

  bool detach( FragmentShader &fs ){
    glDetachShader( _object, fs.object( ) );
    return( !msglError( ) );
  }

  bool detach( VertexShader &vs ){
    glDetachShader( _object, vs.object( ) );
    return( !msglError( ) );
  }

  bool link( ){
    GLint linked_ok;
    char *msg;
    bool ret = true;
  
    glLinkProgram( _object );

    glGetProgramiv( _object, GL_LINK_STATUS, &linked_ok );
    if( !linked_ok ){
      ret = false;
      fprintf( stderr, "Linking failed.\n" );
      msg = getInfoLog( );
      fprintf( stderr, "%s\n", msg );
      free(msg );
    }
    return ret;
  }
  
  char* getInfoLog( ){
    GLint info_log_length;
    glGetProgramiv( _object, GL_INFO_LOG_LENGTH, &info_log_length );
    char *info_log = (char*)malloc( info_log_length );
    glGetProgramInfoLog( _object, info_log_length, NULL, info_log);
    return( info_log );
  }
  
  bool activate( ){
    glUseProgram( _object );
    return( !msglError( ) );
  }
  
  bool deactivate( ){
    glUseProgram( 0 );
    return( !msglError( ) );
  }
  
  bool isActive( ){
    int id;
    // slow!
    glGetIntegerv(GL_CURRENT_PROGRAM,&id);
    return( id == _object );
  }
  
  bool isHardwareAccelerated( ){
    bool ret = true;
     #if defined(__APPLE__)
     /*
      * This checks to make sure that the shaders are going to
      * be hardware accelerated.
      * If they aren't, a message is printed.
      *
      * Something similar can be done on Unix as well as MS Windows but
      * it is left up to the student to look up the neccessary API calls.
      */
    GLint hardwareAccelerated;

    CGLGetParameter( CGLGetCurrentContext( ), kCGLCPGPUVertexProcessing, &hardwareAccelerated );
    if( !hardwareAccelerated ){
      ret = false;
      fprintf( stderr,
        "Warning: Vertex shader is NOT being hardware-accelerated\n" );
    }
    CGLGetParameter( CGLGetCurrentContext( ), kCGLCPGPUFragmentProcessing, &hardwareAccelerated );
    if( !hardwareAccelerated ){
      ret = false;
      fprintf( stderr,
        "Warning: Fragment shader is NOT being hardware-accelerated\n" );
    }
     #endif
    return ret;
  }
};

#endif
