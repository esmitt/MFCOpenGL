#pragma once
#include "loader/3DModel.h"
#include "GLSLProgram.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include "ArcBall.h"

class CGLRenderer
{
public:
	CGLRenderer();
	virtual ~CGLRenderer();
	bool CreateGLContext(CDC* pDC); 	// Creates OpenGL Rendering Context
	void InitScene(std::string strFilename);		// Scene preparation stuff
	void Reshape(CDC* pDC, int w, int h);	// Changing viewport
	void DrawScene(CDC* pDC);		// Draws the scene
	void DestroyScene(CDC* pDC);		// Cleanup
	void LButtonDown(int x, int y){ m_arcBall.OnMouseDown(glm::ivec2(x, y)); }
	void MouseMove(int x, int y){ m_arcBall.OnMouseMove(glm::ivec2(x, y)); }
protected:
	HGLRC	 m_hrc;                        // OpenGL Rendering Context 
	CArcBall m_arcBall;
private:
	C3DModel m_model;
	CGLSLProgram m_progBase;
	glm::mat4x4 m_projMatrix, m_modelViewMatrix;
};

