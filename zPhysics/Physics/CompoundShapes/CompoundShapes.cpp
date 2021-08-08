// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  inline zCProgMeshProto* LoadPhysicalModel( const zSTRING& fileName3DS ) {
    zSTRING name        = fileName3DS.GetPattern( "", ".", -1 );
    zSTRING ext         = fileName3DS.GetPattern( ".", "", -1 );
    zSTRING phyMeshName = name + ".PHY.3DS";

    zCProgMeshProto* proto = zCVisual::LoadVisual( phyMeshName )->CastTo<zCProgMeshProto>();
    if( proto != Null )
      return proto;

    return zCVisual::LoadVisual( fileName3DS )->CastTo<zCProgMeshProto>();
  }




  zPhysicalTriangleMesh* zPhysicalTriangleMesh::CreatePolyMesh( const zSTRING& fileName3DS, const zVEC3& position, float mass, bool useInertia ) {
    zMAT4 trafo;
    trafo.MakeIdentity();
    trafo.SetTranslation( position );
    return CreatePolyMesh( fileName3DS, trafo, mass, useInertia );
  }

  zPhysicalTriangleMesh* zPhysicalTriangleMesh::CreatePolyMesh( const zSTRING& fileName3DS, const zMAT4& trafo, float mass, bool useInertia ) {
    zCProgMeshProto* proto = LoadPhysicalModel( fileName3DS );
    if( !proto )
      return Null;

    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btConvexTriangleMeshShape* trimesh = proto->GetBtTriangleMesh();
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      trimesh->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, trimesh, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalTriangleMesh* polyMesh = new zPhysicalTriangleMesh();
    polyMesh->CollisionShape = trimesh;
    polyMesh->RigidBody = body;
    return polyMesh;
  }





  zPhysicalHullMesh* zPhysicalHullMesh::CreatePolyMesh( const zSTRING& fileName3DS, const zVEC3& position, float mass, bool useInertia ) {
    zMAT4 trafo;
    trafo.MakeIdentity();
    trafo.SetTranslation( position );
    return CreatePolyMesh( fileName3DS, trafo, mass, useInertia );
  }



  zPhysicalHullMesh* zPhysicalHullMesh::CreatePolyMesh( const zSTRING& fileName3DS, const zMAT4& trafo, float mass, bool useInertia ) {
    zCProgMeshProto* proto = LoadPhysicalModel( fileName3DS );
    if( !proto )
      return Null;

    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btConvexHullShape* hullMesh = proto->GetBtHullMesh();
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      hullMesh->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, hullMesh, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalHullMesh* polyMesh = new zPhysicalHullMesh();
    polyMesh->CollisionShape = hullMesh;
    polyMesh->RigidBody = body;
    return polyMesh;
  }





  zPhysicalBvhTriangleMesh* zPhysicalBvhTriangleMesh::CreatePolyMesh( const zSTRING& fileName3DS, const zVEC3& position, float mass, bool useInertia ) {
    zMAT4 trafo;
    trafo.MakeIdentity();
    trafo.SetTranslation( position );
    return CreatePolyMesh( fileName3DS, trafo, mass, useInertia );
  }



  zPhysicalBvhTriangleMesh* zPhysicalBvhTriangleMesh::CreatePolyMesh( const zSTRING& fileName3DS, const zMAT4& trafo, float mass, bool useInertia ) {
    zCProgMeshProto* proto = LoadPhysicalModel( fileName3DS );
    if( !proto )
      return Null;

    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btBvhTriangleMeshShape* trimesh = proto->GetBtBvhTriangleMesh();
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      trimesh->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, trimesh, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalBvhTriangleMesh* polyMesh = new zPhysicalBvhTriangleMesh();
    polyMesh->CollisionShape = trimesh;
    polyMesh->RigidBody = body;
    return polyMesh;
  }
}