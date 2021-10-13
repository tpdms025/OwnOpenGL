#include "customGL.h"
#include "Planet.h"


Planet Earth(1.0f, 18, 72);
int texture_width = 2048;
int texture_height = 1024;

void Init()
{
	//Earth.texture = LoadTexture("Earth.bmp");
}
int main()
{
	Init();

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIndentity();
	gluPerspective(40.0f, (GLdouble)width / (GLdouble)height, 0.0f, 40.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIndentity();
	glTranslatef(0.0, 0.0, -60.0f);
	glRotatef(-70.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(70.0f, 0.0f, 0.0f, 1.0f);


	
	Earth.DrawSphere();

	imshow("customGL", dstimage);
	
	waitKey();

	return 0;
}


GLuint LoadTexture(const char* filename)
{
	GLuint texture = 0;
	int width, height;
	BYTE *data = NULL;
	FILE *file;

	// open texture data  
	fopen_s(&file, filename, "rb");

	if (&file == NULL) return 0;

	// allocate buffer  
	width = 2048;
	height = 1024;

	data = (BYTE*)malloc(width * height * 3);

	// read texture data  
	fread(data, width * height * 3, 1, file);
	fclose(file);

	//bmp파일 BGR로 되어있습니다. RGB로 바꿉니다. 
	unsigned char tmp;
	for (int i = 0; i < width * height * 3; i += 3)
	{
		tmp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = tmp;
	}

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	/*
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // 프래그먼트가 텍스처가 될 때 텍스처 값이 해석되는 방식을 지정합니다.

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 텍스처 축소 기능 설정, 텍스처 처리되는 픽셀의 크기와 가장 일치하는 밉맵을 선택하고 GL_NEAREST 기준(픽셀 중심에 가장 가까운 텍스처 요소)을 사용하여 텍스처 값을 생성합니다.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 텍스처 확대 기능 설정, 텍스처 처리되는 픽셀의 중심에 가장 가까운 4 개의 텍스처 요소에 대한 가중 평균을 반환합니다.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 텍스처 좌표 s의 wrap 매개변수 설정, GL_REPEAT는 s 좌표의 정수 부분이 무시되도록합니다.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 텍스처 좌표 t의 wrap 매개변수 설정, GL_REPEAT는 s 좌표의 정수 부분이 무시되도록합니다.

	// copy texture data - 텍스처 이미지 지정
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	*/
	glBindTexture(GL_TEXTURE_2D, 0);
	free(data);

	return texture;
}



/*******************************************************************************************************************************/
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	viewMatrix = { width*0.5f ,0,0,width*0.5f,
				0,height*0.5f,0,height*0.5f,
				0,0,0,1,
				0,0,0,1 };
}


void glGetFloatv(GLenum name, GLfloat* params)
{
	Matrix m;
	switch (name)
	{
	case GL_PROJECTION_MATRIX:
		m = proMatrix;
		break;
	case GL_MODELVIEW_MATRIX:
		m = modMatrix;
		break;

	default:
		break;
	}
	/*float temp[16] = { m.col1.x,m.col2.x,m.col3.x,m.col4.x, m.col1.y,m.col2.y,m.col3.y,m.col4.y,
		m.col1.z,m.col2.z,m.col3.z,m.col4.z,m.col1.w,m.col2.w,m.col3.w,m.col4.w };
	params = temp;*/
	params = m.arr;
}


void glPushMatrix()
{
	cur_stack->push(*currentMatrix);
}
void glPopMatrix()
{
	*currentMatrix = cur_stack->top();
	cur_stack->pop();
}
void glOrthof(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)
{
	Matrix prj = { 2.0f / (r - l),0,0,-((r + l) / (r - l)),
				 0,2.0f / (t - b),0,-((t + b) / (t - b)),
				 0,0,-2 / (f - n),-((f + n) / (f - n)),
				 0,0,0,1 };
	*currentMatrix = ((*currentMatrix) * prj);

}
void glFrustumf(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)
{
	Matrix prj = { 2.0f * n / (r - l),0,(r + l) / (r - l),0,
				  0,2.0f*n / (t - b),(t + b) / (t - b),0,
				  0,0,-((f + n) / (f - n)),-((2.0f*f*n) / (f - n)),
				  0,0,-1,0 };

	*currentMatrix = ((*currentMatrix) * prj);
	//currentMatrix = currentMatrix * prj;
}
void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble near, GLdouble far)
{
	Matrix prj = { (float)fovy / (float)aspect ,0,0,0,
					0,(float)far,0,0,
					0,0,(float)(far + near) / (float)(near - far),(float)(2.0f*far*near) / (float)(near - far),
					0,0,-1,0 };

	*currentMatrix = ((*currentMatrix) * prj);

}


