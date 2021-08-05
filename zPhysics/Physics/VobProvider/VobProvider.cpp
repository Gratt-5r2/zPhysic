// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	bool ProviderSortFuncByVob( zPhysicalVobProvider* const& left, zCVob& const right ) {
		return left->GetConnectedVob() > &right;
	}

	bool ProviderSortFunc( zPhysicalVobProvider* const& left, zPhysicalVobProvider* const& right ) {
		return ProviderSortFuncByVob( left, *right->GetConnectedVob() );
	}

  bool operator == ( zPhysicalVobProvider* left, const zCVob& right ) {
		return left->GetConnectedVob() == &right;
	}


	ArraySorted<zPhysicalVobProvider*, ProviderSortFunc> zPhysicalVobProvider::Providers;


	zPhysicalVobProvider::zPhysicalVobProvider( zPhysicalMesh* physicalMesh, zCVob* connectedVob, const zPhysicalVobProviderType& type ) : zPhysicalObjectBase() {
		PhysicalMesh = physicalMesh;
		PhysicalMesh->AddRef();
		ConnectedVob = connectedVob;
		Type = type;
		Providers.Insert( this );

		EnableVobReference = dynamic_cast<zCVobPhysical*>( connectedVob ) == Null;
		if( EnableVobReference )
			ConnectedVob->AddRef();
	}


	void zPhysicalVobProvider::Update() {
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
	}


	void zPhysicalVobProvider::SetProviderType( const zPhysicalVobProviderType& type ) {
		Type = type;
	}


	void zPhysicalVobProvider::SetVelocity( const zVEC3& vec ) {
		PhysicalMesh->SetVelocity( btVector3( vec[VX], vec[VY], vec[VZ] ) );
	}


	void zPhysicalVobProvider::SetPosition( const zVEC3& vec ) {
		PhysicalMesh->SetPosition( vec );
	}


	void zPhysicalVobProvider::SetMatrix( const zMAT4& trafo ) {
		PhysicalMesh->SetMatrix( trafo );
	}


	void zPhysicalVobProvider::SetEnableVobReference( bool enable ) {
		if( EnableVobReference && !enable ) {
			ConnectedVob->Release();
			EnableVobReference = enable;
		}
		else if( !EnableVobReference && enable ) {
			ConnectedVob->AddRef();
			EnableVobReference = enable;
		}
	}


	zPhysicalMesh* zPhysicalVobProvider::GetPhysicalMesh() const {
		return PhysicalMesh;
	}


	zCVob* zPhysicalVobProvider::GetConnectedVob() const {
		return ConnectedVob;
	}


	btRigidBody* zPhysicalVobProvider::GetRigidBody() {
		return PhysicalMesh->GetRigidBody();
	}


	zPhysicalVobProvider::~zPhysicalVobProvider() {
		if( EnableVobReference )
			ConnectedVob->Release();

		PhysicalMesh->Release();
		Providers.Remove( this );
	}


	zPhysicalVobProvider* zPhysicalVobProvider::FindProvider( zCVob* vob ) {
		uint index = Providers.SearchEqual<ProviderSortFuncByVob>( *vob );
		if( index == Invalid )
			return Null;

		return Providers[index];
	}


	void zPhysicalVobProvider::UpdateAllProvider() {
		for( uint i = 0; i < Providers.GetNum(); i++ )
			Providers[i]->Update();
	}


	void zPhysicalVobProvider::DeleteAllProvider() {
		Providers.ReleaseData();
	}
}