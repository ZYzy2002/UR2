#define _CRT_SECURE_NO_WARNINGS  //����ʹ�� wcstombs
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
	//wchar_t�ļ���ת��Ϊchar
	char name[40]{};

	wcstombs(name, fileName_FBX.c_str(), fileName_FBX.size());

	//׼����ȡ�ļ�
	assert(fileName_FBX.empty() == false && L"VertexBuffer�����FBX�ļ�Ϊ��");
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
		assert(false && L"��ʼ��ʧ�ܣ��Ҳ����ļ�");
		return;
	}

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess)
	{
		assert(false && L"����ʧ��");
		return;
	}

	pImporter->Destroy();

	//�����ļ�Node
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)		//���� �ļ� Node
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = pFbxChildNode->GetMesh();				//�õ� ����Ϊmesh�� �ӽڵ�

			FbxVector4* pVertices = pMesh->GetControlPoints();//���Ƶ���Ƕ���
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
				assert(iNumVertices == 3 && L"ĳ��Polygone�� ���Ƶ� ��������");

				for (int k = 0; k < iNumVertices; k++)	//����һ������������Ƶ㣬��ȡposition�� normal��tangent��uv��Ϣ
				{
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

					FbxVector4 theVertexNormal;
					pMesh->GetPolygonVertexNormal(j, k, theVertexNormal);

					FbxVector4 tangent{};
					{//��ȡ����
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

					if (isUnmapped)//�ö��� û��ӳ��UV��
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
				vertexBuffer.rbegin()->swap(*(++vertexBuffer.rbegin()));//���һ���뵹���ڶ�������
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

