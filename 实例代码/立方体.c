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


   ESMatrix  mvpMatrix;
      // Uniform locations
   GLint  mvpLoc;
      // Rotation angle
   GLfloat   angle;

} UserData;


int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

      char vShaderStr[] =
      "#version 300 es                          \n"
      "layout(location = 0) in vec4 vPosition;  \n"
	  "layout(location = 1) in vec4 vColor;  \n"
	  "uniform mat4 u_mvpMatrix;                   \n"
	  "out vec4 outColor;"
      "void main()                              \n"
      "{                                        \n"
       "   gl_Position =  u_mvpMatrix * vPosition;              \n"
	   "    outColor=vColor;"
      "}                                        \n";

   char fShaderStr[] =
      "#version 300 es                              \n"
      "precision mediump float;                     \n"
	  "in vec4 outColor; \n"
      "out vec4 fragColor;                          \n"	 
      "void main()                                  \n"
      "{                                            \n"
	  "      fragColor = outColor;  \n"
      "}                                            \n";

   userData->programObject = esLoadProgram(vShaderStr,fShaderStr);

      userData->mvpLoc = glGetUniformLocation ( userData->programObject, "u_mvpMatrix" );

   // Starting rotation angle for the cube
   userData->angle = 45.0f;

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init() 
//

float fX=0.0f;
float fY=0.0f;
float r=0.5f;

float angle=0.0f;

float rCorlor=0.0f;

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
 //  esPerspective ( &perspective, 60.0f, aspect, 1.0f, 20.0f );
   // Generate a model view matrix to rotate/translate the cube
   esMatrixLoadIdentity ( &modelview );

   // Translate away from the viewer
  // esTranslate ( &modelview, 0.0, 0.0, -2.0 );

   // Rotate the cube
   esRotate ( &modelview, userData->angle, 1.0, 1.0, 0.0 );

   // Compute the final MVP by multiplying the
   // modevleiw and perspective matrices together
   esMatrixMultiply ( &userData->mvpMatrix, &modelview, &perspective );
}

//float r=0.5;
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLfloat vVertices[] = {  0.0,  0.5, 0.0f,
                            -0.5f, 0.0f, 0.0f,

                            0.5f, 0.0f, 0.0f,
							// -0.5f, -0.0f, 0.0f

                         };

   int i=0;
   //使用24个顶点绘制立方体
   GLfloat cubeVerts[] =
   {
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f, -0.5f,

      -0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,

      -0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,

      -0.5f, -0.5f, 0.5f,
      -0.5f,  0.5f, 0.5f,
      0.5f,  0.5f, 0.5f,
      0.5f, -0.5f, 0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
   };


   

// 使用8个顶点绘制立方体
  GLfloat cubeVerts2[] =
   {
       r,r,r,
	   -r,r,r,
	   -r,-r,r,
	   r,-r,r,

       r,-r,-r,
	   r,r,-r,
	   -r,r,-r,
	   -r,-r,-r
  };

 //索引数组
  GLubyte indices[36]={ 

	  0,1,2,0,2,3,

	  0,3,4,0,4,5,

	  0,5,6,0,6,1,
	  7,1,6,7,2,1,
	  7,5,4,7,6,5,
	  7,3,2,7,4,3
  };




   GLfloat vColorArray[] = {  fX,  fY, 0.0f,//第一个顶点的颜色
                            1.0f,1.0f, 0.0f,//第二个顶点的颜色

                            0.5f, 0.0f, 0.9f,//第三个顶点的颜色
				
                         };

   int nLoc=0;

    fX=r*cos(angle);
	fY=r*sin(angle);

	angle+=0.1f;

	rCorlor+=0.03f;

	//if(fX>1.0f)
	//	fX=-1.0f;

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );



/*
   //第一种方式，使用24个顶点绘制
   {
// Load the vertex data
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, cubeVerts);
   glEnableVertexAttribArray ( 0 );

   
   glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, vColorArray );
   glEnableVertexAttribArray ( 1);


   glUniformMatrix4fv ( userData->mvpLoc, 1, GL_FALSE, ( GLfloat * ) &userData->mvpMatrix.m[0][0] );

      for(i=0;i<6;i++)
	     glDrawArrays (GL_TRIANGLE_FAN, i*4, 4);


   }
*/

  //第二种方式
   {

    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, cubeVerts2);
    glEnableVertexAttribArray ( 0 );

   
    glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, vColorArray );
    glEnableVertexAttribArray ( 1);
 

    glUniformMatrix4fv ( userData->mvpLoc, 1, GL_FALSE, ( GLfloat * ) &userData->mvpMatrix.m[0][0] );

    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_BYTE,indices);

   }






  


}

void Shutdown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   glDeleteProgram ( userData->programObject );
}

int esMain ( ESContext *esContext )
{   
	
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterDrawFunc ( esContext, Draw );
   esRegisterUpdateFunc ( esContext, Update );

   return GL_TRUE;
}
