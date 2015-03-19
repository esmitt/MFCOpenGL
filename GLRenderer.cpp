#include "stdafx.h"
#include "GLRenderer.h"


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
		WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

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
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	return true;
}

void CGLRenderer::InitScene(std::string strFilename)
{
	if(!m_model.load("models\\TableDesk-AgiSoft.obj", TypeObject::SPECULAR))
	{
		AfxMessageBox(L"Problems Loading the file");
	}
	m_progBase.loadShader("shaders/basic.vert", CGLSLProgram::VERTEX);
	m_progBase.loadShader("shaders/basic.frag", CGLSLProgram::FRAGMENT);
		m_progBase.create_link();
		m_progBase.enable();
		m_progBase.addAttribute("vVertex");
		m_progBase.addAttribute("vNormal");
		m_progBase.addAttribute("vColor");
		m_progBase.addAttribute("vTexCoord");
		m_progBase.addUniform("mProjection");
		m_progBase.addUniform("sTexture");
		m_progBase.addUniform("bHasTexture");
		m_progBase.addUniform("mModelView");
	m_progBase.disable();
	m_modelMatrix = glm::mat4x4(1);
	m_modelViewMatrix = glm::mat4x4(1);
	m_projMatrix = glm::mat4x4(1);
}

void gluPerspective2(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;
	fH = tan(fovY / 360 * pi) * zNear;	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}
void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	float ratio = w / float(h);
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	//glViewport(0, 0, w, h);
	// Projection view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set our current view perspective
	const float NCP = 0.01f;
	const float FCP = 52.f;
	const float fAngle = 45.f;
	gluPerspective2(fAngle, ratio, NCP, FCP);
	// Model view
	glMatrixMode(GL_MODELVIEW);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.15f, 0.15f, 0.15f, 1.f);
	glLoadIdentity();
	glTranslatef(0.f, 0.f, -2.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();
	SwapBuffers(pDC->m_hDC);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}