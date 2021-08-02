// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	Array<zPhysicalVobProvider*> zPhysicalVobProvider::Providers;


	zPhysicalVobProvider::zPhysicalVobProvider( zPhysicalMesh* physicalMesh, zCVob* connectedVob, const zPhysicalVobProviderType& type ) {
		PhysicalMesh = physicalMesh;
		PhysicalMesh->AddRef();
		ConnectedVob = connectedVob;
		Type = type;
		ConnectedVob->AddRef();
		Providers += this;
	}


	void zPhysicalVobProvider::Update() {
#if 1
		if( Type == zVOB_PROVIDER_PHYSICAL_TRAFO ) {
			btTransform& btTrafo = PhysicalMesh->GetRigidBody()->getWorldTransform();
			zMAT4 zTrafo = BtTransformToMat4( btTrafo );
			PUSH_COLLDET( ConnectedVob, False );
			ConnectedVob->SetTrafoObjToWorld( zTrafo );
			POP_COLLDET( ConnectedVob );
		}
		else {
			zMAT4& zTrafo = ConnectedVob->trafoObjToWorld;
			btTransform btTrafo = Mat4ToBtTransform( zTrafo );
			PhysicalMesh->GetRigidBody()->setWorldTransform( btTrafo );
		}
#else
		// zMAT4& vobTrafo = ConnectedVob->GetNewTrafoObjToWorld();
		zMAT4 newVobTrafo;
		newVobTrafo.MakeIdentity();
		btTransform& meshTrafo = PhysicalMesh->GetRigidBody()->getWorldTransform();
		meshTrafo.getOpenGLMatrix( (float*)&newVobTrafo );
		ConnectedVob->SetTrafoObjToWorld( newVobTrafo );

		btVector3 meshOrigin = meshTrafo.getOrigin();
		zVEC3 vobOrigin( meshOrigin[VX], meshOrigin[VY], meshOrigin[VZ] );
		zlineCache->Line3D( vobOrigin, vobOrigin + zVEC3( 0.0f, 35.0f, 0.0f ), GFX_RED, False );
#endif
	}


	void zPhysicalVobProvider::SetProviderType( const zPhysicalVobProviderType& type ) {
		Type = type;
	}


	void zPhysicalVobProvider::SetVelocity( const zVEC3& vec ) {
		PhysicalMesh->SetVelocity( btVector3( vec[VX], vec[VY], vec[VZ] ) );
	}


	zPhysicalMesh* zPhysicalVobProvider::GetPhysicalMesh() {
		return PhysicalMesh;
	}


	zPhysicalVobProvider::~zPhysicalVobProvider() {
		Providers -= this;
		ConnectedVob->Release();
		PhysicalMesh->Release();
	}


	void zPhysicalVobProvider::UpdateAllProvider() {
		for( uint i = 0; i < Providers.GetNum(); i++ )
			Providers[i]->Update();
	}


	void zPhysicalVobProvider::DeleteAllProvider() {
		Providers.DeleteData();
	}
}