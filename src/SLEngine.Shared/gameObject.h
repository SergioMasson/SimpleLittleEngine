#pragma once

#include "math/transform.h"

namespace sle
{
	class Component;
	class BehaviourComponent;
	class GameObject;

	namespace graphics
	{
		class MeshRenderer;
		class Material;
		struct MeshData;
	}

	namespace physics
	{
		class RigidBody;
	}

	extern std::set<BehaviourComponent*> g_activeBehaviours;
	extern std::set<GameObject*> g_activeGameObjects;

	class  GameObject
	{
	public:
		GameObject(math::Vector3 position = math::Vector3(0, 0, 0), math::Quaternion rotation = math::Quaternion(0, 0, 0), GameObject* parent = nullptr, std::wstring name = L"Default");
		~GameObject();
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = delete;

		template<typename T, typename ...Param> T* AddComponent(Param... params)
		{
			T* component = new T(*this, std::forward<Param>(params)...);

			BehaviourComponent* behaviur = dynamic_cast<BehaviourComponent*>(component);

			if (behaviur != nullptr)
				m_behaviourComponents.insert(behaviur);

			m_components.insert(component);
			return component;
		}

		template<class T> T* GetComponent()
		{
			for (Component* component : m_components)
			{
				T* target = dynamic_cast<T*>(component);

				if (target != nullptr)
					return target;
			}

			return nullptr;
		}
		template<class T> void RemoveComponent()
		{
			for (Component* component : m_components)
			{
				T* target = dynamic_cast<T*>(component);

				if (target != nullptr)
				{
					m_components.erase(target);
					delete target;
					return;
				}
			}
		}

		bool IsActive() { return m_isActive; }
		void SetActive(bool active) { m_isActive = active; }

		math::Vector3 GetLocalPosition() const;
		math::Vector3 GetPosition() const;

		math::Quaternion GetLocalRotation() const;
		math::Quaternion GetRotation() const;

		math::Transform GetTransform() const;
		math::Transform GetLocalTransform() const;

		void SetName(std::wstring name) { m_name = name; }
		std::wstring const& GetName() const { return m_name; };

		void SetParent(GameObject* parent);
		void SetPosition(math::Vector3 position);
		void SetRotation(math::Quaternion rotation);
		void SetScale(math::Vector3 scale);

		void NotifyCollision(physics::RigidBody* other);

		graphics::MeshRenderer* AddMeshRenderer(graphics::MeshData* data, graphics::Material material);
		graphics::MeshRenderer* GetMeshRenderer()
		{
			return m_meshRenderer;
		}

		bool HasRenderer() {
			return m_meshRenderer != nullptr;
		}

	private:
		math::Transform m_transform;
		graphics::MeshRenderer* m_meshRenderer;
		std::wstring m_name;
		GameObject* m_parent;
		std::set<Component*> m_components;
		std::set<BehaviourComponent*> m_behaviourComponents;
		bool m_isActive;
	};

	class  Component
	{
	public:
		GameObject& GetGameObject() { return m_gameObject; }
		std::wstring GetName() const { return m_gameObject.GetName(); };

	protected:
		Component(GameObject& gameObject) : m_gameObject{ gameObject }
		{
			m_active = true;
		}

		virtual ~Component() {}
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;

	protected:
		GameObject& m_gameObject;
		bool m_active;
		friend GameObject;
	};

	class  BehaviourComponent : public Component
	{
	public:
		BehaviourComponent(GameObject& gameObject) : Component(gameObject)
		{
			g_activeBehaviours.insert(this);
		};

		virtual void Update(float daltaT) {};
		virtual void FixedUpdate(float deltaT) {};
		virtual void OnCollision(physics::RigidBody* other) {};
	};
}