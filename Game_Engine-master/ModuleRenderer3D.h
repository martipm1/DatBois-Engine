#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include "Light.h"
//#include "MathGeoLib\Math\float4x4.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	//void RenderMesh(Mesh_str* mesh, float4x4 matrix);
	//void RenderMesh(Mesh_str* mesh, float4x4 matrix, uint texture_id);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};