void glLoadIndentity()
{
	memcpy(currentMatrix, &indentityMatrix, sizeof(indentityMatrix));
}

void glLoadMatrixf(const GLfloat *m)
{
	Matrix temp(m);

	memcpy(currentMatrix, &temp, sizeof(temp));

}
void glMultMatrixf(const GLfloat *m)
{
	Matrix temp(m);
	*currentMatrix = ((*currentMatrix) * temp);
}
void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix t = { 1,0,0,x,
				 0,1,0,y,
				 0,0,1,z,
				 0,0,0,1 };
	*currentMatrix = ((*currentMatrix) * t);
}
void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix s = { x,0,0,0,
				 0,y,0,0,
				 0,0,z,0,
				 0,0,0,1 };
	*currentMatrix = ((*currentMatrix) * s);
}
void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	angle *= (PI / 180);
	float c = cos(angle);
	float s = sin(angle);


	Matrix r = { x*x*(1 - c) + c, x*y*(1 - c)-z*s,z*x*(1 - c) + (y * s),0,
				y*x*(1 - c) + (z * s),y*y*(1 - c) + c,y*z*(1 - c) - x * s,0,
				x*z*(1 - c) - y * s,y*z*(1 - c) + (x * s),z*z*(1 - c) + c,0,
				0,0,0,1 };
	*currentMatrix = ((*currentMatrix) * r);
}

//각 모드에 맞게 CurrentMatrix 교체
void glMatrixMode(GLenum mode)
{
	switch (mode)
	{
	case GL_PROJECTION:

		currentMatrix = &proMatrix;
		cur_stack = &pro_stack;
		break;
	case GL_MODELVIEW:
		currentMatrix = &modMatrix;
		cur_stack = &mod_stack;
		break;

	default:
		break;
	}
}


void glEnableClientState(GLenum array)
{
	switch (array)
	{
	case GL_VERTEX_ARRAY:
		isVertexArray = true;
		break;
	case GL_TEXTURE_COORD_ARRAY:
		isTexCoordArray = true;
		break;
	case GL_COLOR_ARRAY:
		isColorArray = true;
		break;
	case GL_NORMAL_ARRAY:
		isNormalArray = true;
		break;
	default:
		break;
	}
}
void glDisableClientState(GLenum array)
{
	switch (array)
	{
	case GL_VERTEX_ARRAY:
		isVertexArray = false;
		break;
	case GL_TEXTURE_COORD_ARRAY:
		isTexCoordArray = false;
		break;
	case GL_COLOR_ARRAY:
		isColorArray = false;
		break;
	case GL_NORMAL_ARRAY:
		isNormalArray = false;
		break;
	default:
		break;
	}
}



void glVertexPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
{
	if (isVertexArray)
	{
		vertexPoints.setPoint(size, type, stride, pointer);
	}
}
void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
{
	if (isTexCoordArray)
	{
		texCoordPoints.setPoint(size, type, stride, pointer);
	}
}
void glColorPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
{
	//ex) size = 4 RGBA
	if (isColorArray)
	{
		colorPoints.setPoint(size, type, stride, pointer);
	}
}
void glNormalPointer(GLenum type, GLsizei stride, const void *pointer)
{
	if (isNormalArray)
	{
		normalPoints.setPoint(3, type, stride, pointer);
	}
}












