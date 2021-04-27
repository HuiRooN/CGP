////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	return true;
}

bool ModelClass::Initialize(ID3D11Device* device, WCHAR* textureFilename)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}
	return true;
}

bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::Initialize(ID3D11Device* device, WCHAR* ObjFilename, WCHAR* textureFilename)
{
	bool result;

	result = LoadObjModel(ObjFilename);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}
	return true;
}

//bool ModelClass::InitializePlane(ID3D11Device* device, WCHAR* textureFilename)
//{
//	bool result;
//
//	result = InitializePlaneBuffers(device);
//	if (!result)
//	{
//		return false;
//	}
//
//	result = LoadTexture(device, textureFilename);
//	if (!result)
//	{
//		return false;
//	}
//	return true;
//}

void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

//bool ModelClass::InitializePlaneBuffers(ID3D11Device* device)
//{
//	VertexType* vertices;
//	unsigned long* indices;
//	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
//	D3D11_SUBRESOURCE_DATA vertexData, indexData;
//	HRESULT result;
//
//	// Set the number of vertices in the vertex array.
//	m_vertexCount = 4;
//	// Set the number of indices in the index array.
//	m_indexCount = 6;
//
//	// Create the vertex array.
//	vertices = new VertexType[m_vertexCount];
//	if (!vertices)
//	{
//		return false;
//	}
//
//	// Create the index array.
//	indices = new unsigned long[m_indexCount];
//	if (!indices)
//	{
//		return false;
//	}
//
//	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); // Bottom left.
//	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
//	vertices[1].position = D3DXVECTOR3(-1.f, 1.0f, 0.0f); // Top left.
//	vertices[1].texture = D3DXVECTOR2(-1.f, -1.0f);
//	vertices[2].position = D3DXVECTOR3(1.f, 1.0f, 0.0f); // Top right.
//	vertices[2].texture = D3DXVECTOR2(1.f, -1.0f);
//	vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f); // Bottom right.
//	vertices[3].texture = D3DXVECTOR2(1.0f, 1.0f);
//
//	// Load the index array with data.
//	indices[0] = 0; // Bottom left.
//	indices[1] = 1; // Top middle.
//	indices[2] = 2; // Bottom right.
//	indices[3] = 0; // Bottom left.
//	indices[4] = 2; // Top middle.
//	indices[5] = 3; // Bottom right.
//
//	// Set up the description of the static vertex buffer.
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = 0;
//	vertexBufferDesc.MiscFlags = 0;
//	vertexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the vertex data.
//	vertexData.pSysMem = vertices;
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;
//
//	// Now create the vertex buffer.
//	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Set up the description of the static index buffer.
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
//	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufferDesc.CPUAccessFlags = 0;
//	indexBufferDesc.MiscFlags = 0;
//	indexBufferDesc.StructureByteStride = 0;
//
//	// Give the subresource structure a pointer to the index data.
//	indexData.pSysMem = indices;
//	indexData.SysMemPitch = 0;
//	indexData.SysMemSlicePitch = 0;
//
//	// Create the index buffer.
//	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Release the arrays now that the vertex and index buffers have been created and loaded.
//	delete[] vertices;
//	vertices = 0;
//
//	delete[] indices;
//	indices = 0;
//
//	return true;
//}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

bool ModelClass::LoadObjModel(WCHAR* ObjFilename)
{
	bool result;

	ModelCount modelCount;
	memset(&modelCount, 0, sizeof(modelCount));

	// Load in the number of vertices, tex coords, normals, and faces so that the data structures can be initialized with the exact sizes needed.
	result = ReadFileCounts(ObjFilename, modelCount);
	if (!result)
	{
		return false;
	}

	// Load the data from the file into the data structures and then output it in our model format.
	result = LoadDataStructures(ObjFilename, modelCount);
	if (!result)
	{
		return false;
	}
	return true;
}

