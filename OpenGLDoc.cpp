
// OpenGLDoc.cpp : implementation of the COpenGLDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGL.h"
#endif

#include "OpenGLDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COpenGLDoc

IMPLEMENT_DYNCREATE(COpenGLDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenGLDoc, CDocument)
END_MESSAGE_MAP()


// COpenGLDoc construction/destruction

COpenGLDoc::COpenGLDoc()
{
	// TODO: add one-time construction code here
	//AllocConsole();

	//*stdout = *_tfdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_WRONLY), _T("a"));
	//*stderr = *_tfdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_ERROR_HANDLE), _O_WRONLY), _T("a"));
	//*stdin = *_tfdopen(_open_osfhandle((intptr_t)GetStdHandle(STD_INPUT_HANDLE), _O_WRONLY), _T("r"));
}

COpenGLDoc::~COpenGLDoc()
{
}

BOOL COpenGLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	//if(!m_model.load("models\\TableDesk-AgiSoft.obj", TypeObject::SPECULAR))
	//{
	//	AfxMessageBox(L"Problems Loading the file");
	//}
	//m_progBase.loadShader("shaders/basic.vert", CGLSLProgram::VERTEX);
	//m_progBase.loadShader("shaders/basic.frag", CGLSLProgram::FRAGMENT);
	//	m_progBase.create_link();
	//	m_progBase.enable();
	//	m_progBase.addAttribute("vVertex");
	//	m_progBase.addAttribute("vNormal");
	//	m_progBase.addAttribute("vColor");
	//	m_progBase.addAttribute("vTexCoord");
	//	m_progBase.addUniform("mProjection");
	//	m_progBase.addUniform("sTexture");
	//	m_progBase.addUniform("bHasTexture");
	//	m_progBase.addUniform("mModelView");
	//m_progBase.disable();
	//m_modelMatrix = glm::mat4x4(1);
	//m_modelViewMatrix = glm::mat4x4(1);
	//m_projMatrix = glm::mat4x4(1);
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}


// COpenGLDoc serialization

void COpenGLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void COpenGLDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void COpenGLDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void COpenGLDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// COpenGLDoc diagnostics

#ifdef _DEBUG
void COpenGLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COpenGLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COpenGLDoc commands
