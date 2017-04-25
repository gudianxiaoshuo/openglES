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
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;

   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   {
      return 0;
   }

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );

   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled )
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
         char *infoLog = malloc ( sizeof ( char ) * infoLen );

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );

         free ( infoLog );
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;

}

///
// Initialize the shader and program object
//


//void DrawPrimitiveWithVBOs ( ESContext *esContext,
//                             GLint numVertices, GLfloat **vtxBuf,
//                             GLint *vtxStrides, GLint numIndices,
//                             GLushort *indices )
//{
//   UserData *userData = esContext->userData;
//
//   // vboIds[0] - used to store vertex position
//   // vboIds[1] - used to store vertex color
//   // vboIds[2] - used to store element indices
//   if ( userData->vboIds[0] == 0 && userData->vboIds[1] == 0 &&
//         userData->vboIds[2] == 0 )
//   {
//      // Only allocate on the first draw
//      glGenBuffers ( 3, userData->vboIds );
//
//      glBindBuffer ( GL_ARRAY_BUFFER, userData->vboIds[0] );
//      glBufferData ( GL_ARRAY_BUFFER, vtxStrides[0] * numVertices,
//                     vtxBuf[0], GL_STATIC_DRAW );
//      glBindBuffer ( GL_ARRAY_BUFFER, userData->vboIds[1] );
//      glBufferData ( GL_ARRAY_BUFFER, vtxStrides[1] * numVertices,
//                     vtxBuf[1], GL_STATIC_DRAW );
//      glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[2] );
//      glBufferData ( GL_ELEMENT_ARRAY_BUFFER,
//                     sizeof ( GLushort ) * numIndices,
//                     indices, GL_STATIC_DRAW );
//   }
//
//
//   glBindBuffer ( GL_ARRAY_BUFFER, userData->vboIds[0] );
//   glEnableVertexAttribArray ( VERTEX_POS_INDX );
//   glVertexAttribPointer ( VERTEX_POS_INDX, VERTEX_POS_SIZE,
//                           GL_FLOAT, GL_FALSE, vtxStrides[0], 0 );
//
//   glBindBuffer ( GL_ARRAY_BUFFER, userData->vboIds[1] );
//   glEnableVertexAttribArray ( VERTEX_COLOR_INDX );
//   glVertexAttribPointer ( VERTEX_COLOR_INDX,
//                           VERTEX_COLOR_SIZE,
//                           GL_FLOAT, GL_FALSE, vtxStrides[1], 0 );
//
//   glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[2] );
//
//   glDrawElements ( GL_TRIANGLES, numIndices,
//                    GL_UNSIGNED_SHORT, 0 );
//
//   glDisableVertexAttribArray ( VERTEX_POS_INDX );
//   glDisableVertexAttribArray ( VERTEX_COLOR_INDX );
//
//   glBindBuffer ( GL_ARRAY_BUFFER, 0 );
//   glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
//}

void DrawPrimitiveWithVBOs ( ESContext *esContext,int nStridesPerVertex,int numVertices,void * pData)
{
   UserData *userData = esContext->userData;

   // vboIds[0] - used to store vertex position
   // vboIds[1] - used to store vertex color
   // vboIds[2] - used to store element indices
   if ( userData->vboIds[0] == 0 )
   {
      // Only allocate on the first draw
      glGenBuffers ( 1, userData->vboIds );

      glBindBuffer ( GL_ARRAY_BUFFER, userData->vboIds[0] );
      glBufferData ( GL_ARRAY_BUFFER, nStridesPerVertex * numVertices,
                    pData, GL_STATIC_DRAW );
 
   }


   glBindBuffer ( GL_ARRAY_BUFFER, userData->vboIds[0] );
   glEnableVertexAttribArray ( VERTEX_POS_INDX );
   glVertexAttribPointer ( VERTEX_POS_INDX, VERTEX_POS_SIZE,
                           GL_FLOAT, GL_FALSE, nStridesPerVertex, 0 );


   glDrawArrays ( GL_TRIANGLES, 0,numVertices);
   glDisableVertexAttribArray ( VERTEX_POS_INDX );



   glBindBuffer ( GL_ARRAY_BUFFER, 0 );

}


