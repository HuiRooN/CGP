////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct ModelCount
	{
		int vertexCount;
		int textureCount;
		int normalCount;
		int faceCount;
	};

	struct ObjType
	{
		float x, y, z;
	};

	struct FaceType
	{
		int x, y, z;
		int tu, tv, tz;
		int nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*);
	bool Initialize(ID3D11Device*, WCHAR*);
	//TXT
	bool Initialize(ID3D11Device*, char*, WCHAR*);
	//OBJ
	bool Initialize(ID3D11Device*, WCHAR*, WCHAR*);
	//Terrain
	//bool InitializePlane(ID3D11Device*, WCHAR*);

	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	bool LoadObjModel(WCHAR*);
	bool LoadDataStructures(WCHAR*, ModelCount);
	bool ReadFileCounts(WCHAR*, ModelCount&);


private:
	bool InitializeBuffers(ID3D11Device*);
	//bool InitializePlaneBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;

	ModelType* m_model;
};

#endif