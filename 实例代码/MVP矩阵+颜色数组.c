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

// 教学演示示例：MVP矩阵使用
//   作者微信：gudianxiaoshuo      QQ：869321632    
//       网站: www.gudianxiaoshuo.com    www.needdown.com
// 打造的软件：快乐课堂、任性动图、藏书笔记（代码助手）、古典小说大全朗读、音乐贺卡等
// 
// 本示例：
// 1）定义统一变量 MVP矩阵
// 2）使用MVP矩阵 设置视口范围 以及进行旋转平移等各种变换
//   
//   
typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // MVP matrix
   ESMatrix  mvpMatrix;

   // Uniform locations   保存统一变量MVP矩阵的位置
   GLint  mvpLoc;  

   // Rotation angle      旋转的角度
   GLfloat   angle;

} UserData;



int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   char vShaderStr[] =
      "#version 300 es                          \n"
      "layout(location = 0) in vec4 vPosition;  \n"
	  "layout(location = 1) in vec4 vColor;  \n"
	   "uniform mat4 u_mvpMatrix;                   \n"                 //定义统一变量 MVP矩阵 
	  "out vec4 outColor; \n"
      "void main()                              \n"
      "{                                        \n"
      "   gl_Position = u_mvpMatrix *vPosition;              \n"	    //使用MVP矩阵进行位置变换
	  "   outColor=vColor;   "
      "}                                        \n";

   char fShaderStr[] =
      "#version 300 es                              \n"
      "precision mediump float;                     \n"
      "in vec4 outColor;                          \n"
      "out vec4 fragColor;                          \n"
      "void main()                                  \n"
      "{                                            " 
      "   fragColor =outColor;  \n"
      "}                                            \n";




   userData->programObject = esLoadProgram(vShaderStr,fShaderStr);

   // Get the uniform locations              获得统一变量位置
   userData->mvpLoc = glGetUniformLocation ( userData->programObject, "u_mvpMatrix" );

   // Starting rotation angle for the cube   给角度赋初始值
   userData->angle = 45.0f;

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.3f );



   return TRUE;
}

// 进行MVP变换
void Update ( ESContext *esContext)
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
   esPerspective ( &perspective, 60.0f,aspect, 0.01f, 20.0f );//   60度视角  近平面距离眼睛0.01f,  远平面距离眼睛20.f
   // Generate a model view matrix to rotate/translate the cube
   esMatrixLoadIdentity ( &modelview );

   esTranslate ( &modelview, 0, 0.0,-1.0 );
   // Rotate the cube
   esRotate ( &modelview, userData->angle, 1, 1.0,0.0 );
   
   
   // Translate away from the viewer

   // Compute the final MVP by multiplying the
   // modevleiw and perspective matrices together
   esMatrixMultiply ( &userData->mvpMatrix, &modelview, &perspective );

      //   // Load the MVP matrix
   glUniformMatrix4fv ( userData->mvpLoc, 1, GL_FALSE, ( GLfloat * ) &userData->mvpMatrix.m[0][0] );

}


///
// Draw a triangle using the shader pair created in Init()
//

float fCor=0.1f;
BOOL bAdd=TRUE;


void Draw ( ESContext *esContext )
{

	

   UserData *userData = esContext->userData;

   GLfloat vVertices[] = {  0.0,  0.5f, 0.0f,
                            -0.5f, 0.0f, 0.0f,
                            0.5f, 0.0f, 0.0f,

                         }; 

      GLfloat vColorArray[] = {
		                    1.0,  fCor, 0,     //第0个顶点 红色
                            fCor, 0.0f, fCor, //第1个顶点 黑色
                            fCor, 0.0f, 1.0f,//第2个顶点 蓝色
		 };

	   

   if(bAdd)
	   fCor+=0.01f;
   else
	   fCor-=0.01f;

   if(fCor<0)
		bAdd=TRUE;
 
   if(fCor>1)
	   bAdd=FALSE;

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );
   glClear ( GL_DEPTH_BUFFER_BIT );

   //glEnable(GL_DEPTH_TEST);
   //glDepthFunc(GL_LEQUAL);
   //glClearDepthf(1.0f);

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex data
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
   glEnableVertexAttribArray ( 0 );

      //顶点属性与数组关联，每个顶点都可以有各自不同的颜色值
   glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, vColorArray );
   glEnableVertexAttribArray ( 1 );


   //使用顶点属性直接赋值   使用这种方法，每个顶点都是同一种颜色
   // glVertexAttrib4f(1,1.0,1.0,0.0,0.0);
   Update ( esContext);
   glDrawArrays ( GL_TRIANGLES, 0, 3 );
}

void Shutdown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   glDeleteProgram ( userData->programObject );
}

int esMain( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Hello Triangle", 620, 540, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}
