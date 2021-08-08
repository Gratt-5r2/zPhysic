// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
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
    sphere->CollisionShape = shape;
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
    box->CollisionShape = shape;
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
    cylinder->CollisionShape = shape;
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
    cone->CollisionShape = shape;
    cone->RigidBody = body;
    return cone;
  }





  zPhysicalCapsule* zPhysicalCapsule::CreateCapsule( const float& radius, const float& height, const zVEC3& position, float mass, bool useInertia ) {
    zMAT4 trafo;
    trafo.MakeIdentity();
    trafo.SetTranslation( position );
    return CreateCapsule( radius, height, trafo, mass, useInertia );
  }


  zPhysicalCapsule* zPhysicalCapsule::CreateCapsule( const float& radius, const float& height, const zMAT4& trafo, float mass, bool useInertia ) {
    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btCapsuleShape* shape = new btCapsuleShape( radius, height );
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      shape->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, shape, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalCapsule* capsule = new zPhysicalCapsule();
    capsule->CollisionShape = shape;
    capsule->RigidBody = body;
    return capsule;
  }
}