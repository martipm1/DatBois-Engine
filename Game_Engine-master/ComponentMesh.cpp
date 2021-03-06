#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ModuleMesh.h"
#include "Component.h"
#include "GameObject.h"
#include "Imgui\imgui.h"
//Geometry stuff, shouldn't be here ONLY TESTING!
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") 
//Geometry stuff, shouldn't be here

#include "MathGeoLib\Math\float4x4.h"

ComponentMesh::ComponentMesh(component_type type, Mesh_str _mesh, GameObject* _parent) : Component(type, "Mesh", _parent)
{
	parent = _parent;
	mesh = _mesh;
	active = true;
	bbox.SetNegativeInfinity();
	bbox.Enclose((float3*)mesh.vertices, mesh.num_vertices);
}

ComponentMesh::~ComponentMesh()
{}

void ComponentMesh::Update()
{
	if (active)
	{
		ComponentTransform* trans = (ComponentTransform*)parent->FindComponent(c_transform);
		if (trans)
		{
			float4x4 matrix = trans->GetRenderingMatrix().Transposed();
			glPushMatrix();
			glMultMatrixf(*matrix.v);
		}

		ComponentMaterial* material = (ComponentMaterial*)parent->FindComponent(c_material);
		//TODO: Use texture id to paint on the mesh!!!
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_uvs);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		if (material != nullptr)
		{
			if (material->active == true)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, material->texture_id);
			}
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
		glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, NULL);

		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		if (trans)
			glPopMatrix();

		static float3 corners[8];
		bbox.GetCornerPoints(corners);

		glPushMatrix();
		glMultMatrixf((GLfloat*)trans->GetRenderingMatrix().Transposed().v);

		glColor3f(255, 255, 0);

		glBegin(GL_LINE_STRIP);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy, sz);
		glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy, sz);
		glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx,  sy, sz);
		glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy, sz);

		glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);
		glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx,  sy, -sz);

		glVertex3fv((GLfloat*)&corners[5]); //glVertex3f(sx, -sy,  sz);
		glVertex3fv((GLfloat*)&corners[4]); //glVertex3f(sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[6]); //glVertex3f(sx,  sy, -sz);
		glVertex3fv((GLfloat*)&corners[7]); //glVertex3f(sx,  sy,  sz);

		glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);
		glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy,  sz);
		glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);

		glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx, sy,  sz);
		glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx, sy,  sz);
		glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx, sy, -sz);
		glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx, sy, -sz);

		glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
		glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy,  sz);
		glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);

		glColor3f(1.0f, 1.0f, 1.0f);
		glEnd();

		glPopMatrix();
	}
}

void ComponentMesh::DeleteComponent()
{
	parent = nullptr;
}

void ComponentMesh::OnProperties()
{
	if (ImGui::CollapsingHeader("GameObject Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool active_state = active;
		if (ImGui::Checkbox("MeshIsActive", &active_state))
		{
			SetActive(active_state);
		}

		ImGui::TextColored(ImVec4(255, 255, 255, 255), "Number of vertices: %u", mesh.num_vertices);
		ImGui::TextColored(ImVec4(255, 255, 255, 255), "Number of indices: %u", mesh.num_indices);
		ImGui::TextColored(ImVec4(255, 255, 255, 255), "Number of normals: %u", mesh.num_normals);
		ImGui::TextColored(ImVec4(255, 255, 255, 255), "Number of UVs: %u", mesh.num_uvs);
	}
}