void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	Scalar color1 = Scalar(255, 255, 255);	//기본 흰색
	Scalar color2 = Scalar(255, 255, 255);	
	int vertexSize = vertexPoints.size;				// 3 (x,y,z)
	int colorSize = colorPoints.size;				// 4 (r,g,b,a)

	Vec4 v1, v2;
	int *_indices = vertexIndices.data();


	if (mode == GL_POINTS)
	{
		if (isVertexArray)
		{
			for (int i = first; i < first + count; i++)
			{
				//vertexPoints.pointer - xyz xyz xyz ... 로 이루어진 배열이기에 3*i
				Vec4 p1(((float*)vertexPoints.pointer)[vertexSize * i], ((float*)vertexPoints.pointer)[vertexSize * i + 1], ((float*)vertexPoints.pointer)[vertexSize * i + 2], 1);
				Vec4 p2(((float*)vertexPoints.pointer)[vertexSize * i], ((float*)vertexPoints.pointer)[vertexSize * i + 1], ((float*)vertexPoints.pointer)[vertexSize * i + 2], 1);

				 v1 = p1 * modMatrix*proMatrix*viewMatrix;
				 v2 = p2 * modMatrix*proMatrix*viewMatrix;

				v1.x /= v1.w; v1.y /= v1.w; v1.z /= v1.w; v1.w /= v1.w;
				v2.x /= v2.w; v2.y /= v2.w; v2.z /= v2.w; v2.w /= v2.w;

				if (isColorArray)
				{
					color1 = Scalar(((float*)colorPoints.pointer)[colorSize * i], ((float*)colorPoints.pointer)[colorSize  * i + 1], ((float*)colorPoints.pointer)[colorSize * i + 2], ((float*)colorPoints.pointer)[colorSize * i + 3]);
					color1 *= 255;				//0~1의 값이기에 255를 곱해준다.

					line(dstimage, Point2f(v1.x, v1.y), Point2f(v2.x, v2.y), color1,2);

				}
				else
				{
					line(dstimage, Point2f(v1.x, v1.y), Point2f(v2.x, v2.y), Scalar(255, 255, 255),2);
				}
			}
		}
		
	}
	else if (mode == GL_LINES)
	{
		if (isVertexArray)
		{
			for (int i = first; i < (first + count)*0.5 - 1; i++)
			{
				Vec4 p1(((float*)vertexPoints.pointer)[vertexSize * 2 * i], ((float*)vertexPoints.pointer)[vertexSize * 2 * i + 1], ((float*)vertexPoints.pointer)[vertexSize * 2 * i + 2], 1);
				Vec4 p2(((float*)vertexPoints.pointer)[vertexSize * 2 * i + vertexSize], ((float*)vertexPoints.pointer)[vertexSize * 2 * i + 1 + vertexSize], ((float*)vertexPoints.pointer)[vertexSize * 2 * i + 2 + vertexSize], 1);


				 v1 = p1 * modMatrix*proMatrix;
				 v2 = p2 * modMatrix*proMatrix;

				v1.x /= v1.w; v1.y /= v1.w; v1.z /= v1.w; v1.w /= v1.w;
				v2.x /= v2.w; v2.y /= v2.w; v2.z /= v2.w; v2.w /= v2.w;

				v1 = v1 *viewMatrix;
				v2 = v2 *viewMatrix;

				if (isColorArray)
				{
					color1 = Scalar(((float*)colorPoints.pointer)[colorSize * 2 * i], ((float*)colorPoints.pointer)[colorSize * 2 * i + 1], ((float*)colorPoints.pointer)[colorSize * 2 * i + 2], ((float*)colorPoints.pointer)[colorSize * 2 * i + 3]);
					color2 = Scalar(((float*)colorPoints.pointer)[colorSize * 2 * i+ colorSize], ((float*)colorPoints.pointer)[colorSize * 2 * i + 1+ colorSize], ((float*)colorPoints.pointer)[colorSize * 2 * i + 2+ colorSize], ((float*)colorPoints.pointer)[colorSize * 2 * i + 3+ colorSize]);
					color1 *= 255;	color2 *= 255;			//0~1의 값이기에 255를 곱해준다.

					line2(dstimage, Point2f(v1.x, v1.y), Point2f(v2.x, v2.y), color1, color2);
				}
				else
				{
					line(dstimage, Point2f(v1.x, v1.y), Point2f(v2.x, v2.y), Scalar(255,255,255));
				}
				
			}
		}
		
	}
	else if (mode == GL_LINE_LOOP)
	{
		if (isVertexArray)
		{
			for (int i = first; i < (first + count) - 1; i++)
			{
				Vec4 p1(((float*)vertexPoints.pointer)[vertexSize  * i], ((float*)vertexPoints.pointer)[vertexSize  * i + 1], ((float*)vertexPoints.pointer)[vertexSize  * i + 2], 1);
				Vec4 p2(((float*)vertexPoints.pointer)[vertexSize  * i + vertexSize], ((float*)vertexPoints.pointer)[vertexSize  * i + 1 + vertexSize], ((float*)vertexPoints.pointer)[vertexSize  * i + 2 + vertexSize], 1);


				v1 = p1 * modMatrix*proMatrix;
				v2 = p2 * modMatrix*proMatrix;

				v1.x /= v1.w; v1.y /= v1.w; v1.z /= v1.w; v1.w /= v1.w;
				v2.x /= v2.w; v2.y /= v2.w; v2.z /= v2.w; v2.w /= v2.w;

				v1 = v1 * viewMatrix;
				v2 = v2 * viewMatrix;

				if (isColorArray)
				{
					color1 = Scalar(((float*)colorPoints.pointer)[colorSize * i], ((float*)colorPoints.pointer)[colorSize * i + 1], ((float*)colorPoints.pointer)[colorSize * i + 2], ((float*)colorPoints.pointer)[colorSize * i + 3]);
					color2 = Scalar(((float*)colorPoints.pointer)[colorSize  * i + colorSize], ((float*)colorPoints.pointer)[colorSize  * i + 1 + colorSize], ((float*)colorPoints.pointer)[colorSize * i + 2 + colorSize], ((float*)colorPoints.pointer)[colorSize * i + 3 + colorSize]);
					color1 *= 255;	color2 *= 255;		//0~1의 값이기에 255를 곱해준다.

					line2(dstimage, Point2f(v1.x, v1.y), Point2f(v2.x, v2.y), color1, color2);
				}
				else
				{
					line(dstimage, Point2f(v1.x, v1.y), Point2f(v2.x, v2.y), Scalar(255, 255, 255));
				}
			}
		}
		if (isTexCoordArray)
		{
		}
	}
	if (mode == GL_TRIANGLES)
	{
		if (isVertexArray)
		{
			Vec4 v3;
			for (int i = 0; i < vertexIndices.size(); i += 3)
			{

				Vec4 p1(((float*)vertexPoints.pointer)[vertexSize *_indices[i]], ((float*)vertexPoints.pointer)[vertexSize *_indices[i] + 1], ((float*)vertexPoints.pointer)[vertexSize * _indices[i] + 2], 1);
				Vec4 p2(((float*)vertexPoints.pointer)[vertexSize * _indices[i + 1]], ((float*)vertexPoints.pointer)[vertexSize * _indices[i + 1] + 1], ((float*)vertexPoints.pointer)[vertexSize *_indices[i + 1] + 2], 1);
				Vec4 p3(((float*)vertexPoints.pointer)[vertexSize * _indices[i + 2]], ((float*)vertexPoints.pointer)[vertexSize * _indices[i + 2] + 1], ((float*)vertexPoints.pointer)[vertexSize *_indices[i + 2] + 2], 1);


				v1 = p1 * modMatrix*proMatrix*viewMatrix;
				v2 = p2 * modMatrix*proMatrix*viewMatrix;
				v3 = p3 * modMatrix*proMatrix*viewMatrix;

				v1.x /= v1.w; v1.y /= v1.w; v1.z /= v1.w; v1.w /= v1.w;
				v2.x /= v2.w; v2.y /= v2.w; v2.z /= v2.w; v2.w /= v2.w;
				v3.x /= v3.w; v3.y /= v3.w; v3.z /= v3.w; v3.w /= v3.w;

				if (isColorArray)
				{
					color1 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i]], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 1], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 2], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 3]);
					color2 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i + 1]], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 1], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 2], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 3]);
					//color3 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i + 2]], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2]+1], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2]+2], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2] + 3]);
					color1 *= 255;
					color2 *= 255;
					//color3 *= 255;

					line2(dstimage, Point2f(v1.x, v1.y), Point2f(v2.x, v2.y), color1, color2);
					line2(dstimage, Point2f(v2.x, v2.y), Point2f(v3.x, v3.y), color1, color2);
					line2(dstimage, Point2f(v3.x, v3.y), Point2f(v1.x, v1.y), color1, color2);
				}
				else
				{
					line(dstimage, Point2f(v1.x, v1.y), Point2f(v2.x, v2.y), Scalar(255, 255, 255));
					line(dstimage, Point2f(v2.x, v2.y), Point2f(v3.x, v3.y), Scalar(255, 255, 255));
					line(dstimage, Point2f(v3.x, v3.y), Point2f(v1.x, v1.y), Scalar(255, 255, 255));
				}


			}
		}
	}
	else if (mode == GL_POLYGON)
	{
		if (isVertexArray)
		{
			Scalar color3 = Scalar(255, 255, 255);	//기본 흰색
			Vec4 v3;

			for (int i = 0; i < vertexIndices.size(); i += 3)
			{
				Vec4 p1(((float*)vertexPoints.pointer)[vertexSize *_indices[i]], ((float*)vertexPoints.pointer)[vertexSize *_indices[i] + 1], ((float*)vertexPoints.pointer)[vertexSize * _indices[i] + 2], 1);
				Vec4 p2(((float*)vertexPoints.pointer)[vertexSize * _indices[i + 1]], ((float*)vertexPoints.pointer)[vertexSize * _indices[i + 1] + 1], ((float*)vertexPoints.pointer)[vertexSize *_indices[i + 1] + 2], 1);
				Vec4 p3(((float*)vertexPoints.pointer)[vertexSize * _indices[i + 2]], ((float*)vertexPoints.pointer)[vertexSize * _indices[i + 2] + 1], ((float*)vertexPoints.pointer)[vertexSize *_indices[i + 2] + 2], 1);

				v1 = p1 * modMatrix*proMatrix*viewMatrix;
				v2 = p2 * modMatrix*proMatrix*viewMatrix;
				v3 = p3 * modMatrix*proMatrix*viewMatrix;

				v1.x /= v1.w;	 v1.y /= v1.w;	v1.z /= v1.w;	v1.w /= v1.w;
				v2.x /= v2.w;	 v2.y /= v2.w;	v2.z /= v2.w;	v2.w /= v2.w;
				v3.x /= v3.w;	 v3.y /= v3.w;	v3.z /= v3.w;	v3.w /= v3.w;

				Point rook_points[1][3];
				rook_points[0][0] = Point(v1.x, v1.y);
				rook_points[0][1] = Point(v2.x, v2.y);
				rook_points[0][2] = Point(v3.x, v3.y);
				const Point* ppt[1] = { rook_points[0] };
				int npt[] = { 3 };

				if (isColorArray)
				{
					color1 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i]], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 1], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 2], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 3]);
					color2 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i + 1]], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 1], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 2], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 3]);
					color3 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i + 2]], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2]+1], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2]+2], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2] + 3]);
					color1 *= 255;
					color2 *= 255;
					color3 *= 255;
					Scalar temp = mixColor(color1, color2, 0.5f);
					fillPoly(dstimage, ppt, npt, 1, mixColor(temp, color3, 0.5f));


				}
				else
				{
					fillPoly(dstimage, ppt, npt,1,Scalar(255,255,255));
				}


			}
		}
	}
}

