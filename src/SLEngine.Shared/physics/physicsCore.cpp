#include "pch.h"
#include <map>
#include <thread>
#include "../dxassert.h"
#include "physicsCore.h"
#include "physx/PxPhysicsAPI.h"
#include "rigidBody.h"
#include "collider.h"

using namespace physx;

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;

static PxFoundation* g_Foundation = nullptr;
static PxPhysics* g_Physics = nullptr;
static PxPvd* g_debugger = nullptr;
static PxScene* g_scene = nullptr;
static PxAggregate* g_aggregate = nullptr;

#define PVD_HOST "127.0.0.1"
#define PVD_PORT 5425

#define SELF_COLLISION false
#define MAX_COLLISION 100

static std::map<physics::RigidBody*, PxRigidActor*> g_activeRigidBodies{};
static std::map<physics::Collider*, PxShape*> g_activeColliders{};

static bool isFirstDelta = true;

PxVec3 GetPXVec(math::Vector3 const& vec)
{
	DirectX::XMFLOAT3 vecL = vec;
	return PxVec3(vecL.x, vecL.y, vecL.z);
}

PxQuat GetPXQuat(math::Quaternion const& quat)
{
	DirectX::XMFLOAT4 vecL = quat;
	return PxQuat(vecL.x, vecL.y, vecL.z, vecL.w);
}

math::Vector3 GetVec(PxVec3 vec)
{
	return math::Vector3(vec.x, vec.y, vec.z);
}

void CreateScene()
{
	PxSceneDesc sceneDesc(g_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -30.0f, 0);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	const auto processor_count = std::thread::hardware_concurrency();

	// create CPU dispatcher which mNbThreads worker threads
	auto mCpuDispatcher = PxDefaultCpuDispatcherCreate(processor_count - 1);
	ASSERT(mCpuDispatcher, "PxDefaultCpuDispatcherCreate failed!");
	sceneDesc.cpuDispatcher = mCpuDispatcher;

	g_scene = g_Physics->createScene(sceneDesc);

	ASSERT(g_scene, "Fail to create PxScene");
}

PxRigidDynamic* CreateRigbodyCapsule(PxVec3 position, float radius, float halfHeight)
{
	PxRigidDynamic* aCapsuleActor = g_Physics->createRigidDynamic(PxTransform(position));

	PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
	PxMaterial* aMaterial = g_Physics->createMaterial(0, 0, 0);

	PxShape* aCapsuleShape = PxRigidActorExt::createExclusiveShape(*aCapsuleActor, PxCapsuleGeometry(radius, halfHeight), *aMaterial);
	aCapsuleShape->setLocalPose(relativePose);
	PxRigidBodyExt::updateMassAndInertia(*aCapsuleActor, 1);
	g_scene->addActor(*aCapsuleActor);

	return aCapsuleActor;
}

void physics::Initialize()
{
	g_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	ASSERT(g_Foundation, "Fail to initalize PxFoundation");

	g_debugger = PxCreatePvd(*g_Foundation);

#if defined(DEBUG) || defined(_DEBUG)
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, PVD_PORT, 10);
	g_debugger->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif

	g_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_Foundation, PxTolerancesScale(), false, g_debugger);

	ASSERT(g_Physics, "Fail to create PxPhysics");

	CreateScene();

	g_aggregate = g_Physics->createAggregate(MAX_COLLISION, SELF_COLLISION);
	ASSERT(g_aggregate, "Fail to create aggregate");

	g_scene->addAggregate(*g_aggregate);
}

void physics::Shutdown()
{
	if (g_debugger != nullptr)
		g_debugger->release();

	if (g_Foundation != nullptr)
		g_Foundation->release();

	if (g_Physics != nullptr)
		g_Physics->release();
}

