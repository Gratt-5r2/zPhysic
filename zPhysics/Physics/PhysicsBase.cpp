// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static uint physicalMeshesAllocated = 0;

  zPhysicalObjectBase::zPhysicalObjectBase() : zCObject() {
    physicalMeshesAllocated++;
  }

  zPhysicalObjectBase::~zPhysicalObjectBase() {
    if( --physicalMeshesAllocated == 0 )
      cmd << Col16( CMD_INT | CMD_GREEN ) << "All physical vobs deleted" << Col16() << endl;
  }



  zTPhysicalMeshType zPhysicalMesh::Type = zTPhysicalMeshType::zPHY_MSH_EMPTY;

  zPhysicalMesh::zPhysicalMesh() : zPhysicalObjectBase() {
  }


  void zPhysicalMesh::SetVelocity( const zVEC3& vec ) {
    RigidBody->setLinearVelocity( vec.ToBtVector3() );
  }


  void zPhysicalMesh::SetPosition( const zVEC3& vec ) {
    btTransform& trafo = RigidBody->getWorldTransform();
    trafo.setOrigin( vec.ToBtVector3() );
    RigidBody->setWorldTransform( trafo );
  }


  void zPhysicalMesh::SetMatrix( const zMAT4& trafo ) {
    RigidBody->setWorldTransform( Mat4ToBtTransform( trafo ) );
  }


  btCollisionObject* zPhysicalMesh::GetCollisionObject() {
    return CollisionObject;
  }


  btRigidBody* zPhysicalMesh::GetRigidBody() {
    if( GetMeshType() == zTPhysicalMeshType::zPHY_MSH_RIGID )
      return RigidBody;

    return Null;
  }


  btSoftBody* zPhysicalMesh::GetSoftBody() {
    if( GetMeshType() == zTPhysicalMeshType::zPHY_MSH_SOFT )
      return SoftBody;

    return Null;
  }


  zTPhysicalMeshType zPhysicalMesh::GetMeshType() {
    return Type;
  }


  btCollisionShape* zPhysicalMesh::GetCollisionShape() {
    return RigidBody->getCollisionShape();
  }


  zPhysicalMesh::~zPhysicalMesh() {
    btMotionState* state = RigidBody->getMotionState();
    btCollisionShape* shape = RigidBody->getCollisionShape();
    delete shape;
    delete state;
  }



  btTransform Mat4ToBtTransform( zMAT4 zTrafo ) {
    zCQuat zRotation;
    zRotation.Matrix4ToQuat( zTrafo );
    zRotation = zRotation.Inverse();

    btQuaternion btRotation;
    btRotation.set128( *(btSimdFloat4*)&zRotation );

    btTransform btTrafo;
    btTrafo.setIdentity();
    btTrafo.setRotation( btRotation );
    btTrafo.setOrigin( zTrafo.GetTranslation().ToBtVector3() );
    return btTrafo;
  }

  zMAT4 BtTransformToMat4( btTransform btTrafo ) {
    zCQuat vobRotation;
    btQuaternion& meshRotation = btTrafo.getRotation();
    *(btSimdFloat4*)&vobRotation = meshRotation.get128();

    btVector3 meshOrigin = btTrafo.getOrigin();
    zVEC3 vobOrigin = meshOrigin;

    zMAT4 zTrafo;
    zTrafo.MakeIdentity();
    vobRotation.Inverse().QuatToMatrix4( zTrafo );
    zTrafo.SetTranslation( vobOrigin );
    return zTrafo;
  }

  zMAT4 GetVobBBoxTrafo( zCVob* vob ) {
    zMAT4 trafo = vob->trafoObjToWorld;
    zTBBox3D bbox = vob->GetBBox3DLocal();
    zVEC3 offset = bbox.GetCenter();
    zVEC3 vobPositionWorld = trafo.GetTranslation();
    zVEC3 centerOfVob = vobPositionWorld + offset;
    trafo.SetTranslation( centerOfVob );
    return trafo;
  }

  zVEC3 GetVobBBoxOffset( zCVob* vob ) {
    zMAT4 trafo = vob->trafoObjToWorld;
    zTBBox3D bbox = vob->GetBBox3DLocal();
    return bbox.GetCenter();
  }



  zTPhysicalMeshType zPhysicalRigidMesh::Type = zPHY_MSH_RIGID;


  btCollisionShape* zPhysicalRigidMesh::GetCollisionShape() {
    return CollisionShape;
  }


  zTPhysicalMeshType zPhysicalRigidMesh::GetMeshType() {
    return Type;
  }


  zTPhysicalMeshType zPhysicalRigidMesh::GetMeshTypeStatic() {
    return Type;
  }



  zTPhysicalMeshType zPhysicalSoftMesh::Type = zPHY_MSH_SOFT;


  btCollisionShape* zPhysicalSoftMesh::GetCollisionShape() {
    return Null; // TODO
  }


  zTPhysicalMeshType zPhysicalSoftMesh::GetMeshType() {
    return Type;
  }


  zTPhysicalMeshType zPhysicalSoftMesh::GetMeshTypeStatic() {
    return Type;
  }
}