Scalar mixColor(Scalar a, Scalar b, float value)
{
	return a * (1.0f - value) + (b * value);
}


void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void * indices)
{
	Scalar color1 = Scalar(255, 255, 255);	//기본 흰색
	Scalar color2 = Scalar(255, 255, 255);
	Scalar color3 = Scalar(255, 255, 255);	
	Vec4 v1, v2, v3;

	int vertexSize = vertexPoints.size;
	int colorSize = colorPoints.size;
	int *_indices = (int*)indices;



	if (mode == GL_TRIANGLES)
	{
		if (isVertexArray)
		{
			for (int i = 0; i < count; i += 3)
			{
				Vec4 p1(((float*)vertexPoints.pointer)[vertexSize *_indices[i]], ((float*)vertexPoints.pointer)[vertexSize *_indices[i] + 1], ((float*)vertexPoints.pointer)[vertexSize * _indices[i] + 2], 1);
				Vec4 p2(((float*)vertexPoints.pointer)[vertexSize * _indices[i + 1]], ((float*)vertexPoints.pointer)[vertexSize * _indices[i + 1] + 1], ((float*)vertexPoints.pointer)[vertexSize *_indices[i + 1] + 2], 1);
				Vec4 p3(((float*)vertexPoints.pointer)[vertexSize * _indices[i + 2]], ((float*)vertexPoints.pointer)[vertexSize * _indices[i + 2] + 1], ((float*)vertexPoints.pointer)[vertexSize *_indices[i + 2] + 2], 1);


				v1 = p1 * modMatrix*proMatrix*viewMatrix;
				v2 = p2 * modMatrix*proMatrix*viewMatrix;
				v3 = p3 * modMatrix*proMatrix*viewMatrix;

				v1.x /= v1.w; v1.y /= v1.w; v1.z /= v1.w; v1.w /= v1.w;
				v2.x /= v2.w; v2.y /= v2.w; v2.z /= v2.w; v2.w /= v2.w;
				v3.x /= v3.w; v3.y /= v3.w; v3.z /= v3.w; v3.w /= v3.w;

				if (isColorArray)
				{
					color1 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i]], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 1], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 2], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 3]);
					color2 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i + 1]], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 1], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 2], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 3]);
					color3 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i + 2]], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2]+1], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2]+2], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2] + 3]);
					color1 *= 255; color2 *= 255; color3 *= 255;		//0~1의 값이기에 255를 곱해준다.

					line2(dstimage, Point2f(v1.x, v1.y), Point2f(v2.x, v2.y), color1, color2);
					line2(dstimage, Point2f(v2.x, v2.y), Point2f(v3.x, v3.y), color2, color3);
					line2(dstimage, Point2f(v3.x, v3.y), Point2f(v1.x, v1.y), color3, color1);
				}
				else
				{
					line(dstimage, Point2f(v1.x, v1.y), Point2f(v2.x, v2.y), Scalar(255, 255, 255));
					line(dstimage, Point2f(v2.x, v2.y), Point2f(v3.x, v3.y), Scalar(255, 255, 255));
					line(dstimage, Point2f(v3.x, v3.y), Point2f(v1.x, v1.y), Scalar(255, 255, 255));
				}
			}
		}
	}
	else if (mode == GL_POLYGON)
	{
		if (isVertexArray)
		{
			for (int i = 0; i < count; i += 3)
			{
				Vec4 p1(((float*)vertexPoints.pointer)[vertexSize *_indices[i]], ((float*)vertexPoints.pointer)[vertexSize *_indices[i] + 1], ((float*)vertexPoints.pointer)[vertexSize * _indices[i] + 2], 1);
				Vec4 p2(((float*)vertexPoints.pointer)[vertexSize * _indices[i + 1]], ((float*)vertexPoints.pointer)[vertexSize * _indices[i + 1] + 1], ((float*)vertexPoints.pointer)[vertexSize *_indices[i + 1] + 2], 1);
				Vec4 p3(((float*)vertexPoints.pointer)[vertexSize * _indices[i + 2]], ((float*)vertexPoints.pointer)[vertexSize * _indices[i + 2] + 1], ((float*)vertexPoints.pointer)[vertexSize *_indices[i + 2] + 2], 1);

				v1 = p1 * modMatrix*proMatrix*viewMatrix;
				v2 = p2 * modMatrix*proMatrix*viewMatrix;
				v3 = p3 * modMatrix*proMatrix*viewMatrix;

				v1.x /= v1.w;	 v1.y /= v1.w;	v1.z /= v1.w;	v1.w /= v1.w;
				v2.x /= v2.w;	 v2.y /= v2.w;	v2.z /= v2.w;	v2.w /= v2.w;
				v3.x /= v3.w;	 v3.y /= v3.w;	v3.z /= v3.w;	v3.w /= v3.w;

				Point rook_points[1][3];
				rook_points[0][0] = Point(v1.x, v1.y);
				rook_points[0][1] = Point(v2.x, v2.y);
				rook_points[0][2] = Point(v3.x, v3.y);
				const Point* ppt[1] = { rook_points[0] };
				int npt[] = { 3 };

				if (isColorArray)
				{
					color1 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i]], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 1], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 2], ((float*)colorPoints.pointer)[colorSize *_indices[i] + 3]);
					color2 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i + 1]], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 1], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 2], ((float*)colorPoints.pointer)[colorSize *_indices[i + 1] + 3]);
					color3 = Scalar(((float*)colorPoints.pointer)[colorSize *_indices[i + 2]], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2] + 1], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2] + 2], ((float*)colorPoints.pointer)[colorSize *_indices[i + 2] + 3]);
					color1 *= 255; color2 *= 255; color3 *= 255;		//0~1의 값이기에 255를 곱해준다.

					Scalar temp = mixColor(color1, color2, 0.5f);
					fillPoly(dstimage, ppt, npt, 1, mixColor(temp, color3, 0.5f));
				}
				else
				{
					fillPoly(dstimage, ppt, npt, 1, Scalar(255, 255, 255));
				}

			}
		}
	}
}








