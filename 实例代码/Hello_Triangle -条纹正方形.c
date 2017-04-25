// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this
//    example is to demonstrate the basic concepts of
//    OpenGL ES 3.0 rendering.
#include "esUtil.h"
#include "math.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

      // MVP matrix
   ESMatrix  mvpMatrix;

      // Uniform locations
   GLint  mvpLoc;

      // Rotation angle
   GLfloat   angle;


     GLfloat  *vertices;

	// VertexBufferObject Ids
   GLuint vboIds[3];

} UserData;

#define VERTEX_POS_SIZE       3 // x, y and z
#define VERTEX_COLOR_SIZE     4 // r, g, b, and a

#define VERTEX_POS_INDX       0
#define VERTEX_COLOR_INDX     1

///
// Create a shader object, load the shader source, and
// compile the shader.
//

int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   char vShaderStr[] =
      "#version 300 es                          \n"
      "layout(location = 0) in vec4 vPosition;  \n"
	  "out vec4 tranglePos;                     \n"
      "void main()                              \n"
      "{                                        \n"  
	  "	  tranglePos = vPosition;"
	  "   gl_Position = vPosition;  \n"
      "}";

   char fShaderStr[] =
      "#version 300 es                              \n"
      "precision mediump float;                     \n"
      "out vec4 fragColor;				\n"
	  "in vec4 tranglePos;						\n"
      "void main()                                  \n"
	  "{												\n" 
	//  "	 fragColor = vec4 ( 0,abs(tranglePos.y), 0, 0.0f );"
	//  "	 fragColor = vec4 ( abs(tranglePos.x),abs(tranglePos.y), abs(tranglePos.z), 0.0f );"
	 "	if(tranglePos.x>-1.0&&tranglePos.x<-0.8||tranglePos.x>-0.6&&tranglePos.x<-0.4||tranglePos.x>-0.2&&tranglePos.x<0.0||tranglePos.x>0.2&&tranglePos.x<0.4||tranglePos.x>0.4&&tranglePos.x<0.6||tranglePos.x>0.8)                                    \n"
     "		 fragColor = vec4 ( 1.0f, 1.0f, 1.0f, 1.0f );  \n"
	  "	else            \n"
	  "      fragColor = vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );     "
      "}                                            \n";
    

   // Store the program object
   userData->programObject = esLoadProgram(vShaderStr,fShaderStr);
   userData->vboIds[0] = 0;

      // Get the uniform locations
   userData->mvpLoc = glGetUniformLocation ( userData->programObject, "u_mvpMatrix" );
   // Starting rotation angle for the cube
   userData->angle = 45.0f;

   glClearColor ( 0.0f, 0.5f, 0.5f, 1.0f );
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//                                                                                                 
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLfloat vVertices[] = {  0.0,  0.5f, 0.0f,
                            -0.5f, 0.0f, 0.0f,
                            0.5f, 0.0f, 0.0f,

                            -0.5f, 0.0f, 0.0f,
                            0.5f, 0.0f, 0.0f,
                            0.0f, -0.5f, 0.0f,
                         };

    // Set the viewport
   glViewport (0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   // Use the program object
   glUseProgram ( userData->programObject );

   glEnableVertexAttribArray (0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
   glDrawArrays ( GL_TRIANGLES, 0, 6);
}

void Shutdown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   glDeleteProgram ( userData->programObject );
}

int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Hello Triangle", 480, 480, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterDrawFunc ( esContext, Draw );
   // esRegisterUpdateFunc ( esContext, Update );
   return GL_TRUE;
}