void physics::Update(float deltaT)
{
	if (isFirstDelta)
	{
		isFirstDelta = false;
		return;
	}

	for (auto keyValuePair : g_activeRigidBodies)
	{
		PxRigidDynamic* pxRigidBody = keyValuePair.second->is<PxRigidDynamic>();

		if (pxRigidBody == nullptr)
			continue;

		RigidBody* rigidBody = keyValuePair.first;

		if (rigidBody->m_hasTarget)
		{
			PxTransform transform{};
			transform.p = GetPXVec(rigidBody->m_targetPositon);
			transform.q = GetPXQuat(rigidBody->m_targetRotation);
			pxRigidBody->setKinematicTarget(transform);
			rigidBody->m_hasTarget = false;
		}
	}

	g_scene->simulate(deltaT);
	g_scene->fetchResults(true);

	/*PxU32 size;
	PxActor** activeActors = g_scene->getActiveActors(size);

	for (PxU32 i = 0; i < size; i++)
	{
		PxActor* actor = activeActors[i];
		PxRigidDynamic* pxRigidBody = dynamic_cast<PxRigidDynamic*>(actor);

		if (pxRigidBody != nullptr)
		{
			GameObject* go = static_cast<GameObject*>(pxRigidBody->userData);
			PxTransform transform = pxRigidBody->getGlobalPose();
			PxVec3 position = transform.p;
			PxQuat rotation = transform.q;

			go->SetPosition(math::Vector3(position.x, position.y, position.z));
			go->SetRotation(math::Quaternion(rotation.x, rotation.y, rotation.z, rotation.w));
		}
	}*/

	for (auto keyValuePair : g_activeRigidBodies)
	{
		PxRigidActor* pxRigidBody = keyValuePair.second;
		GameObject* go = keyValuePair.first->GetGameObject();

		PxTransform transform = pxRigidBody->getGlobalPose();
		PxVec3 position = transform.p;
		PxQuat rotation = transform.q;

		go->SetPosition(math::Vector3(position.x, position.y, position.z));
		go->SetRotation(math::Quaternion(rotation.x, rotation.y, rotation.z, rotation.w));
	}
}

void physics::CreateRigidBody(RigidBody* rigidBody, math::Vector3 position, math::Quaternion rotation, float inialMass, bool isKinematic, bool isStatic)
{
	DirectX::XMFLOAT3 positionL = position;
	DirectX::XMFLOAT4 rotationL = rotation;

	PxTransform actorTransform = PxTransform(GetPXVec(position), GetPXQuat(rotation));

	PxRigidActor* actor;

	if (isStatic)
	{
		actor = g_Physics->createRigidStatic(actorTransform);
	}
	else
	{
		PxRigidDynamic* dynamicActor = g_Physics->createRigidDynamic(actorTransform);
		PxRigidBodyExt::updateMassAndInertia(*dynamicActor, inialMass);
		dynamicActor->setMaxLinearVelocity(100.0f);
		dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::Enum::eKINEMATIC, isKinematic);
		actor = dynamic_cast<PxRigidActor*>(dynamicActor);
	}

	actor->userData = rigidBody;
	g_scene->addActor(*actor);
	g_activeRigidBodies.insert({ rigidBody, actor });
	g_aggregate->addActor(*actor);
}

void physics::RigidBodySetMass(RigidBody* rigidBody, float mass)
{
	auto actor = g_activeRigidBodies[rigidBody];

	auto dynamicActor = actor->is<PxRigidDynamic>();

	ASSERT(dynamicActor, "RigidBodySetMass: Actor is not a dynamic actor.");

	if (dynamicActor != nullptr)
		PxRigidBodyExt::updateMassAndInertia(*dynamicActor, mass);
}

void physics::RigidBodySetVelocity(RigidBody* rigidBody, math::Vector3 velocity)
{
	auto actor = g_activeRigidBodies[rigidBody];

	auto dynamicActor = actor->is<PxRigidDynamic>();

	ASSERT(dynamicActor, "RigidBodySetVelocity: Actor is not a dynamic actor.");

	if (dynamicActor != nullptr)
		dynamicActor->setLinearVelocity(GetPXVec(velocity));
}

void physics::RigidBodyAddVelocity(RigidBody* rigidBody, math::Vector3 velocity)
{
	auto actor = g_activeRigidBodies[rigidBody];

	auto dynamicActor = actor->is<PxRigidDynamic>();

	ASSERT(dynamicActor, "RigidBodySetVelocity: Actor is not a dynamic actor.");

	if (dynamicActor != nullptr)
	{
		auto extraV = GetPXVec(velocity);
		auto currentV = dynamicActor->getLinearVelocity();
		dynamicActor->setLinearVelocity(currentV + extraV);
	}
}