void glGenTextures(GLsizei n, GLuint *textures)
{
	textureArray = new GLuint[n];
	textureArray = textures;
}

void glBindTexture(GLenum target, GLuint texture)
{
	if (target == GL_TEXTURE_2D)
	{
		for (int i = 0; i < sizeof(textureArray) / sizeof(GLuint); i++)
		{
			if (textureArray[i] == texture)
			{
				cur_texture = &textureArray[i];
			}
		}
	}
}

void glDeleteTextures(GLsizei n, const GLuint *textures)
{
	free((GLuint*)textures);
	free((GLuint*)textureArray);
}

void glTextParameteri(GLenum target, GLenum pname, GLint param)
{
	if (target == GL_TEXTURE_2D)
	{
		if (pname == GL_TEXTURE_MAG_FILTER)
		{
			if (param == GL_NEAREST)
			{

			}
			else if (param == GL_LINEAR)
			{
				int unit_size = 4 * sizeof(GLubyte);

				for (int i = 0; i < sizeof(texCoordPoints.pointer) / sizeof(float); i++)
				{
					float img_u = texture_width * ((float*)texCoordPoints.pointer)[texCoordPoints.size * i];     //u
					float img_v = texture_height * ((float*)texCoordPoints.pointer)[texCoordPoints.size * i+1];  //v

					int u0 = (int)floor(img_u);
					int v0 = (int)floor(img_v);

				}
			}
		}
		else if (pname == GL_TEXTURE_MIN_FILTER)
		{
		}
		else if (pname == GL_TEXTURE_WRAP_S)
		{
		}
		else if (pname == GL_TEXTURE_WRAP_T)
		{
		}
		
	}
}


void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
	if (target == GL_TEXTURE_2D)
	{
		int bitCount = 0;
		int dataBit = 0;
		GLbyte imageBitSize = 0;

		switch (type)
		{
		case GL_UNSIGNED_BYTE:
			dataBit = 8;
			break;
		case GL_UNSIGNED_SHORT_5_6_5:
			dataBit = 16;
			break;
		case  GL_UNSIGNED_SHORT_4_4_4_4:
			dataBit = 16;
			break;
		case GL_UNSIGNED_SHORT_5_5_5_1:
			dataBit = 16;
			break;
		default:
			break;
		}

		switch (internalformat)
		{
		case GL_ALPHA:	case GL_LUMINANCE:
			bitCount = 1 * dataBit;
			break;
		case GL_LUMINANCE_ALPHA:
			bitCount = 2 * dataBit;
			break;
		case GL_RGB:
			bitCount = 3 * dataBit;
			break;
		case  GL_RGBA:
			bitCount = 4 * dataBit;
			break;
		default:
			break;
		}
		curTex->data = (BYTE*)pixels;
		imageBitSize = width * height*bitCount;
		//pixels = imageBitSize;
	}
}