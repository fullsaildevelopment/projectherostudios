#include "stdafx.h"
#include "Entity.h"

unsigned int createEntity(World * world)
{
	unsigned int entity;
	//Mark entity for Creation by finding the first index in the entity list that has no components. 
	//Return the index of this marked entity
	for (entity = 0; entity < ENTITY_COUNT; ++entity)
	{
		if (world->mask[entity] == COMPONENT_NONE)
		{
			printf("Entity created: %d\n", entity);
			return(entity);
		}
	}

	printf("Error!  No more entities left!\n");
	return(ENTITY_COUNT);
}

void destroyEntity(World * world, unsigned int entity)
{
	//Set component list for current entity to none.
	printf("Entity destroyed: %d\n", entity);
	world->mask[entity] = COMPONENT_NONE;
}

unsigned int createDebugTransformLines(World * world, DebugMesh mesh)
{
	unsigned int entity = createEntity(world);
	world->mask[entity] =  COMPONENT_DEBUGMESH;
	world->debug_mesh[entity].vertexCount = 6;

	static simple_mesh lineVerts[]
	{
		simple_mesh{ 0,	0, 0, 0, 0,0,0,0,0},//00	Green
		simple_mesh{ 0,	2.0f, 0, 0,0,0,0,0},//01	Green
		simple_mesh{ 0,	0, 0, 0, 0,0,0,0,0},//00	Red
		simple_mesh{ 2.0f, 0, 0, 0,0,0,0,0},//01	Red
		simple_mesh{ 0,	0, 0, 0, 0,0,0,0,0},//00	Blue
		simple_mesh{ 0,	0, 2.0f, 0,0,0,0,0},//01	Blue	
	};

	

	world->debug_mesh[entity].vertexBufferStride = sizeof(simple_mesh);
	world->debug_mesh[entity].vertexBufferOffset = 0;

	world->debug_mesh[entity].vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	world->debug_mesh[entity].vertexBufferDesc.ByteWidth = sizeof(simple_mesh) * world->debug_mesh[entity].vertexCount;
	world->debug_mesh[entity].vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	world->debug_mesh[entity].vertexBufferDesc.CPUAccessFlags = 0;
	world->debug_mesh[entity].vertexBufferDesc.MiscFlags = 0;
	world->debug_mesh[entity].vertexBufferDesc.StructureByteStride = 0;

	world->debug_mesh[entity].vertexData.pSysMem = lineVerts;
	world->debug_mesh[entity].vertexData.SysMemPitch = 0;
	world->debug_mesh[entity].vertexData.SysMemSlicePitch = 0;
	return 0;
}

