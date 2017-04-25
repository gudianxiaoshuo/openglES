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

// ��ѧ��ʾʾ����MVP����ʹ��
//   ����΢�ţ�gudianxiaoshuo      QQ��869321632    
//       ��վ: www.gudianxiaoshuo.com    www.needdown.com
// �������������ֿ��á����Զ�ͼ������ʼǣ��������֣����ŵ�С˵��ȫ�ʶ������ֺؿ���
// 
// ��ʾ����
// 1������ͳһ���� MVP����
// 2��ʹ��MVP���� �����ӿڷ�Χ �Լ�������תƽ�Ƶȸ��ֱ任
//   
//   
typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // MVP matrix
   ESMatrix  mvpMatrix;

   // Uniform locations   ����ͳһ����MVP�����λ��
   GLint  mvpLoc;  

   // Rotation angle      ��ת�ĽǶ�
   GLfloat   angle;

} UserData;



int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   char vShaderStr[] =
      "#version 300 es                          \n"
      "layout(location = 0) in vec4 vPosition;  \n"
	  "layout(location = 1) in vec4 vColor;  \n"
	   "uniform mat4 u_mvpMatrix;                   \n"                 //����ͳһ���� MVP���� 
	  "out vec4 outColor; \n"
      "void main()                              \n"
      "{                                        \n"
      "   gl_Position = u_mvpMatrix *vPosition;              \n"	    //ʹ��MVP�������λ�ñ任
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

   // Get the uniform locations              ���ͳһ����λ��
   userData->mvpLoc = glGetUniformLocation ( userData->programObject, "u_mvpMatrix" );

   // Starting rotation angle for the cube   ���Ƕȸ���ʼֵ
   userData->angle = 45.0f;

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.3f );



   return TRUE;
}

// ����MVP�任
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
   esPerspective ( &perspective, 60.0f,aspect, 0.01f, 20.0f );//   60���ӽ�  ��ƽ������۾�0.01f,  Զƽ������۾�20.f
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
		                    1.0,  fCor, 0,     //��0������ ��ɫ
                            fCor, 0.0f, fCor, //��1������ ��ɫ
                            fCor, 0.0f, 1.0f,//��2������ ��ɫ
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

      //�������������������ÿ�����㶼�����и��Բ�ͬ����ɫֵ
   glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, vColorArray );
   glEnableVertexAttribArray ( 1 );


   //ʹ�ö�������ֱ�Ӹ�ֵ   ʹ�����ַ�����ÿ�����㶼��ͬһ����ɫ
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
