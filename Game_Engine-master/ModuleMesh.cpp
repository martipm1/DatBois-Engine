#include "Application.h"
#include "ModuleMesh.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Component.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "Glew\include\glew.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib")

using namespace std;


ModuleMesh::ModuleMesh(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleMesh::~ModuleMesh()
{}

bool ModuleMesh::Init()
{
	//Use console to LOG debug stuff from assimp
	//struct aiLogStrem stream;
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullprt);
	//aiAttachLogStream(&stream);

	return true;
}

bool ModuleMesh::CleanUp()
{
	//Stop debbuging through the console
	//aiDetachAllLogStreams();

	return true;
}

vector<Mesh_str> ModuleMesh::LoadMesh(const char* path)
{
	vector<Mesh_str> full_mesh;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		//USE NODES TO ITERATE ALL THE SCENE
		//if(scene->mRootNode->)
		GameObject* object = App->go_manager->CreateGameObject("GameObject", nullptr);
		//
		////Getting the transformation
		//aiVector3D translation;
		//aiVector3D scaling;
		//aiQuaternion rotation;
		//
		//scene->mRootNode->mTransformation.Decompose(scaling, rotation, translation);
		//
		//float3 pos(translation.x, translation.y, translation.z);
		//float3 scale(scaling.x, scaling.y, scaling.z);
		//Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
		//
		////Send everything to the GameObject
		//object->AddComponent(c_transform, pos, scale, rot);

		//Getting the mesh
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh_to_load = scene->mMeshes[i];
			Mesh_str* mesh = new Mesh_str();

			//VERTICES
			mesh->num_vertices = mesh_to_load->mNumVertices;
			mesh->vertices = new uint[mesh->num_vertices * 3];
			memcpy(mesh->vertices, mesh_to_load->mVertices, sizeof(float)*mesh->num_vertices * 3);
			
			glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->vertices, GL_STATIC_DRAW);

			//INDICES
			if (mesh_to_load->HasFaces())
			{
				mesh->num_indices = mesh_to_load->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices];
				for (uint j = 0; j < mesh_to_load->mNumFaces; j++)
				{
					if (mesh_to_load->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING: geometry with indices != 3 found");
					}
					else
					{
						memcpy(&mesh->indices[j * 3], mesh_to_load->mFaces[j].mIndices, 3 * sizeof(uint));
					}
						
				}
			}

			glGenBuffers(1, (GLuint*)&(mesh->id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

			object->AddComponent(c_mesh, mesh);
		}
		//Mesh complete! Send it to GameObject as a Mesh Component

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	return full_mesh;
}