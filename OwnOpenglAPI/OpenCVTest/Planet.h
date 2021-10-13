#pragma once
#include "customGL.h"
#include <vector>

const float PI = 3.1415926f;

struct Planet {
	//GLUquadricObj *pObj;
	GLuint texture;
	GLfloat radius;
	GLfloat rotate = 0.0f;
	GLfloat* VertexArray;
	GLfloat* TextCoordArray;
	GLfloat* NormalArray;
	GLfloat* ColorArray;
	std::vector<int> indices;

	GLint elementCount;
	GLint stackCount;
	GLint sectorCount;



	Planet(GLfloat _radius, GLint _stackCount, GLint _sectorCount)
	{
		this->radius = _radius;
		elementCount = (_stackCount + 1) * (_sectorCount + 1);
		stackCount = _stackCount;
		sectorCount = _sectorCount;
		VertexArray = new GLfloat[elementCount * 3];
		TextCoordArray = new GLfloat[elementCount * 2];
		NormalArray = new GLfloat[elementCount * 3];
		ColorArray = new GLfloat[elementCount * 4];
		//indices = new GLint[elementCount * 3];
		CaculateSphere();
	};


	

	void DrawSphere()
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, VertexArray);
		glTexCoordPointer(2, GL_FLOAT, 0, TextCoordArray);
		glNormalPointer(GL_FLOAT, 0, NormalArray);
		glColorPointer(4, GL_FLOAT, 0, ColorArray);

		//glDrawArrays(GL_POINTS, 0, elementCount);
		glDrawElements(GL_POLYGON, indices.size(), GL_INT, indices.data());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	void CaculateSphere()
	{

		GLfloat x, y, z, xy;                              // vertex position
		GLfloat nx, ny, nz, lengthInv = 1.0f / radius;    // normal
		GLfloat s, t;                                     // texCoord

		GLfloat sectorStep = 2 * PI / sectorCount;
		GLfloat stackStep = PI / stackCount;
		GLfloat sectorAngle, stackAngle;

		GLuint temp_index = 0, temp_index_TCA = 0;
		GLuint temp_index_color = 0;


		for (int i = 0; i <= stackCount; ++i)
		{
			//temp_index = i * (stackCount + 1);
			//temp_index_TCA = i * (stackCount + 1);

			stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			for (int j = 0; j <= sectorCount; ++j)
			{
			sectorAngle =j * sectorStep;
				if (i == 0)
				{
					x = 0;						        //cos(PI/2) = 0 쓰레기값이 나와 임시로 지정해줌
				}
				else
				{
					// vertex position
					x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				}
					y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				VertexArray[temp_index] = x;
				VertexArray[temp_index + 1] = y;
				VertexArray[temp_index + 2] = z;

				printf("Vertex[%f, %f, %f]\n", x, y, z);


				ColorArray[temp_index_color++] = x;
				ColorArray[temp_index_color++] = y;
				ColorArray[temp_index_color++] = z;
				ColorArray[temp_index_color++] = 1;


				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				NormalArray[temp_index++] = nx;
				NormalArray[temp_index++] = ny;
				NormalArray[temp_index++] = nz;

				s = (float)j / sectorCount;
				t = (float)i / stackCount;
				TextCoordArray[temp_index_TCA++] = s;
				TextCoordArray[temp_index_TCA++] = t;


			}

		}

		// indices
		//  k1--k1+1
		//  |  / |
		//  | /  |
		//  k2--k2+1
		GLuint k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     
			k2 = k1 + sectorCount + 1;     
			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				if (i != 0)                
				{
					// k1---> k2 ---> k1+1
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}
				if (i != (stackCount - 1))
				{ 
					// k1+1---> k2---> k2+1
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}


		vertexIndices = indices;

	}
};
