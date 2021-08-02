// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  zPhysicalMesh::~zPhysicalMesh() {
    btMotionState* state = RigidBody->getMotionState();
    btCollisionShape* shape = RigidBody->getCollisionShape();
    delete shape;
    delete state;
  }


  void zPhysicalMesh::SetVelocity( const zVEC3& vec ) {
    RigidBody->setLinearVelocity( vec.ToBtVector3() );
  }


  btRigidBody* zPhysicalMesh::GetRigidBody() {
    return RigidBody;
  }






  zPhysicalSphere* zPhysicalSphere::CreateSphere( float radius, const zVEC3& position, float mass, bool useInertia ) {
    zMAT4 trafo;
    trafo.MakeIdentity();
    trafo.SetTranslation( position );
    return CreateSphere( radius, trafo, mass, useInertia );
  }


  zPhysicalSphere* zPhysicalSphere::CreateSphere( float radius, const zMAT4& trafo, float mass, bool useInertia ) {
    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btSphereShape* shape = new btSphereShape( radius );
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      shape->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, shape, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );
    // body->setCollisionFlags();

    zPhysicalSphere* sphere = new zPhysicalSphere();
    sphere->Sphere = shape;
    sphere->RigidBody = body;
    return sphere;
  }






  zPhysicalBox* zPhysicalBox::CreateBox( const zVEC3& halfExtents, const zVEC3& position, float mass, bool useInertia ) {
    zMAT4 trafo;
    trafo.MakeIdentity();
    trafo.SetTranslation( position );
    return CreateBox( halfExtents , trafo, mass, useInertia );
  }


  zPhysicalBox* zPhysicalBox::CreateBox( const zVEC3& halfExtents, const zMAT4& trafo, float mass, bool useInertia ) {
    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btBoxShape* shape = new btBoxShape( halfExtents.ToBtVector3() );
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      shape->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, shape, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalBox* box = new zPhysicalBox();
    box->Box = shape;
    box->RigidBody = body;
    return box;
  }






  zPhysicalCylinder* zPhysicalCylinder::CreateCylinder( const zVEC3& halfExtents, const zVEC3& position, float mass, bool useInertia ) {
    zMAT4 trafo;
    trafo.MakeIdentity();
    trafo.SetTranslation( position );
    return CreateCylinder( halfExtents, trafo, mass, useInertia );
  }


  zPhysicalCylinder* zPhysicalCylinder::CreateCylinder( const zVEC3& halfExtents, const zMAT4& trafo, float mass, bool useInertia ) {
    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btCylinderShape* shape = new btCylinderShape( halfExtents.ToBtVector3() );
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      shape->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, shape, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalCylinder* cylinder = new zPhysicalCylinder();
    cylinder->Cylinder = shape;
    cylinder->RigidBody = body;
    return cylinder;
  }






  zPhysicalCone* zPhysicalCone::CreateCone( const float& radius, const float& height, const zVEC3& position, float mass, bool useInertia ) {
    zMAT4 trafo;
    trafo.MakeIdentity();
    trafo.SetTranslation( position );
    return CreateCone( radius, height, trafo, mass, useInertia );
  }


  zPhysicalCone* zPhysicalCone::CreateCone( const float& radius, const float& height, const zMAT4& trafo, float mass, bool useInertia ) {
    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btConeShape* shape = new btConeShape( radius, height );
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      shape->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, shape, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );
   
    zPhysicalCone* cone = new zPhysicalCone();
    cone->Cone = shape;
    cone->RigidBody = body;
    return cone;
  }
}