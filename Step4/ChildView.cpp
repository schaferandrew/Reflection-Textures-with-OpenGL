
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Step4.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CShaderWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_SAVE32771, &CChildView::OnFileSave32771)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CShaderWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::InitGL()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// Fish
	m_fishtex.LoadFile(L"models/BLUEGILL.BMP");

	m_fish.LoadOBJ("models\\fish4.obj");
	m_fish.m_program = LoadShaders("ShaderWnd/vertex.glsl", "ShaderWnd/fragment.glsl");
	m_fish.InitGL();

	// Sphere
	m_sphereTex.LoadFile(L"textures/bumpmap.jpg");
	m_sphere.SetRadius(3);
	m_sphere.m_program = LoadShaders("ShaderWnd/vertexSphere.glsl", "ShaderWnd/fragmentSphere.glsl");
	m_sphere.InitGL();

	// Skybox
	m_cubeTex.LoadFiles(L"textures/right.jpg", L"textures/left.jpg", L"textures/top.jpg",
		L"textures/bottom.jpg", L"textures/front.jpg", L"textures/back.jpg");

	m_skybox.CreateCube();
	m_skybox.m_program = LoadShaders("ShaderWnd/vertexSky.glsl", "ShaderWnd/fragmentSky.glsl");
	m_skybox.InitGL();

	// Metallic Sphere
	m_metallicSphere.SetRadius(3);
	m_metallicSphere.m_program = LoadShaders("ShaderWnd/vertexSphere2.glsl", "ShaderWnd/fragmentSphere2.glsl");
	m_metallicSphere.InitGL();

	// Metallic Torus
	m_metallicTorus.SetR1(3);
	m_metallicTorus.SetR2(4);
	//m_metallicTorus.Draw();
	//m_metallicTorus.m_program = LoadShaders("ShaderWnd/vertex.glsl", "ShaderWnd/fragment.glsl");
	m_metallicTorus.InitGL();
}

void CChildView::RenderGL()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Fish
	m_program = m_fish.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "diffuse_mat"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(m_mPVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(m_mVM));

	glm::vec4 light_position(-5.f, 5.f, -5.f, 0.f);
	glm::vec4 light_ambient(0.2f, 0.2f, 0.2f, 1.f);
	glm::vec4 light_diffuse(1.f, 1.f, 1.f, 1.f);
	glm::vec4 light_specular(1.f, 1.f, 1.f, 1.f);

	glm::vec4 material_ambient(1.f, 1.f, 1.f, 1.f);
	glm::vec4 material_diffuse(1.f, 1.f, 1.f, 1.f);
	glm::vec4 material_specular(1.f, 1.f, 1.f, 1.f);
	float material_shininess = 100.0f;

	glm::vec4 ambient_product = light_ambient*material_ambient;
	glm::vec4 diffuse_product = light_diffuse*material_diffuse;
	glm::vec4 specular_product = light_specular*material_specular;

	glUniform4fv(glGetUniformLocation(m_program, "AmbientProduct"), 1, value_ptr(ambient_product));
	glUniform4fv(glGetUniformLocation(m_program, "DiffuseProduct"), 1, value_ptr(diffuse_product));
	glUniform4fv(glGetUniformLocation(m_program, "SpecularProduct"), 1, value_ptr(specular_product));
	glUniform4fv(glGetUniformLocation(m_program, "LightPosition"), 1, value_ptr(light_position));
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), material_shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fishtex.TexName());

	m_fish.RenderGL();

	// Sphere
	m_program = m_sphere.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "bump_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	glm::mat4 M = translate(mat4(1.f), vec3(-10., 0., 0.))
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.));
	glm::mat4 VM = m_mVM*M;
	glm::mat4 PVM = m_mPVM*M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_sphereTex.TexName());

	m_sphere.RenderGL();

	// Skybox
	m_program = m_skybox.m_program;
	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "skybox"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mP");
	m_nVM = glGetUniformLocation(m_program, "mV");
	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(m_mProjection));
	glm::mat4 view = glm::mat4(glm::mat3(m_mVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(view));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTex.TexName());
	glDepthFunc(GL_LEQUAL);
	m_skybox.RenderGL();

	// Metallic Sphere

	m_program = m_metallicSphere.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "env_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(10., 0., 0.));
	VM = m_mVM*M;
	PVM = m_mPVM*M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	m_metallicSphere.RenderGL();

	// Torus
	m_program = m_metallicSphere.m_program;
	glUseProgram(m_program);

	glUniform1i(glGetUniformLocation(m_program, "env_map"), 0);

	m_nPVM = glGetUniformLocation(m_program, "mPVM");
	m_nVM = glGetUniformLocation(m_program, "mVM");

	M = translate(mat4(1.f), vec3(5., 0., 10.)) 
		* rotate(mat4(1.f), 90.f, vec3(1., 0., 0.));
	VM = m_mVM*M;
	PVM = m_mPVM*M;

	glUniformMatrix4fv(m_nPVM, 1, GL_FALSE, value_ptr(PVM));
	glUniformMatrix4fv(m_nVM, 1, GL_FALSE, value_ptr(VM));
	m_metallicTorus.RenderGL();
}

void CChildView::CleanGL()
{

}

void CChildView::OnFileSave32771()
{
}
