#include "stdafx.h"
#include "GLRenderer.h"
#include "OGLBasic.h"


CGLRenderer::CGLRenderer() : m_hrc(0)
{
}


CGLRenderer::~CGLRenderer()
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	HGLRC tempContext = wglCreateContext(pDC->m_hDC);
	wglMakeCurrent(pDC->m_hDC, tempContext);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		AfxMessageBox(_T("GLEW is not initialized!"));
	}

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	//int attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	//	WGL_CONTEXT_MINOR_VERSION_ARB, 1,
	//	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	//	0, 0 };
	//int attribs[] =
	//{
	//	WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	//	WGL_CONTEXT_MINOR_VERSION_ARB, 2,
	//	WGL_CONTEXT_PROFILE_MASK_ARB,
	//	WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	//	WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
	//	0
	//};
	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		m_hrc = wglCreateContextAttribsARB(pDC->m_hDC, 0, attribs);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(pDC->m_hDC, m_hrc);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		m_hrc = tempContext;
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	//Or better yet, use the GL3 way to get the version number
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	if (!m_hrc) return false;
	return true;
}

void CGLRenderer::InitScene(std::string strFilename)
{
	//opengl init
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.15f, 0.15f, 0.15f, 1.f);
	m_progBase.loadShader("shaders/basic.vert", CGLSLProgram::VERTEX);
	m_progBase.loadShader("shaders/basic.frag", CGLSLProgram::FRAGMENT);
	m_progBase.create_link();
	m_progBase.enable();
		m_progBase.addAttribute("vVertex");
		m_progBase.addAttribute("vNormal");
		m_progBase.addAttribute("vColor");
		m_progBase.addAttribute("vTexCoord");
		m_progBase.addUniform("mModelView");
		m_progBase.addUniform("mProjection");
		m_progBase.addUniform("bHasTexture");
		m_progBase.addUniform("sTexture");
	m_progBase.disable();
	if (!m_model.load(strFilename.c_str(), m_progBase))
	{
		AfxMessageBox(L"Problems Loading the file");
	}
	else return;

	m_modelViewMatrix = glm::mat4x4(1);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	if (h == 0) h = 1;
	float ratio = w / float(h);
	glViewport(0, 0, w, h);
	m_projMatrix = glm::perspective(COGLBasic::getInstance().fAngle, ratio, COGLBasic::getInstance().fNCP, COGLBasic::getInstance().fFCP);
	m_arcBall.Resize(w, h);
}


void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.15f, 0.15f, 0.15f, 1.f);
	assert(glGetError() == GL_NO_ERROR);
	m_modelViewMatrix = glm::translate(glm::mat4(1), -m_model.getCenter());
	float d = 1.f / m_model.getDiagonal();
	m_modelViewMatrix = glm::scale(glm::mat4(1), glm::vec3(d, d, d)) * m_modelViewMatrix;
	//m_modelViewMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -0.5)) * m_modelViewMatrix;
	m_modelViewMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -0.5)) * m_arcBall.GetTransformation() * m_modelViewMatrix;
	//m_modelViewMatrix =  * m_modelViewMatrix;
	m_progBase.enable();
		glActiveTexture(GL_TEXTURE0);
		glUniformMatrix4fv(m_progBase.getLocation("mModelView"), 1, GL_FALSE, glm::value_ptr(m_modelViewMatrix));
		glUniformMatrix4fv(m_progBase.getLocation("mProjection"), 1, GL_FALSE, glm::value_ptr(m_projMatrix));
		glUniform1i(m_progBase.getLocation("bHasTexture"), 1);	//0 false, otherwise true
		glUniform1i(m_progBase.getLocation("sTexture"), 0);
		m_model.draw();
	m_progBase.disable();

	SwapBuffers(pDC->m_hDC);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	//m_progBase.~CGLSLProgram();		//problems with that before the shaders are not destroyed
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}