unsigned int createDebugGrid(World * world, DebugMesh mesh)
{
	unsigned int entity = createEntity(world);
	world->mask[entity] = COMPONENT_DEBUGMESH;
	world->debug_mesh[entity].vertexCount = 44;

	static primalVert gridVerts[]
	{
		primalVert{ XMFLOAT3(-.5f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//00	Green
		primalVert{ XMFLOAT3(-.5f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//01	Green
		primalVert{ XMFLOAT3(-.4f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//02	Red
		primalVert{ XMFLOAT3(-.4f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//03	Red
		primalVert{ XMFLOAT3(-.3f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//04	Blue
		primalVert{ XMFLOAT3(-.3f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//05	Blue	
		primalVert{ XMFLOAT3(-.2f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//06	Blue
		primalVert{ XMFLOAT3(-.2f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//07	Red
		primalVert{ XMFLOAT3(-.1f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//08	Red
		primalVert{ XMFLOAT3(-.1f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//09	Green
		primalVert{ XMFLOAT3( .0f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//10	Green							 		   
		primalVert{ XMFLOAT3( .0f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//11	Green
		primalVert{ XMFLOAT3( .1f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//12	Green	
		primalVert{ XMFLOAT3( .1f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//13	Green
		primalVert{ XMFLOAT3( .2f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//14	Green	
		primalVert{ XMFLOAT3( .2f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//15	Green
		primalVert{ XMFLOAT3( .3f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//16	Green								  
		primalVert{ XMFLOAT3( .3f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//17	Green
		primalVert{ XMFLOAT3( .4f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//18	Green	
		primalVert{ XMFLOAT3( .4f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//19	Green
		primalVert{ XMFLOAT3( .5f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//20	Green	
		primalVert{ XMFLOAT3( .5f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//21	Green
		
		primalVert{ XMFLOAT3( .5f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//00	Green
		primalVert{ XMFLOAT3(-.5f, 0, -.5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//01	Green
		primalVert{ XMFLOAT3( .5f, 0, -.4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//02	Red
		primalVert{ XMFLOAT3(-.5f, 0, -.4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//03	Red
		primalVert{ XMFLOAT3( .5f, 0, -.3f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//04	Blue
		primalVert{ XMFLOAT3(-.5f, 0, -.3f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//05	Blue	
		primalVert{ XMFLOAT3( .5f, 0, -.2f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//06	Blue
		primalVert{ XMFLOAT3(-.5f, 0, -.2f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//07	Red
		primalVert{ XMFLOAT3( .5f, 0, -.1f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//08	Red
		primalVert{ XMFLOAT3(-.5f, 0, -.1f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//09	Green
		primalVert{ XMFLOAT3( .5f, 0,  .0f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//10	Green							 		   
		primalVert{ XMFLOAT3(-.5f, 0,  .0f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//11	Green
		primalVert{ XMFLOAT3( .5f, 0,  .1f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//12	Green	
		primalVert{ XMFLOAT3(-.5f, 0,  .1f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//13	Green
		primalVert{ XMFLOAT3( .5f, 0,  .2f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//14	Green	
		primalVert{ XMFLOAT3(-.5f, 0,  .2f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//15	Green
		primalVert{ XMFLOAT3( .5f, 0,  .3f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//16	Green								  
		primalVert{ XMFLOAT3(-.5f, 0,  .3f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//17	Green
		primalVert{ XMFLOAT3( .5f, 0,  .4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//18	Green	
		primalVert{ XMFLOAT3(-.5f, 0,  .4f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//19	Green
		primalVert{ XMFLOAT3( .5f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//20	Green	
		primalVert{ XMFLOAT3(-.5f, 0,  .5f),		XMFLOAT4(0, 1.0f,	0.0f, 1.0f) },//21	Green
	};

	
	world->debug_mesh[entity].vertexBufferStride = sizeof(primalVert);
	world->debug_mesh[entity].vertexBufferOffset = 0;

	world->debug_mesh[entity].vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	world->debug_mesh[entity].vertexBufferDesc.ByteWidth = sizeof(primalVert) * world->debug_mesh[entity].vertexCount;
	world->debug_mesh[entity].vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	world->debug_mesh[entity].vertexBufferDesc.CPUAccessFlags = 0;
	world->debug_mesh[entity].vertexBufferDesc.MiscFlags = 0;
	world->debug_mesh[entity].vertexBufferDesc.StructureByteStride = 0;

	world->debug_mesh[entity].vertexData.pSysMem = gridVerts;
	world->debug_mesh[entity].vertexData.SysMemPitch = 0;
	world->debug_mesh[entity].vertexData.SysMemSlicePitch = 0;
	return 0;
}

unsigned int createFrustum(World * world, Mesh meshd)
{
	//Create entity
	unsigned int entity = createEntity(world);
	world->mask[entity] = COMPONENT_MESH;


	world->mesh[entity].indexCount = 36;
	world->mesh[entity].vertexCount = 0;

	// Set up the description of the static index buffer.
	world->mesh[entity].indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	world->mesh[entity].indexBufferDesc.ByteWidth = sizeof(unsigned int) * world->mesh[entity].indexCount;
	world->mesh[entity].indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	world->mesh[entity].indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	world->mesh[entity].indexBufferDesc.MiscFlags = 0;
	world->mesh[entity].indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
//	world->mesh[entity].indexData.pSysMem = PainInTheFrust_Intdices;
	world->mesh[entity].indexData.SysMemPitch = 0;
	world->mesh[entity].indexData.SysMemSlicePitch = 0;

//	world->mesh[entity].texture = nullptr;
	world->mesh[entity].vertexBufferStride = sizeof(primalVert);
	world->mesh[entity].vertexBufferOffset = 0;
	world->mesh[entity].vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	world->mesh[entity].vertexBufferDesc.ByteWidth = sizeof(primalVert) * world->mesh[entity].vertexCount;
	world->mesh[entity].vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	world->mesh[entity].vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	world->mesh[entity].vertexBufferDesc.MiscFlags = 0;
	world->mesh[entity].vertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA g;
	ZeroMemory(&g, sizeof(g));

	world->mesh[entity].vertexData.pSysMem = &g;
	world->mesh[entity].vertexData.SysMemPitch = 0;
	world->mesh[entity].vertexData.SysMemSlicePitch = 0;
	return 0;
}

unsigned int createMesh(ID3D11Device * dev, World * world, Mesh mesh)
{
	unsigned int entity = createEntity(world);
	
	#pragma region ReadAnimationData	
	int animFrameCount = 0;
	int nodeCount = 0;
	double duration = 0;
	std::vector<int> parent_Indices;
	std::vector<XMMATRIX> invBindPosesForJoints;
	std::ifstream matFile("../meshData.txt", std::ios::in | std::ios::binary);
	matFile.read((char*)&duration, sizeof(double));	
	matFile.read((char*)&nodeCount, sizeof(int));
	matFile.read((char*)&animFrameCount, sizeof(int));
	anim_clip myAnim;
	char * fileName1, *fileName2, *fileName3;

	int bSize = sizeof(double) + sizeof(int) + sizeof(int);
	for (int i = 0; i < animFrameCount; i++)
	{
		keyframe myKey;

		//current frame time offset
		double curTime = 0;
		matFile.read((char*)&curTime, sizeof(double));
		bSize += sizeof(double);
		XMFLOAT4X4 temp;
		for (int h = 0; h < nodeCount; h++)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					//joint matrix data

					matFile.read((char*)&temp(j,k), sizeof(float));
					bSize += sizeof(float);
				}
			}	
			myKey.joints.push_back(XMLoadFloat4x4(&temp));
		}
		//myKey.joints = XMLoadFloat4x4(&temp);
		myKey.time = curTime / 1000;
		myAnim.frames.push_back(myKey);
	}
	for (int i = 0; i < nodeCount; i++)
	{
		int j = 0;
		matFile.read((char*)&j, sizeof(int));
		bSize += sizeof(int);
		parent_Indices.push_back(j);
	}
	XMFLOAT4X4 fTemp;
	for (int h = 0; h < nodeCount; h++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				//joint matrix data

				matFile.read((char*)&fTemp(j,k), sizeof(float));
				bSize += sizeof(float);
			}
		}
		invBindPosesForJoints.push_back(XMMatrixInverse(NULL, XMLoadFloat4x4(&fTemp)));
	}

	myAnim.duration = duration;

	myAnim.parent_Indicies = parent_Indices;
#pragma endregion

	#pragma region ReadMaterials

	/*
	Things to write:
		-MaterialType(sizeof(char))
			if(phong)
				-AmbientColor(24 byte double3)
				-DiffuseColor(24 byte double3)
				-SpecularColor(24 byte double3)
				-EmmisiveColor(24 byte double3)
				-TransparencyFactor(8 byte double)
				-Shininess(8 byte double)
				-ReflectionFactor(8 byte double)
			if(lambert)
				-AmbientColor(24 byte double3)
				-DiffuseColor(24 byte double3)
				-SpecularColor(24 byte double3)
				-EmmisiveColor(24 byte double3)
				-TransparencyFactor(8 byte double)
	*/
	int materialCount = 0;
	//matFile.read((char*)&materialCount, sizeof(int));
	char lambert = false;
	double ambientColor[3];
	double diffuseColor[3];
	double specularColor[3];
	double emmissiveColor[3];
	//double transparencyFactor;
	double shininess, transparency;
	//double reflectionFactor;
	int bufferSize = 0;
	bufferSize = sizeof(lambert) + sizeof(ambientColor) + sizeof(diffuseColor) + sizeof(specularColor) + sizeof(emmissiveColor) + sizeof(shininess);
	char *matBuffer = new char[bufferSize];
	matFile.clear();
	matFile.seekg(bSize);
	matFile.read(matBuffer, bufferSize);
	/*
		-AmbientColor(24 byte double3)
		- DiffuseColor(24 byte double3)
		- SpecularColor(24 byte double3)
		- EmmisiveColor(24 byte double3)
		- Shininess(8 byte double)
	*/

	memcpy(&lambert, &matBuffer[0], sizeof(lambert));

	if (lambert)
	{
		bSize += sizeof(lambert) + sizeof(ambientColor) + sizeof(diffuseColor) + sizeof(emmissiveColor) + sizeof(transparency);

		memcpy(&ambientColor, &matBuffer[sizeof(lambert)], sizeof(ambientColor));

		memcpy(&diffuseColor, &matBuffer[sizeof(lambert) + sizeof(ambientColor)], sizeof(diffuseColor));

		memcpy(&emmissiveColor, &matBuffer[sizeof(lambert) + sizeof(ambientColor) + sizeof(diffuseColor)], sizeof(emmissiveColor));

		memcpy(&transparency, &matBuffer[sizeof(lambert) + sizeof(ambientColor) + sizeof(diffuseColor)] + sizeof(emmissiveColor), sizeof(double));
	}
	else
	{
		bSize += sizeof(lambert) + sizeof(ambientColor) + sizeof(diffuseColor) + sizeof(specularColor) + sizeof(emmissiveColor) + sizeof(shininess);

		memcpy(&ambientColor, &matBuffer[sizeof(lambert)], sizeof(ambientColor));

		memcpy(&diffuseColor, &matBuffer[sizeof(lambert) + sizeof(ambientColor)], sizeof(diffuseColor));

		memcpy(&specularColor, &matBuffer[sizeof(lambert) + sizeof(ambientColor) + sizeof(diffuseColor)], sizeof(specularColor));

		memcpy(&emmissiveColor, &matBuffer[sizeof(lambert) + sizeof(ambientColor) + sizeof(diffuseColor) + sizeof(specularColor)], sizeof(emmissiveColor));

		memcpy(&shininess, &matBuffer[sizeof(lambert) + sizeof(ambientColor) + sizeof(diffuseColor) + sizeof(specularColor) + sizeof(emmissiveColor)], sizeof(double));
	}



#pragma endregion

	#pragma region ReadMesh
	int polyVertCount = 0;
	matFile.clear();
	matFile.seekg(bSize);
	matFile.read((char*)&polyVertCount, sizeof(int));
	int mySize = polyVertCount * sizeof(simple_mesh);
	char *buffer = new char[mySize];
	matFile.read(buffer, mySize);

	if (!matFile) {
		// An error occurred!
		// myFile.gcount() returns the number of bytes read.
		// calling myFile.clear() will reset the stream state
		// so it is usable again.
		matFile.clear();
	}

	simple_mesh * meshArray = new simple_mesh[polyVertCount];
	for (int i = 0; i < polyVertCount; i++)
	{
		memcpy(&meshArray[i], &buffer[i * sizeof(simple_mesh)], sizeof(simple_mesh));
	}

#pragma endregion

	#pragma region ReadTextures

	int fileSize[3];
	char*fileBuffer = new char[sizeof(fileSize)];

	matFile.read(fileBuffer, sizeof(fileSize));
	if (!matFile) {
		// An error occurred!
		// matFile.gcount() returns the number of bytes read.
		// calling matFile.clear() will reset the stream state
		// so it is usable again.
		matFile.clear();
	}
	int sizeOfNames = 0;
	for (int i = 0; i < 3; i++)
	{
		memcpy(&fileSize[i], &fileBuffer[i * sizeof(int)], sizeof(int));
		sizeOfNames += fileSize[i];
	}

	char * textureBuffer = new char[sizeOfNames];
	fileName1 = new char[fileSize[0]];
	fileName2 = new char[fileSize[1]];
	fileName3 = new char[fileSize[2]];
	
	//matFile.seekg(sizeof(fileSize), std::ios_base::cur);
	matFile.read(fileName1, fileSize[0]);
	
	//matFile.seekg(fileSize[0], std::ios_base::cur);
	matFile.read(fileName2, fileSize[1]);
	
	//matFile.seekg(fileSize[1], std::ios_base::cur);
	matFile.read(fileName3, fileSize[2]);

	matFile.close();

#pragma endregion

	#pragma region CreateTexturesFromFile
	wchar_t fn1[260];
	wchar_t fn2[260];
	wchar_t fn3[260];
	size_t result = 0;
	mbstate_t d;
	/*
		size_t * numberOfCharactersConverted,
		wchar_t * wchar_tFileName,
		size_t countOfWordsInCharArray,
		const char ** charArrayFileName,
		size_t numberOfBytesInCharArrayFileName,
		mbstate_t * mbstate
	*/
	//const char * thing;
	//mbstate_t *d = nullptr;
	mbsrtowcs_s(&result, fn1, 260, (const char **)(&fileName1), fileSize[0], &d);
	mbsrtowcs_s(&result, fn2, 260, (const char **)(&fileName2), fileSize[1], &d);
	mbsrtowcs_s(&result, fn3, 260, (const char **)(&fileName3), fileSize[2], &d);

	//mbstowcs(fn1, fileName1, fileSize[0]);
	//mbstowcs(fn2, fileName2, fileSize[1]);
	//mbstowcs(fn3, fileName3, fileSize[2]);

	ID3D11Resource * diffuseTexture;
	ID3D11Resource * specularTexture;
	ID3D11Resource * emissiveTexture;
	//result = CreateWICTextureFromFile(dev, fn1, &diffuseTexture, nullptr, NULL);
	//result = CreateWICTextureFromFile(dev, fn2, &specularTexture, nullptr, NULL);
	//result = CreateWICTextureFromFile(dev, fn3, &emissiveTexture, nullptr, NULL);

#pragma endregion

	world->mask[entity] = COMPONENT_MESH;

	//memcpy(&world->model[entity].ambientColor,   &ambientColor, sizeof(ambientColor));
	//memcpy(&world->model[entity].diffuseColor,   &diffuseColor, sizeof(diffuseColor));
	//memcpy(&world->model[entity].specularColor,  &specularColor, sizeof(specularColor));
	//memcpy(&world->model[entity].emmissiveColor, &emmissiveColor, sizeof(emmissiveColor));

	////world->model[entity].transparencyFactor = transparencyFactor;
	//world->model[entity].shininess = shininess;
	////world->model[entity].reflectionFactor = reflectionFactor;
	
	world->mesh[entity].vertexCount = polyVertCount;
	world->mesh[entity].vertexBufferStride = sizeof(simple_mesh);
	world->mesh[entity].vertexBufferOffset = 0;
	world->mesh[entity].vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	world->mesh[entity].vertexBufferDesc.ByteWidth = sizeof(simple_mesh) * world->mesh[entity].vertexCount;
	world->mesh[entity].vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	world->mesh[entity].vertexBufferDesc.CPUAccessFlags = 0;
	world->mesh[entity].vertexBufferDesc.MiscFlags = 0;
	world->mesh[entity].vertexBufferDesc.StructureByteStride = 0;

	world->mesh[entity].vertexData.pSysMem = meshArray;
	world->mesh[entity].vertexData.SysMemPitch = 0;
	world->mesh[entity].vertexData.SysMemSlicePitch = 0;

	//dev->CreateShaderResourceView(diffuseTexture,   NULL, &world->mesh[entity].shaderResourceView1);
	//dev->CreateShaderResourceView(specularTexture,  NULL, &world->mesh[entity].shaderResourceView2);
	//dev->CreateShaderResourceView(emissiveTexture,	NULL, &world->mesh[entity].shaderResourceView3);

	//world->model[entity].anim = myAnim;
	//world->model[entity].inverseBindPoseJointMatrices = invBindPosesForJoints;
	return 0;
}