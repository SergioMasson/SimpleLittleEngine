#pragma once

#include "graphics/meshRenderer.h"
#include "gameObject.h"

class Enemy;
extern std::set<Enemy*> g_activeEnemies;

class Enemy : public sle::BehaviourComponent
{
private:
	Enemy(sle::GameObject& gameObject, sle::graphics::Texture2D texture) :
		sle::BehaviourComponent{ gameObject },
		m_detectedTexture{ texture },
		m_isDetected{ false }
	{
		g_activeEnemies.insert(this);
	}

	~Enemy()
	{
		g_activeEnemies.erase(this);
	}

public:
	void SetDetected();
	bool IsDetected() { return m_isDetected; }
	void OnCollision(sle::physics::RigidBody* other) override;
	void FixedUpdate(float detalT) override;
	sle::math::Vector3 GetPosition() { return m_gameObject.GetPosition(); }
	sle::math::BoudingBox WBoudingBox() const { return m_gameObject.GetMeshRenderer()->WBoudingBox(); }

private:
	sle::graphics::Texture2D m_detectedTexture;
	bool m_isDetected;

	friend sle::GameObject;
};
