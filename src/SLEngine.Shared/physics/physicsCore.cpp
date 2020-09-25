#include "pch.h"
#include "../dxassert.h"
#include "physicsCore.h"
#include "physx/PxPhysicsAPI.h"

using namespace physx;

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;

static PxFoundation* g_Foundation = nullptr;
static PxPhysics* g_Physics = nullptr;
static PxPvd* g_debugger = nullptr;
static PxScene* g_scene = nullptr;

#define PVD_HOST "127.0.0.1"
#define PVD_PORT 5425

void CreateScene()
{
	PxSceneDesc sceneDesc(g_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -9, 0);
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	// create CPU dispatcher which mNbThreads worker threads
	auto mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
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
	g_scene->simulate(deltaT);
	g_scene->fetchResults(true);
}