void physics::RigidBodyAddVelocity(RigidBody* rigidBody, math::Vector3 velocity, float maxModule)
{
	auto actor = g_activeRigidBodies[rigidBody];

	auto dynamicActor = actor->is<PxRigidDynamic>();

	ASSERT(dynamicActor, "RigidBodySetVelocity: Actor is not a dynamic actor.");

	if (dynamicActor != nullptr)
	{
		auto extraV = GetPXVec(velocity);
		auto currentV = dynamicActor->getLinearVelocity();
		auto finalV = currentV + extraV;

		if (finalV.magnitudeSquared() <= (maxModule * maxModule))
			dynamicActor->setLinearVelocity(currentV + extraV);
	}
}

void physics::RigidBodyAddForce(RigidBody* rigidBody, math::Vector3 force)
{
	auto actor = g_activeRigidBodies[rigidBody];

	PxRigidDynamic* dynamicActor = actor->is<PxRigidDynamic>();

	ASSERT(dynamicActor, "RigidBodyAddForce: Actor is not a dynamic actor.");

	if (dynamicActor != nullptr)
		dynamicActor->addForce(GetPXVec(force));
}

void physics::RigidBodyAddTorque(RigidBody* rigidBody, math::Vector3 torque)
{
	auto actor = g_activeRigidBodies[rigidBody];

	PxRigidDynamic* dynamicActor = actor->is<PxRigidDynamic>();

	ASSERT(dynamicActor, "RigidBodyAddForce: Actor is not a dynamic actor.");

	if (dynamicActor != nullptr)
		dynamicActor->addTorque(GetPXVec(torque));
}

void physics::RigidBodySetKinematic(RigidBody* rigidBody, bool isKinematic)
{
	auto actor = g_activeRigidBodies[rigidBody];

	PxRigidDynamic* dynamicActor = actor->is<PxRigidDynamic>();

	ASSERT(dynamicActor, "RigidBodySetKinematic: Actor is not a dynamic actor.");

	if (dynamicActor != nullptr)
		dynamicActor->setRigidBodyFlag(PxRigidBodyFlag::Enum::eKINEMATIC, isKinematic);
}

void physics::RigidBodySetCollisionMask(RigidBody* rigidBody, uint32_t mask, uint32_t filter)
{
	auto actor = g_activeRigidBodies[rigidBody];

	PxFilterData filterData;
	filterData.word0 = mask;	// word0 = own ID
	filterData.word1 = filter;  // word1 = ID mask to filter pairs that trigger a
									// contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)malloc(sizeof(PxShape*) * numShapes);
	actor->getShapes(shapes, numShapes);

	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}

	free(shapes);
}

void physics::RigidBodySetPosition(RigidBody* rigidBody, math::Vector3 position)
{
	auto actor = g_activeRigidBodies[rigidBody];

	PxRigidDynamic* dynamicActor = actor->is<PxRigidDynamic>();

	ASSERT(dynamicActor, "RigidBodyAddForce: Actor is not a dynamic actor.");

	if (dynamicActor != nullptr)
	{
		PxTransform pose = dynamicActor->getGlobalPose();
		pose.p = GetPXVec(position);
		dynamicActor->setGlobalPose(pose);
	}
}

void physics::RigidBodySetRotation(RigidBody* rigidBody, math::Quaternion rotation)
{
	auto actor = g_activeRigidBodies[rigidBody];

	PxRigidDynamic* dynamicActor = actor->is<PxRigidDynamic>();

	ASSERT(dynamicActor, "RigidBodyAddForce: Actor is not a dynamic actor.");

	if (dynamicActor != nullptr)
	{
		PxTransform pose = dynamicActor->getGlobalPose();
		pose.q = GetPXQuat(rotation);
		dynamicActor->setGlobalPose(pose);
	}
}

void physics::CreateCapsuleCollider(RigidBody* rigidBody, Collider** collider, float radius, float halfHeight)
{
	PxRigidActor* actor = g_activeRigidBodies[rigidBody];

	PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
	PxMaterial* aMaterial = g_Physics->createMaterial(0, 1, 0);

	PxShape* aCapsuleShape = PxRigidActorExt::createExclusiveShape(*actor, PxCapsuleGeometry(radius, halfHeight), *aMaterial);
	aCapsuleShape->setLocalPose(relativePose);

	PxTransform transform = actor->getGlobalPose();
	aCapsuleShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	aCapsuleShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
}

