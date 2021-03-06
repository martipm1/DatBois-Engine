#ifndef __GAMEOBJECT_H__
#define __GMAEOBJECT_H__

#include "Globals.h"
#include "MathGeoLib\Math\float3.h"
#include "MathGeoLib\Math\Quat.h"
#include "MathGeoLib\Geometry\AABB.h"
#include <vector>
class Component;
class MeshComponent;
struct Mesh_str;
enum component_type;

class GameObject
{
	public:
		GameObject(std::string _name, GameObject* _parent = nullptr);
		~GameObject();

		void Update();
		void Delete();
		GameObject* GetParent();
		std::vector<GameObject*> GetChilds();
		void SetActive(bool act);
		//add Mesh component
		Component* AddComponent(component_type type, Mesh_str mesh, GameObject* _parent);
		//add Transform component
		Component* AddComponent(component_type type, float3 pos, float3 scale, Quat rot, GameObject* _parent);
		//add Material component
		Component* AddComponent(component_type type, size_t id, GameObject* _parent);
		Component* FindComponent(component_type type);
	
		GameObject* parent;
		std::string name;
		std::vector<Component*> components;
		std::vector<GameObject*> childs;
		AABB bbox;
		bool active;
};

#endif // __GAMEOBJECT_H__

