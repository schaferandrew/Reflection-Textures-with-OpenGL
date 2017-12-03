
// ChildView.h : interface of the CChildView class
//


#pragma once

#include "ShaderWnd\ShaderWnd.h"
#include "Mesh.h"
#include "Sphere.h"
#include "graphics\GrTexture.h"
#include "graphics\GrCubeTexture.h"
#include "Torus.h"

// CChildView window

class CChildView : public CShaderWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSave32771();
	virtual void RenderGL();
	virtual void InitGL();
	virtual void CleanGL();

private:
	CGrTexture m_fishtex;
	CGrTexture m_sphereTex;
	CGrCubeTexture m_cubeTex;
	CMesh m_fish;
	CSphere m_sphere;
	CSphere m_metallicSphere;
	CMesh m_skybox;
	CTorus m_metallicTorus;
};

