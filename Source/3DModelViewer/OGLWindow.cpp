#include "Resource.h"
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "OGLWindow.h"
#include "Matrix4x4.h"
#include "Vector4D.h"

OGLWindow::OGLWindow()
{
	
}

OGLWindow::~OGLWindow()
{
	//Clean up the renderable
	delete m_mesh;
}

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height)
{
	//InitWindow(hInstance, width, height);
}

HGLRC OGLWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat=ChoosePixelFormat(hdc,&pfd)))
	{
		return 0;
	}

	if(!SetPixelFormat(hdc,pixelformat,&pfd))
	{
		return 0;
	}

	if (!(hglrc=wglCreateContext(hdc)))
	{
		return 0;
	}

	if(!wglMakeCurrent(hdc,hglrc))
	{
		return 0;
	}

	return hglrc;
}

void OGLWindow::DestroyOGLWindow()
{
	DestroyOGLContext();

	DestroyWindow( m_hwnd );

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::DestroyOGLContext()
{
	if ( m_hglrc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hglrc );
		m_hglrc = NULL;
	}
	
	if ( !ReleaseDC ( m_hwnd, m_hdc ) )
		return FALSE;

	return TRUE;
}

BOOL OGLWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	RECT clientrect;

	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"OGLWindow", L"OGLWindow", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd )
		return FALSE;

	m_hdc = GetDC( m_hwnd );

	GetClientRect(m_hwnd, &clientrect);

	SetWindowPos(m_hwnd, NULL, 10, 10, (width << 1) - clientrect.right, (height << 1) - clientrect.bottom, SWP_HIDEWINDOW | SWP_NOREDRAW);
		
	m_width = width;
	m_height = height;

	if (!(m_hglrc = CreateOGLContext(m_hdc)))
		return FALSE;

	// Create a new mesh and then load the mesh from file
	m_mesh = new TriangleMesh();
	m_mesh->LoadMeshFromOBJFile("Mudkip.obj");
	return TRUE;
}

void OGLWindow::SetVisible ( BOOL visible )
{
	ShowWindow ( m_hwnd, visible? SW_SHOW : SW_HIDE );
}

void OGLWindow::Render()
{
	// Added GL_DEPTH_BUFFER_BIT for lighting
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enabling light 0 along with the depth test and setting the shade model to flat 
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);

	// Set the lighting source to a colour and a high intensity
	GLfloat m_lightsource[] = { 100.0, 100.0, 100.0, 1000 };
	GLfloat m_lcolour[] = { 0.0, 0.5, 0.5, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, m_lightsource);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, m_lcolour);

	glLoadIdentity();
	
	Matrix4x4 transformation;
	Matrix4x4 rX;
	Matrix4x4 rY;
	Matrix4x4 rZ;
	Matrix4x4 scale;

	// Best option would be to use the FPS calculate that each frame the rotation increments by a set amount
	// For now I'm incrementing by a float of 0.1f so that the roation speed doesn't get out of control!
	rotSpeed += 0.1f;

	// Set our rotation Axis and pass in the rotation speed
	rX.SetRotationXAxis(rotSpeed);
	rY.SetRotationYAxis(rotSpeed);
	rZ.SetRotationZAxis(rotSpeed);

	// Set the scale of the object
	scale.SetScale(0.25, 0.25, 0.25);

	// Transform the shape and rotate on one axis
	transformation.SetTranslate({ 0 ,0, -200 });
	transformation = rZ * scale;
	
	// Concatenate the rotations together
	//transformation = rX * rY * rZ * scale;

	// Render the shape with the new transformation
	m_mesh->Render(&transformation);
	
	glFlush();

	SwapBuffers(m_hdc);
	return;
}

void OGLWindow::Resize( int width, int height )
{
	RECT clientrect;

	GetClientRect(m_hwnd, &clientrect);

	int left = width - clientrect.right;
	int top = height - clientrect.bottom;

	m_width = width - left;
	m_height = height - top;

	double aspectratio = (double)m_width / (double)m_height;

	glViewport(left >> 1, top >> 1 , m_width, m_height );
	
	// Set it to model view and then create a frustum camera.
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glFrustum(aspectratio*-1.0, aspectratio* 1.0, -1.0, 1.0, -1.0, 1.0);

	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

BOOL OGLWindow::MouseLBDown ( int x, int y )
{
	
	return TRUE;
}

BOOL OGLWindow::MouseLBUp ( int x, int y )
{
	return TRUE;
}

BOOL OGLWindow::MouseMove ( int x, int y )
{
	return TRUE;
}