////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_TextureShader = 0;

	m_RexModel = 0;
	m_SonicModel = 0;
	m_WazowskiModel = 0;
	m_PlaneModel = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}

void GraphicsClass::FilterChange(D3D11_FILTER Filter)
{
	ID3D11SamplerState* pSamplerState;
	pSamplerState = m_TextureShader->Get_SamplerState();

	D3D11_SAMPLER_DESC samplerDesc;
	pSamplerState->GetDesc(&samplerDesc);

	samplerDesc.Filter = Filter;
	if (Filter == D3D11_FILTER_ANISOTROPIC)
	{
		samplerDesc.MaxAnisotropy = 16;
	}
	else
		samplerDesc.MaxAnisotropy = 0;

	m_D3D->GetDevice()->CreateSamplerState(&samplerDesc, &pSamplerState);
	m_TextureShader->Set_SamplerState(pSamplerState);
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model object.
	m_RexModel = new ModelClass;
	if(!m_RexModel)
	{
		return false;
	}

	m_SonicModel = new ModelClass;
	if (!m_SonicModel)
	{
		return false;
	}

	m_WazowskiModel = new ModelClass;
	if (!m_WazowskiModel)
	{
		return false;
	}

	m_PlaneModel = new ModelClass;
	if (!m_PlaneModel)
	{
		return false;
	}

	// Initialize the model object.
	//result = m_Model->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/seafloor.dds");
//	result = m_Model->Initialize(m_D3D->GetDevice(), "../Engine/data/chair.txt", L"../Engine/data/chair_d.dds");
	result = m_RexModel->Initialize(m_D3D->GetDevice(), L"../Engine/data/Rex/rex.obj", L"../Engine/data/Rex/rex.png");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_SonicModel->Initialize(m_D3D->GetDevice(), L"../Engine/data/Sonic/Sonic.obj", L"../Engine/data/Sonic/Sonic.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_WazowskiModel->Initialize(m_D3D->GetDevice(), L"../Engine/data/Wazowski/wazowski.obj", L"../Engine/data/Wazowski/wazowski.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_PlaneModel->Initialize(m_D3D->GetDevice(), L"../Engine/data/Plane/Plane.obj", L"../Engine/data/Plane/PlaneTexture.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	if(m_RexModel)
	{
		m_RexModel->Shutdown();
		delete m_RexModel;
		m_RexModel = 0;
	}

	if (m_SonicModel)
	{
		m_SonicModel->Shutdown();
		delete m_SonicModel;
		m_SonicModel = 0;
	}

	if (m_WazowskiModel)
	{
		m_WazowskiModel->Shutdown();
		delete m_WazowskiModel;
		m_WazowskiModel = 0;
	}

	if (m_PlaneModel)
	{
		m_PlaneModel->Shutdown();
		delete m_PlaneModel;
		m_PlaneModel = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	D3DXMATRIX matScale, matRot, matTrans;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->SetPosition(0, 0, -14.0f);
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&matRot, float(D3DXToRadian(80.f)));

	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);
	D3DXMatrixRotationY(&matRot, -rotation);
	D3DXMatrixTranslation(&matTrans, -2.5f, -1.5f, -5.0f);
	worldMatrix = matScale * matRot * matTrans;
	
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_RexModel->Render(m_D3D->GetDeviceContext());
	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_RexModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
									 m_RexModel->GetTexture());
	if(!result)
	{
		return false;
	}

	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 1.5f);
	D3DXMatrixRotationY(&matRot, -rotation);
	D3DXMatrixTranslation(&matTrans, 3.0f, -1.5f, -5.0f);
	worldMatrix = matScale * matRot * matTrans;

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SonicModel->Render(m_D3D->GetDeviceContext());
	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_SonicModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SonicModel->GetTexture());
	if (!result)
	{
		return false;
	}


	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);
	D3DXMatrixRotationY(&matRot, -rotation);
	D3DXMatrixTranslation(&matTrans, 0.0f, -1.5f, 1.0f);
	worldMatrix = matScale * matRot * matTrans;

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_WazowskiModel->Render(m_D3D->GetDeviceContext());
	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_WazowskiModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_WazowskiModel->GetTexture());
	if (!result)
	{
		return false;
	}

	D3DXMatrixRotationX(&matRot, float(D3DXToRadian(80.f)));
	D3DXMatrixTranslation(&matTrans, 0.0f, -5.5f, 0.0f);
	worldMatrix = matRot * matTrans;
	
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_PlaneModel->Render(m_D3D->GetDeviceContext());
	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_PlaneModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_PlaneModel->GetTexture());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}