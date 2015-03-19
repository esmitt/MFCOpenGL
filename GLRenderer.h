#pragma once
#include "loader\3DModel.h"
#include "GLSLProgram.h"
#include "glm\glm.hpp"
#include <string>
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
protected:
	void CreateBuffers();	                        // Creates VAO and VBOs and fill them with data
	
	HGLRC	 m_hrc;                        // OpenGL Rendering Context 
	//CDC* pDC;
	//CGLProgram* m_pProgram;	               // Program
	//CGLShader* m_pVertSh;		       // Vertex shader
	//CGLShader* m_pFragSh;		       // Fragment shader

	//GLuint m_vaoID[2];			// two vertex array objects, one for each drawn object
	//GLuint m_vboID[3];			// three VBOs
private:
	C3DModel m_model;
	CGLSLProgram m_progBase;
	glm::mat4x4 m_projMatrix, m_modelViewMatrix, m_modelMatrix;
};