int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   char vShaderStr[] =
      "#version 300 es                          \n"
      "layout(location = 0) in vec4 vPosition;  \n"
	  "layout(location =1) in vec4 vCorlor; \n"
	  "uniform mat4 u_mvpMatrix;                   \n"
	  "out vec4 outCorlor;                     \n"
	  "out vec4 tranglePos;                     \n"
      "void main()                              \n"
      "{                                        \n"
	//  "   gl_Position = u_mvpMatrix * vPosition;  \n"
	  
	  "	  tranglePos = vPosition;"
	   "   gl_Position = vPosition;  \n"
	   "   outCorlor = vCorlor;  \n"
      "}\n";




   char fShaderStr[] =
      "#version 300 es                              \n"
      "precision mediump float;                     \n"
      "out vec4 fragColor;				\n"
	  "in vec4 tranglePos;						\n"
	   "in vec4 outCorlor;                     \n"
	  "uniform float rCorlor;     \n"
      "void main()                                  \n"
	  "{												\n" 
	   "	 fragColor = outCorlor; "
	//  "	 fragColor = vec4 ( 0,abs(tranglePos.y), 0, 0.0f );"
	 // "	 fragColor = vec4 ( rCorlor,abs(tranglePos.y), abs(tranglePos.z), 0.0f );"
	//  "	 fragColor = vec4 ( abs(tranglePos.x),abs(tranglePos.y), abs(tranglePos.z), 0.0f );"
	// "	if(tranglePos.x>-1.0&&tranglePos.x<-0.8||tranglePos.x>-0.6&&tranglePos.x<-0.4||tranglePos.x>-0.2&&tranglePos.x<0.0||tranglePos.x>0.0&&tranglePos.x<0.2||tranglePos.x>0.4&&tranglePos.x<0.6||tranglePos.x>0.8)                                    \n"
   //  "		 fragColor = vec4 ( 1.0f, 1.0f, 1.0f, 1.0f );  \n"
	//  "	else            \n"
	//  "      fragColor = vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );     "
      "}                                            \n";
    
   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   // Load the vertex/fragment shaders
   vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
   fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

   // Create the program object
   programObject = glCreateProgram ( );

   if ( programObject == 0 )
   {
      return 0;
   }

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked )
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

      if ( infoLen > 1 )
      {
         char *infoLog = malloc ( sizeof ( char ) * infoLen );

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         esLogMessage ( "Error linking program:\n%s\n", infoLog );

         free ( infoLog );
      }

      glDeleteProgram ( programObject );
      return FALSE;
   }

   // Store the program object
   userData->programObject = programObject;
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

void Update ( ESContext *esContext, float deltaTime )
{
   UserData *userData = esContext->userData;
   ESMatrix perspective;
   ESMatrix modelview;
   float    aspect;

   // Compute a rotation angle based on time to rotate the cube
   userData->angle +=1.0f; //( deltaTime * 40.0f );

   if ( userData->angle >= 360.0f )
   {
      userData->angle -= 360.0f;
   }

   // Compute the window aspect ratio
   aspect = ( GLfloat ) esContext->width / ( GLfloat ) esContext->height;

   // Generate a perspective matrix with a 60 degree FOV
   esMatrixLoadIdentity ( &perspective );
  // esPerspective ( &perspective, 1.0f, aspect, 0.1f, 20.0f );
   esPerspective ( &perspective, 60.0f,aspect, 0.01f, 20.0f );//   60度视角  近平面距离眼睛1.0f,  远平面距离眼睛20.f
   // Generate a model view matrix to rotate/translate the cube
   esMatrixLoadIdentity ( &modelview );

   esTranslate ( &modelview, 0, 0.0,-2.0 );
   // Rotate the cube
   esRotate ( &modelview, userData->angle, 1, 1.0,0.0 );
   
   
   // Translate away from the viewer

   // Compute the final MVP by multiplying the
   // modevleiw and perspective matrices together
   esMatrixMultiply ( &userData->mvpMatrix, &modelview, &perspective );

      //   // Load the MVP matrix
   glUniformMatrix4fv ( userData->mvpLoc, 1, GL_FALSE, ( GLfloat * ) &userData->mvpMatrix.m[0][0] );

}


float rCor=0.0f;
BOOL bAdd=TRUE;

                                                                                                     
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLfloat vVertices[] = {  0.0,  0.5f, 0.0f,
                            -0.5f, 0.0f, 0.0f,
                            0.5f, 0.0f, 0.0f,

							 0.0,  -0.5f, 0.0f,
                            -0.5f, 0.5f, 0.0f,
                            0.5f, 0.0f, 0.0f,
                         };

      GLfloat vCor[] = {  0.0,  0.5f, 0.0f,
                            -0.5f, 0.0f, 0.0f,
                            0.5f, 0.0f, 0.0f,

							 0.0,  -0.5f, 0.0f,
                            -0.5f, 0.0f, 0.0f,
                            0.5f, 0.0f, 0.0f,
                         };
      int nLocation=0;

     GLint vtxStrides[2] =
   {
      VERTEX_POS_SIZE * sizeof ( GLfloat ),
      VERTEX_COLOR_SIZE * sizeof ( GLfloat )
   };

	  if(bAdd)
		 rCor+=0.001f;
	  else
		 rCor-=0.001f;

   if(rCor>1.0)
	   bAdd=FALSE;

   if(rCor<0.0)
	   bAdd=TRUE;





 // Set the viewport
   glViewport (0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   // Use the program object
   glUseProgram ( userData->programObject );

  

 //  DrawPrimitiveWithVBOs ( esContext,VERTEX_POS_SIZE * sizeof ( GLfloat ), 6, vVertices);
      // Set the vertex color to red
 //  glVertexAttrib4f ( 1, 0, 0, 0.0f, 1.0f );
   // Load the vertex data
   glEnableVertexAttribArray (0 );
  // glVertexAttrib1f(1,1);
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );


   glEnableVertexAttribArray (1);
  // glVertexAttrib1f(1,1);
   glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, vCor );
 
 //  glDrawArrays ( GL_LINE_LOOP, 0, 3);

 //  nLocation=glGetUniformLocation(userData->programObject,"rCorlor");
 //  glUniform1f(nLocation,rCor);

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
