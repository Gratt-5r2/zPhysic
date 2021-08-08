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
    GizmoModeEnabled = false;
    Providers.Insert( this );
    SavedVobTrafo = connectedVob->trafoObjToWorld;

    InternalProvider = dynamic_cast<zCVobPhysical*>( connectedVob ) != Null;
    if( !InternalProvider )
      ConnectedVob->AddRef();
  }


  bool zPhysicalVobProvider::IsVobPhysicsEnabled() {
    if( InternalProvider )
      return ConnectedPhysicalVob->IsPhysicsEnabled();

    return true;
  }


  bool zPhysicalVobProvider::UpdateByGizmo() {
    if( !IsOnEditor() )
      return false;

    if( zMouseKeyPressed( Left ) ) {
      if( ConnectedVob->trafoObjToWorld != SavedVobTrafo ) {
        if( !GizmoModeEnabled )
          GizmoModeEnabled = true;
      }
    }
    else if( GizmoModeEnabled )
      GizmoModeEnabled = false;

    return GizmoModeEnabled;
  }


  zMAT4 Vob_GetCorrectedAbMatrix( zCVob* vob, bool internal ) {
    if( internal ) {
      zCVobPhysical* vobPhysical = (zCVobPhysical*)vob;
      return vobPhysical->GetCorrectedAbMatrix();
    }

    return vob->trafoObjToWorld;
  }


  void Vob_SetCorrectedAbMatrix( zCVob* vob, const zMAT4& trafo, bool internal ) {
    if( internal ) {
      zCVobPhysical* vobPhysical = (zCVobPhysical*)vob;
      return vobPhysical->SetCorrectedAbMatrix( trafo );
    }

    vob->SetTrafoObjToWorld( trafo );
  }


  void zPhysicalVobProvider::Update() {
    uint type = Type;
    if( !IsVobPhysicsEnabled() || UpdateByGizmo() )
      type = zVOB_PROVIDER_VOB_TRAFO;

    if( type == zVOB_PROVIDER_PHYSICAL_TRAFO ) {
      btTransform& btTrafo = PhysicalMesh->GetRigidBody()->getWorldTransform();
      zMAT4 zTrafo = BtTransformToMat4( btTrafo );
      PUSH_COLLDET( ConnectedVob, False );
      Vob_SetCorrectedAbMatrix( ConnectedVob, zTrafo, InternalProvider );
      SavedVobTrafo = zTrafo;
      POP_COLLDET( ConnectedVob );
    }
    else {
      btRigidBody* rigidBody = PhysicalMesh->GetRigidBody();
      zMAT4& zTrafo = Vob_GetCorrectedAbMatrix( ConnectedVob, InternalProvider );
      btTransform btTrafo = Mat4ToBtTransform( zTrafo );
      zVEC3 vobMotion = zTrafo.GetTranslation() - SavedVobTrafo.GetTranslation();

      // For editors only
      if( GizmoModeEnabled ) {
        vobMotion *= 10.0f * ztimer->factorMotion;
        rigidBody->setLinearVelocity( vobMotion.ToBtVector3() );
        rigidBody->setAngularVelocity( btVector3( 0.0f, 0.0f, 0.0f ) );
      }

      rigidBody->setWorldTransform( btTrafo );
      SavedVobTrafo = zTrafo;
    }

    auto rigidBody = PhysicalMesh->GetRigidBody();
    zVEC3 v = rigidBody->getWorldTransform().getOrigin();
    zlineCache->Line3D( v, v + zVEC3( 0.0f, 100.0f, 0.0f ), GFX_ORANGE, 1 );

    if( InternalProvider )
      ConnectedPhysicalVob->ProviderCallback();
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


  zPhysicalMesh* zPhysicalVobProvider::GetPhysicalMesh() const {
    return PhysicalMesh;
  }


  zCVob* zPhysicalVobProvider::GetConnectedVob() const {
    return ConnectedVob;
  }


  btRigidBody* zPhysicalVobProvider::GetRigidBody() {
    return PhysicalMesh->GetRigidBody();
  }


  btSoftBody* zPhysicalVobProvider::GetSoftBody() {
    return PhysicalMesh->GetSoftBody();
  }


  btCollisionObject* zPhysicalVobProvider::GetCollisionObject() {
    return PhysicalMesh->GetCollisionObject();
  }


  zPhysicalVobProvider::~zPhysicalVobProvider() {
    if( !InternalProvider )
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





  Array<zPhysicalNpcProvider*> zPhysicalNpcProvider::Providers;

  zPhysicalNpcProvider::zPhysicalNpcProvider( oCNpc* npc ) {
    zTBBox3D bbox = npc->visual->GetBBox3D();
    zVEC3 extents = bbox.maxs - bbox.mins;
    float width   = (extents[VX] + extents[VZ]) * 0.4f;
    float height  = extents[VY] * 0.25f;

    zPhysicalCapsule* cylinder = zPhysicalCapsule::CreateCapsule( width, height, npc->trafoObjToWorld, 5.0f );
    btRigidBody* rigidBody = cylinder->GetRigidBody();
    rigidBody->setAngularFactor( 0.0f );
    rigidBody->setFriction( 0.0f );
    rigidBody->setRestitution( 0.0f );
    //rigidBody->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
    rigidBody->setGravity( btVector3( 0.0f, 0.0f, 0.0f ) );

    PhysicalMesh = cylinder;
    ConnectedNpc = npc;
    npc->AddRef();

    Providers.Insert( this );
  }


  void zPhysicalNpcProvider::Update() {
    auto rigidBody = PhysicalMesh->GetRigidBody();

    zTBBox3D bbox;
    btVector3 btMin;
    btVector3 btMax;

    rigidBody->getAabb( btMin, btMax );

    bbox.mins = btMin;
    bbox.maxs = btMax;
    bbox.Draw( GFX_AQUA );

    zMAT4& zTrafo   = ConnectedNpc->GetCorrectedAbMatrix();
    zVEC3 posTarget = zTrafo.GetTranslation();
    zVEC3 posRigid  = rigidBody->getWorldTransform().getOrigin();
    zVEC3 velocity  = posTarget - posRigid;
    NextPosition    = posTarget + velocity;

    btVector3 linearVelo = rigidBody->getLinearVelocity();
    linearVelo[VY] = 0.0f;
    rigidBody->setLinearVelocity( linearVelo * 0.5f );
    rigidBody->setWorldTransform( Mat4ToBtTransform( zTrafo ) );
    rigidBody->activate();
  }


  inline bool BodyStateIsNormal( int bodyState ) {
    return
      bodyState != BS_CLIMB &&
      bodyState != BS_JUMP &&
      bodyState != BS_FALL &&
      bodyState != BS_ITEMINTERACT &&
      bodyState != BS_MOBINTERACT &&
      bodyState != BS_MOBINTERACT_INTERRUPT &&
      bodyState != BS_STUMBLE &&
      bodyState != BS_UNCONSCIOUS &&
      bodyState != BS_DEAD;
  }


  void zPhysicalNpcProvider::PostUpdate() {
    if( !BodyStateIsNormal( ConnectedNpc->bodyState ) )
      return;

    auto rigidBody = PhysicalMesh->GetRigidBody();
    zVEC3 posNpc   = ConnectedNpc->GetCorrectedAbMatrix().GetTranslation();
    zVEC3 posRigid = rigidBody->getWorldTransform().getOrigin();

    float frameFactor = ztimer->frameTimeFloat;
    if( zVEC3( NextPosition[VX], 0.0f, NextPosition[VZ] ).Distance( zVEC3( posRigid[VX], 0.0f, posRigid[VZ] ) ) < 8.0f )
      return;

    zVEC3 motion = (posRigid - posNpc) * 0.1f;

    ConnectedNpc->collDetectionDynamic = 0;
    ConnectedNpc->Move( motion[VX], motion[VY], motion[VZ] );
    ConnectedNpc->collDetectionDynamic = 1;

    cmd << string( NextPosition.Distance( posRigid ), 1 ) << endl;
  }


  btRigidBody* zPhysicalNpcProvider::GetRigidBody() {
    return PhysicalMesh->GetRigidBody();
  }


  btSoftBody* zPhysicalNpcProvider::GetSoftBody() {
    return PhysicalMesh->GetSoftBody();
  }


  btCollisionObject* zPhysicalNpcProvider::GetCollisionObject() {
    return PhysicalMesh->GetCollisionObject();
  }


  void zPhysicalNpcProvider::UpdateAllProvider() {
    for( uint i = 0; i < Providers.GetNum(); i++ )
      Providers[i]->Update();
  }


  void zPhysicalNpcProvider::PostUpdateAllProvider() {
    for( uint i = 0; i < Providers.GetNum(); i++ )
      Providers[i]->PostUpdate();
  }


  zPhysicalNpcProvider::~zPhysicalNpcProvider() {
    PhysicalMesh->Release();
    ConnectedNpc->Release();
    Providers.Remove( this );
  }
}