#include "pch.h"
#include "gameObject.h"
#include "graphics/meshRenderer.h"

using namespace sle;

std::set<BehaviourComponent*> sle::g_activeBehaviours;
std::set<sle::GameObject*> sle::g_activeGameObjects;

sle::GameObject::GameObject(math::Vector3 position, math::Quaternion rotation, GameObject* parent, std::wstring name) :
	m_transform{ rotation, position },
	m_meshRenderer{ nullptr },
	m_parent{ parent },
	m_isActive{ true },
	m_name{ name }
{
	g_activeGameObjects.insert(this);
}

sle::GameObject::~GameObject()
{
	for (Component* component : m_components)
		delete component;

	if (m_meshRenderer != nullptr)
		delete m_meshRenderer;

	g_activeGameObjects.erase(this);
}

math::Vector3 sle::GameObject::GetLocalPosition() const
{
	return m_transform.GetTranslation();
}

math::Vector3 sle::GameObject::GetPosition() const
{
	if (m_parent != nullptr)
		return m_parent->GetTransform() * m_transform.GetTranslation();

	return m_transform.GetTranslation();
}

math::Quaternion sle::GameObject::GetLocalRotation() const
{
	return m_transform.GetRotation();
}

math::Quaternion GameObject::GetRotation() const
{
	if (m_parent != nullptr)
		return m_parent->GetRotation() * m_transform.GetRotation();

	return m_transform.GetRotation();
}

math::Transform GameObject::GetLocalTransform() const
{
	return m_transform;
}

void GameObject::SetParent(GameObject* parent)
{
	m_parent = parent;
}

void GameObject::SetPosition(math::Vector3 position)
{
	m_transform.SetTranslation(position);
}

void GameObject::SetRotation(math::Quaternion rotation)
{
	m_transform.SetRotation(rotation);
}

void GameObject::SetScale(math::Vector3 scale)
{
	m_transform.SetScale(scale);
}

void GameObject::NotifyCollision(physics::RigidBody* other)
{
	for (BehaviourComponent* component : m_behaviourComponents)
		component->OnCollision(other);
}

graphics::MeshRenderer* GameObject::AddMeshRenderer(graphics::MeshData* data, graphics::Material material)
{
	m_meshRenderer = new graphics::MeshRenderer(*this, data, material);
	return m_meshRenderer;
}

math::Transform GameObject::GetTransform() const
{
	if (m_parent != nullptr)
		return m_parent->GetTransform() * m_transform;

	return m_transform;
}