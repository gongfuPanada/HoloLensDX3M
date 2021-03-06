#include "pch.h"
#include "Model.h"

#include "Mesh.h"
#include "ModelMaterial.h"

#include "MatrixHelper.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <windows.graphics.directx.direct3d11.interop.h>



using namespace Platform;

namespace HolographicDXMatModMeshLibrary
{
	Model::Model(HolographicSpace ^ holographicSpace, Platform::String ^ filename, bool flipUVs = false) : mHolographicSpace(holographicSpace)
	{
		mMeshes = ref new Vector<Mesh^>();
		mMaterials = ref new Vector<ModelMaterial^>();

		mDeviceResources = std::make_shared<DX::DeviceResources>();
		mDeviceResources->SetHolographicSpace(mHolographicSpace);

		Assimp::Importer importer;

		UINT flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder;
		if (flipUVs)
		{
			flags |= aiProcess_FlipUVs;
		}


		std::wstring wfilename(filename->Begin());
		std::string s_filename(wfilename.begin(), wfilename.end());


		const aiScene* scene = importer.ReadFile(s_filename, flags);
		if (scene == nullptr)
		{
			OutputDebugStringA(importer.GetErrorString());
			OutputDebugString(L"\n");
		}

		if (scene->HasMaterials())
		{
			for (UINT i = 0; i < scene->mNumMaterials; i++)
			{
				mMaterials->Append(ref new ModelMaterial(this, scene->mMaterials[i]));
			}
		}

		if (scene->HasMeshes())
		{
			for (UINT i = 0; i < scene->mNumMeshes; i++)
			{
				aiMesh  _aimesh = *(scene->mMeshes[i]);
				
				Mesh^ mesh = ref new Mesh(this, _aimesh);
				mMeshes->Append(mesh);
			}
		}
	}

	std::shared_ptr<DX::DeviceResources> Model::GetDeviceResources()
	{
		return mDeviceResources;
	}

	//Model::~Model()
	//{
	//	for (Mesh^ mesh : mMeshes)
	//	{
	//		//delete mesh;
	//		mesh = nullptr;
	//	}

	//	for (ModelMaterial^ material : mMaterials)
	//	{
	//		//delete material;
	//		material = nullptr;
	//	}
	//}




}