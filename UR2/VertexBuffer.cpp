#define _CRT_SECURE_NO_WARNINGS  //允许使用 wcstombs
#include "VertexBuffer.h"
#include <fbxsdk.h>
#include <assert.h>

void VertexBuffer::VertexInfo::swap(VertexInfo& other)
{
	XMFLOAT3 temp3;
	XMFLOAT2 temp2;

	temp3 = other.position;
	other.position = position;
	position = temp3;

	temp3 = other.normal;
	other.normal = normal;
	normal = temp3;

	temp3 = other.tangent;
	other.tangent = tangent;
	tangent = temp3;
	
	temp2 = other.texcoord;
	other.texcoord = texcoord;
	texcoord = temp2;
}


VertexBuffer::VertexBuffer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:Bindable(pDevice, pContext)
{

}

VertexBuffer::VertexBuffer(const VertexBuffer&& vb)
	:Bindable{vb.GetDevice(), vb.GetContext()}
{
	pVertexBuffer = vb.pVertexBuffer;
	vertexCount = vb.vertexCount;
}

void VertexBuffer::Load(wstring fileName_FBX)
{
	//wchar_t文件名转换为char
	char name[40]{};

	wcstombs(name, fileName_FBX.c_str(), fileName_FBX.size());

	//准备读取文件
	assert(fileName_FBX.empty() == false && L"VertexBuffer读入的FBX文件为空");
	vector<VertexInfo> vertexBuffer;

	FbxManager* g_pFbxSdkManager = nullptr;
	if (g_pFbxSdkManager == nullptr)
	{
		g_pFbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");

	bool bSuccess = pImporter->Initialize(name, -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess)
	{
		assert(false && L"初始化失败，找不到文件");
		return;
	}

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess)
	{
		assert(false && L"导入失败");
		return;
	}

	pImporter->Destroy();

	//遍历文件Node
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)		//迭代 文件 Node
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = pFbxChildNode->GetMesh();				//拿到 类型为mesh的 子节点

			FbxVector4* pVertices = pMesh->GetControlPoints();//控制点就是顶点
// 			FbxArray<FbxVector4> pNormal;
// 			pMesh->GetPolygonVertexNormals(pNormal);
//			FbxArray<FbxVector2> pUVs;
			FbxStringList UVSetName;
			pMesh->GetUVSetNames(UVSetName);
			const char* UVset1Name = (const char*)UVSetName[0];
//			pMesh->GetPolygonVertexUVs(NULL, pUVs, NULL);

			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				int iNumVertices = pMesh->GetPolygonSize(j);
				assert(iNumVertices == 3 && L"某个Polygone的 控制点 不是三个");

				for (int k = 0; k < iNumVertices; k++)	//遍历一个面的三个控制点，获取position， normal，tangent，uv信息
				{
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

					FbxVector4 theVertexNormal;
					pMesh->GetPolygonVertexNormal(j, k, theVertexNormal);

					FbxVector4 tangent{};
					{//获取切线
						if (pMesh->GetElementTangentCount() < 1)
						{
							return;
						}

						fbxsdk::FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(0);

						switch (leTangent->GetMappingMode())
						{
						case fbxsdk::FbxGeometryElement::EMappingMode::eByControlPoint:
						{
							switch (leTangent->GetReferenceMode())
							{
							case fbxsdk::FbxGeometryElement::EReferenceMode::eDirect:
							{
								tangent = leTangent->GetDirectArray().GetAt(iControlPointIndex);
							}
							break;

							case fbxsdk::FbxGeometryElement::EReferenceMode::eIndexToDirect:
							{
								int id = leTangent->GetIndexArray().GetAt(iControlPointIndex);
								tangent = leTangent->GetDirectArray().GetAt(id);
							}
							break;

							default:
								break;
							}
						}
						break;

						case fbxsdk::FbxGeometryElement::EMappingMode::eByPolygonVertex:
						{
							switch (leTangent->GetReferenceMode())
							{
							case fbxsdk::FbxGeometryElement::EReferenceMode::eDirect:
							{
								tangent = leTangent->GetDirectArray().GetAt(iControlPointIndex);
							}
							break;

							case fbxsdk::FbxGeometryElement::EReferenceMode::eIndexToDirect:
							{
								int id = leTangent->GetIndexArray().GetAt(iControlPointIndex);
								tangent = leTangent->GetDirectArray().GetAt(id);
							}
							break;

							default:
								break;
							}
						}
						break;
						}


					}

					FbxVector2 theUV;
					bool isUnmapped;
					pMesh->GetPolygonVertexUV(j, k, UVset1Name, theUV, isUnmapped);


					VertexInfo oneVertexInfo;
					oneVertexInfo.position.x = pVertices[iControlPointIndex][0];
					oneVertexInfo.position.y = pVertices[iControlPointIndex][1];
					oneVertexInfo.position.z = pVertices[iControlPointIndex][2];

					oneVertexInfo.normal.x = theVertexNormal[0] * REVERSE_NORMAL;
					oneVertexInfo.normal.y = theVertexNormal[1] * REVERSE_NORMAL;
					oneVertexInfo.normal.z = theVertexNormal[2] * REVERSE_NORMAL;
					oneVertexInfo.tangent.x = tangent[0];
					oneVertexInfo.tangent.y = tangent[1];
					oneVertexInfo.tangent.z = tangent[2];

					if (isUnmapped)//该顶点 没有映射UV；
					{
						oneVertexInfo.texcoord.x = 0;
						oneVertexInfo.texcoord.y = 0;
					}
					else
					{
						oneVertexInfo.texcoord.x = theUV[0];
						oneVertexInfo.texcoord.y = theUV[1];
					}
					vertexBuffer.push_back(oneVertexInfo);
				}
#if CHANGE_Clockwise
				vertexBuffer.rbegin()->swap(*(++vertexBuffer.rbegin()));//最后一个与倒数第二个交换
#endif
			}
		}
	}

	D3D11_BUFFER_DESC bufDesc{};
	bufDesc.ByteWidth = sizeof(VertexInfo) * vertexBuffer.size();
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0u;
	bufDesc.MiscFlags = 0u;
	bufDesc.StructureByteStride = sizeof(VertexInfo);
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA initiaVertexBuffer{};
	initiaVertexBuffer.pSysMem = vertexBuffer.data();
	pDevice->CreateBuffer(&bufDesc, &initiaVertexBuffer, &pVertexBuffer);

	vertexCount = vertexBuffer.size();
}

void VertexBuffer::Bind()
{
	if (pVertexBuffer == nullptr)
	{
		return;
	}
	else
	{
		UINT strides[1]{ sizeof(VertexInfo) };
		UINT offsets[1]{ 0u };

		pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), strides, offsets);
		return;
	}
}

void VertexBuffer::Draw()
{
	this->Bind();
	pContext->Draw(vertexCount, 0u);
}

UINT VertexBuffer::GetVertexCount()
{
	return vertexCount;
}