void physics::RigidBodyCreateBoxCollider(RigidBody* rigidBody, float width, float height, float depth, math::Vector3 relativePositon)
{
	PxRigidActor* actor = g_activeRigidBodies[rigidBody];
	PxMaterial* aMaterial = g_Physics->createMaterial(0.1, 0.99f, 0);
	PxBoxGeometry geometry(width / 2.0f, height / 2.0f, depth / 2.0f);
	PxShape* aBoxShape = PxRigidActorExt::createExclusiveShape(*actor, geometry, *aMaterial);

	PxTransform relativePose(GetPXVec(relativePositon), PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
	aBoxShape->setLocalPose(relativePose);
	aBoxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	aBoxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
}

void physics::RigidBodyCreatePlaneCollider(RigidBody* rigidBody, math::Vector3 position, math::Quaternion rotation)
{
	PxRigidActor* actor = g_activeRigidBodies[rigidBody];
	PxMaterial* aMaterial = g_Physics->createMaterial(0, 0, 0);

	PxPlaneGeometry geometry{};

	PxShape* aPlaneShape = PxRigidActorExt::createExclusiveShape(*actor, geometry, *aMaterial);
	PxTransform relativePose{ GetPXVec(position), GetPXQuat(rotation) };
	aPlaneShape->setLocalPose(relativePose);
}

void physics::RigidBodyCreatePlaneCollider(RigidBody* rigidBody, float x, float y, float z, float w)
{
	PxRigidActor* actor = g_activeRigidBodies[rigidBody];
	PxMaterial* aMaterial = g_Physics->createMaterial(0, 0, 0);

	PxPlaneGeometry geometry{};

	PxShape* aPlaneShape = PxRigidActorExt::createExclusiveShape(*actor, geometry, *aMaterial);
	PxPlane planeEquation{ x, y, z, w };
	PxTransform relativePose = PxTransformFromPlaneEquation(planeEquation);

	aPlaneShape->setLocalPose(relativePose);
}

void physics::RigidBodyCreatePlaneCollider(RigidBody* rigidBody, math::Vector3 position, float distance)
{
	PxRigidActor* actor = g_activeRigidBodies[rigidBody];
	PxMaterial* aMaterial = g_Physics->createMaterial(0, 0, 0);

	PxPlaneGeometry geometry{};

	PxShape* aPlaneShape = PxRigidActorExt::createExclusiveShape(*actor, geometry, *aMaterial);
	PxPlane planeEquation{ GetPXVec(position), distance };
	PxTransform relativePose = PxTransformFromPlaneEquation(planeEquation);

	aPlaneShape->setLocalPose(relativePose);
}

void physics::RigidBodyCreatePlaneCollider(RigidBody* rigidBody, math::Vector3 position, math::Vector3 normal)
{
	PxRigidActor* actor = g_activeRigidBodies[rigidBody];
	PxMaterial* aMaterial = g_Physics->createMaterial(0, 0, 0);

	PxPlaneGeometry geometry{};

	PxShape* aPlaneShape = PxRigidActorExt::createExclusiveShape(*actor, geometry, *aMaterial);
	PxPlane planeEquation{ GetPXVec(position), GetPXVec(normal) };
	PxTransform relativePose = PxTransformFromPlaneEquation(planeEquation);

	aPlaneShape->setLocalPose(relativePose);
}

bool physics::Raycast(math::Vector3 origin, math::Vector3 direction, RaycastHit& result, float maxDistance, uint32_t layerMask)
{
	PxVec3 pxOrigin = GetPXVec(origin);					// [in] Ray origin
	PxVec3 unitDir = GetPXVec(direction);               // [in] Normalized ray direction
	PxRaycastBuffer  hit{};								// [out] Raycast results

	// [in] Define what parts of PxRaycastHit we're interested in
	const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::Enum::ePOSITION | PxSceneQueryFlag::eNORMAL;

	// Raycast against all static & dynamic objects (no filtering)
	// The main result from this call is the closest hit, stored in the 'hit' structure
	bool status = g_scene->raycast(pxOrigin, unitDir, maxDistance, hit, outputFlags);

	//Closest hit.
	auto foundHit = hit.block;
	result.RigibBodyHit = reinterpret_cast<RigidBody*>(foundHit.actor->userData);
	result.Position = GetVec(foundHit.position);
	result.Normal = GetVec(foundHit.normal);

	return status;
}