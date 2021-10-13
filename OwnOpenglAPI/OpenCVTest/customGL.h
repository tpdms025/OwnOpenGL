#pragma once

/* Boolean */
#define GL_TRUE                           1
#define GL_FALSE                          0

/* DataType */
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_DOUBLE                         0x140A
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034

/* MatrixMode */
#define GL_MODELVIEW                      0x1700
#define GL_PROJECTION                     0x1701

/* GetTarget */
#define GL_MODELVIEW_MATRIX               0x0BA6
#define GL_PROJECTION_MATRIX              0x0BA7

/* vertex_array */
#define GL_VERTEX_ARRAY                   0x8074
#define GL_NORMAL_ARRAY                   0x8075
#define GL_COLOR_ARRAY                    0x8076
#define GL_INDEX_ARRAY                    0x8077
#define GL_TEXTURE_COORD_ARRAY            0x8078

/* BeginMode */
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_TRIANGLES                      0x0004
#define GL_POLYGON                        0x0009

/* GetTarget */
#define GL_TEXTURE_2D                     0x0DE1

/* TextureParameterName */
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803

/* TextureMagFilter */
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601

/* TextureWrapMode */
#define GL_CLAMP                          0x2900
#define GL_REPEAT                         0x2901

/* PixelFormat */
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A

#include <stack>
#include <stdlib.h> 
#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>

typedef unsigned char       BYTE;

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

using namespace std;
using namespace cv;


struct Vec4
{
	float x, y,z,w;
	Vec4() {}
	Vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) : x(x), y(y),z(z),w(w) {}

	void Set(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_)
	{ 
		x = x_; y = y_; z = z_; w = w_;
	}
};


/*
matrix m = {
m1 m5 m9  m13
m2 m6 m10 m14
m3 m7 m11 m15
m4 m8 m12 m16
}
*/
struct Matrix
{	
	GLfloat *arr;
	Vec4 col1, col2, col3, col4;
	Matrix() { }
	Matrix(const float *_arr) 
	{ 
		col1 = { _arr[0], _arr[1], _arr[2], _arr[3] };
		col2 = { _arr[4], _arr[5], _arr[6], _arr[7] };
		col3 = { _arr[8], _arr[9], _arr[10], _arr[11] };
		col4 = { _arr[12], _arr[13], _arr[14], _arr[15] };
		getArray();
	}
	Matrix(float a1b1, float a2b1, float a3b1, float a4b1, float a1b2, float a2b2, float a3b2, float a4b2,
		float a1b3, float a2b3, float a3b3, float a4b3, float a1b4, float a2b4, float a3b4, float a4b4 )
	{
		col1 = { a1b1, a1b2, a1b3, a1b4 };
		col2 = { a2b1, a2b2, a2b3, a2b4 };
		col3 = { a3b1, a3b2, a3b3, a3b4 };
		col4 = { a4b1, a4b2, a4b3, a4b4 };
		getArray();
	}
	Matrix(const Vec4& _col1, const Vec4& _col2, const Vec4& _col3, const Vec4& _col4) : col1(_col1), col2(_col2), col3(_col3), col4(_col4) { getArray(); }

	Matrix Transpose() const
	{
		return Matrix(Vec4(col1.x, col2.x, col3.x, col4.x), 
			Vec4(col1.y, col2.y, col3.y, col4.y),
			Vec4(col1.z, col2.z, col3.z, col4.z), 
			Vec4(col1.w, col2.w, col3.w, col4.w));
	}

	void getArray()
	{
		float temp[16] = { col1.x,col2.x,col3.x,col4.x, col1.y,col2.y,col3.y,col4.y,col1.z,col2.z,col3.z,col4.z,col1.w,col2.w,col3.w,col4.w };
		arr = temp;
	}


	
};

inline float operator * (const Vec4& v1, const Vec4& v2)
{
	return float(v1.x*v2.x + v1.y*v2.y+ v1.z*v2.z+ v1.w*v2.w);
}
inline Vec4 operator * (const Matrix& A, const Vec4& v)
{
	Matrix temp = A.Transpose();
	return Vec4(temp.col1 * v, temp.col2*v, temp.col3*v, temp.col4*v);
}
inline Vec4 operator * (const Vec4& v, const Matrix& A)
{
	Matrix temp = A.Transpose();
	return Vec4(temp.col1*v, temp.col2*v, temp.col3*v, temp.col4*v);
}

inline Matrix operator * (const Matrix& c, const Matrix& m)
{
	Matrix temp = c.Transpose();
	return Matrix(Vec4(temp.col1 * m.col1, temp.col2 * m.col1, temp.col3 * m.col1, temp.col4*m.col1),
		Vec4(temp.col1*m.col2, temp.col2*m.col2, temp.col3*m.col2, temp.col4*m.col2),
		Vec4(temp.col1*m.col3, temp.col2*m.col3, temp.col3*m.col3, temp.col4*m.col3),
		Vec4(temp.col1*m.col4, temp.col2*m.col4, temp.col3*m.col4, temp.col4*m.col4));
}