bool ModelClass::ReadFileCounts(WCHAR* ObjFilename, ModelCount& modelCount)
{
	ifstream fin;
	char input;

	fin.open(ObjFilename);
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { modelCount.vertexCount++; }
			if (input == 't') { modelCount.textureCount++; }
			if (input == 'n') { modelCount.normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { modelCount.faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}
	fin.close();

	return true;
}

bool ModelClass::LoadDataStructures(WCHAR* ObjFilename, ModelCount modelCount)
{
	ObjType* vertices, * texcoords, * normals;
	FaceType* faces;
	ifstream fin;
	int vIndex, tIndex, nIndex;
	char input, input2;

	ModelCount temp;
	memset(&temp, 0, sizeof(temp));

	vertices = new ObjType[modelCount.vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new ObjType[modelCount.textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new ObjType[modelCount.normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[modelCount.faceCount];
	if (!faces)
	{
		return false;
	}

	// Open the file.
	fin.open(ObjFilename);
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			if (input == ' ')
			{
				fin >> vertices[temp.vertexCount].x >> vertices[temp.vertexCount].y >> vertices[temp.vertexCount].z;

				vertices[temp.vertexCount].z = vertices[temp.vertexCount].z * -1.0f;
				temp.vertexCount++;
			}

			if (input == 't')
			{
				fin >> texcoords[temp.textureCount].x >> texcoords[temp.textureCount].y;

				texcoords[temp.textureCount].y = 1.0f - texcoords[temp.textureCount].y;
				temp.textureCount++;
			}

			if (input == 'n')
			{
				fin >> normals[temp.normalCount].x >> normals[temp.normalCount].y >> normals[temp.normalCount].z;

				normals[temp.normalCount].z = normals[temp.normalCount].z * -1.0f;
				temp.normalCount++;
			}
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				fin >> faces[temp.faceCount].z >> input2 >> faces[temp.faceCount].tz >> input2 >> faces[temp.faceCount].nz
					>> faces[temp.faceCount].y >> input2 >> faces[temp.faceCount].tv >> input2 >> faces[temp.faceCount].ny
					>> faces[temp.faceCount].x >> input2 >> faces[temp.faceCount].tu >> input2 >> faces[temp.faceCount].nx;
				temp.faceCount++;
			}
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	ObjType *newVertices, *newTexcoords, *newNormals;

	newVertices = new ObjType[temp.faceCount * 3];
	if (!newVertices)
	{
		return false;
	}

	newTexcoords = new ObjType[temp.faceCount * 3];
	if (!newTexcoords)
	{
		return false;
	}

	newNormals = new ObjType[temp.faceCount * 3];
	if (!newNormals)
	{
		return false;
	}

	int index = 0;
	for (int i = 0; i < temp.faceCount; i++)
	{
		index = 3 * i;
		vIndex = faces[i].x - 1;
		tIndex = faces[i].tu - 1;
		nIndex = faces[i].nz - 1;

		newVertices[index].x = vertices[vIndex].x;
		newVertices[index].y = vertices[vIndex].y;
		newVertices[index].z = vertices[vIndex].z;

		newTexcoords[index].x = texcoords[tIndex].x;
		newTexcoords[index].y = texcoords[tIndex].y;

		newNormals[index].x = normals[nIndex].x;
		newNormals[index].y = normals[nIndex].y;
		newNormals[index].z = normals[nIndex].z;

		index = 3 * i + 1;
		vIndex = faces[i].y - 1;
		tIndex = faces[i].tv - 1;
		nIndex = faces[i].ny - 1;

		newVertices[index].x = vertices[vIndex].x;
		newVertices[index].y = vertices[vIndex].y;
		newVertices[index].z = vertices[vIndex].z;

		newTexcoords[index].x = texcoords[tIndex].x;
		newTexcoords[index].y = texcoords[tIndex].y;

		newNormals[index].x = normals[nIndex].x;
		newNormals[index].y = normals[nIndex].y;
		newNormals[index].z = normals[nIndex].z;

		index = 3 * i + 2;
		vIndex = faces[i].z - 1;
		tIndex = faces[i].tz - 1;
		nIndex = faces[i].nz - 1;

		newVertices[index].x = vertices[vIndex].x;
		newVertices[index].y = vertices[vIndex].y;
		newVertices[index].z = vertices[vIndex].z;

		newTexcoords[index].x = texcoords[tIndex].x;
		newTexcoords[index].y = texcoords[tIndex].y;

		newNormals[index].x = normals[nIndex].x;
		newNormals[index].y = normals[nIndex].y;
		newNormals[index].z = normals[nIndex].z;
	}

	m_vertexCount = m_indexCount = modelCount.faceCount * 3;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	for (int i = 0; i < m_vertexCount; i++)
	{
		m_model[i].x = newVertices[i].x;
		m_model[i].y = newVertices[i].y;
		m_model[i].z = newVertices[i].z;

		m_model[i].tu = newTexcoords[i].x;
		m_model[i].tv = newTexcoords[i].y;

		m_model[i].nx = newNormals[i].x;
		m_model[i].ny = newNormals[i].y;
		m_model[i].nz = newNormals[i].z;
	}
	
	
	// Release the four data structures.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