int getByte(GLenum _type)
{
	int typeByte = 0;
	switch (_type)
	{
	case GL_DOUBLE:
		typeByte = 8;
		break;
	case GL_FLOAT: case GL_INT:case GL_UNSIGNED_INT:
		typeByte = 4;
		break;
	case GL_SHORT: case GL_UNSIGNED_SHORT:
		typeByte = 2;
		break;
	case GL_UNSIGNED_BYTE:
		typeByte = 1;
		break;
	default:
		break;
	}
	return typeByte;
}
int getBit(GLenum _type)
{
	return getByte(_type) * 8;
}
















struct VertexArray
{
	GLint size;
	GLenum type;
	GLsizei stride;
	const void *pointer;

	VertexArray(GLint _size, GLenum _type, GLsizei _stride, const void *_pointer)
	{
		size = _size;
		type = _type;
		stride = _stride;
		pointer = _pointer;
	}

	void setPoint(GLint _size, GLenum _type, GLsizei _stride, const void *_pointer)
	{
		size = _size;
		type = _type;
		stride = _stride;
		pointer = _pointer;

		stride += size * getByte(type);
	}

};


/* Matrix */
Matrix indentityMatrix = { 1,0,0,0,
				 	 	   0,1,0,0,
				 	 	   0,0,1,0,
						   0,0,0,1 }; 

Matrix viewMatrix = {  };
Matrix modMatrix = indentityMatrix;
Matrix proMatrix = indentityMatrix;
Matrix *currentMatrix= &modMatrix;

stack<Matrix> pro_stack;
stack<Matrix> mod_stack;
stack<Matrix>* cur_stack = &mod_stack;





VertexArray vertexPoints = { 3,GL_FLOAT,0 ,0};
VertexArray texCoordPoints = { 2,GL_FLOAT,0 ,0 };
VertexArray colorPoints = { 3,GL_FLOAT,0 ,0 };
VertexArray normalPoints = { 3,GL_FLOAT,0 ,0 };
std::vector<int> vertexIndices;


bool isVertexArray = false;
bool isTexCoordArray = false;
bool isColorArray = false;
bool isNormalArray = false;

GLfloat width=1280.f;
GLfloat height=720.f;
Mat dstimage(height, width, CV_8UC3, Scalar(0,0,0));

Scalar mixColor(Scalar a, Scalar b, float value);


GLuint LoadTexture(const char* filename);

GLuint *textureArray;
GLuint *cur_texture;
Scalar *texColor;

struct a
{
	BYTE *data;
};
a *curTex;


/*   functions   */
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
void glMatrixMode(GLenum mode);
void glPushMatrix();
void glPopMatrix();
void glOrthof(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
void glFrustumf(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
void gluPerspective(GLdouble fovy, GLdouble aspect,GLdouble near, GLdouble far);

void glLoadIndentity();
void glLoadMatrixf(const GLfloat *m);
void glMultMatrixf(const GLfloat *m);
void glTranslatef(GLfloat x, GLfloat y, GLfloat z);
void glScalef(GLfloat x, GLfloat y, GLfloat z);
void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void glGetFloatv(GLenum name, GLfloat *params);

void glEnableClientState(GLenum array);
void glDisableClientState(GLenum array);

void glVertexPointer(GLint size, GLenum type, GLsizei stride, const void *pointer);
void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const void *pointer);
void glColorPointer(GLint size, GLenum type, GLsizei stride, const void *pointer);
void glNormalPointer(GLenum type, GLsizei stride, const void *pointer);
void glDrawArrays(GLenum mode, GLint first, GLsizei count);

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void * indices);

void glGenTextures(GLsizei n, GLuint *textures);
void glBindTexture(GLenum target, GLuint texture);
void glDeleteTextures(GLsizei n, const GLuint *textures);

void glTextParameteri(GLenum target, GLenum pname, GLint param);
void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);






/*	opencv	*/
void line2(Mat& img, const Point& start, const Point& end,const Scalar& c1, const Scalar& c2)
{
	LineIterator iter(img, start, end, LINE_8);

	for (int i = 0; i < iter.count; i++, iter++) {
		double alpha = double(i) / iter.count;
		// note: using img.at<T>(iter.pos()) is faster, but 
		// then you have to deal with mat type and channel number yourself
		img(Rect(iter.pos(), Size(1, 1))) = c1 * (1.0 - alpha) + c2 * alpha;
	}
}
