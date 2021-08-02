// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  // Poly method
  inline btConvexTriangleMeshShape* ProgMeshProtoToConvexTriangleMeshShape( zCProgMeshProto* proto ) {
    zCMesh* mesh = proto->GetMesh( 0 );
    if( !mesh ) {
      Message::Fatal( "No mesh data - " + proto->GetVisualName() );
      return Null;
    }
  
    btTriangleMesh* meshInterface = new btTriangleMesh();
    for( int i = 0; i < mesh->numPoly; i++ ) {
      zCPolygon* poly = mesh->polyList[i];
  
      meshInterface->addTriangle(
        poly->vertex[0]->position.ToBtVector3(),
        poly->vertex[1]->position.ToBtVector3(),
        poly->vertex[2]->position.ToBtVector3()
      );
    }
  
    btConvexTriangleMeshShape* triangleMeshShape = new btConvexTriangleMeshShape( meshInterface );
    triangleMeshShape->setMargin( 0.0 );
    mesh->Release();
    return triangleMeshShape;
  }


  // Vertex method
  inline btConvexHullShape* ProgMeshProtoToConvexHullShape( zCProgMeshProto* proto ) {
    zCMesh* mesh = proto->GetMesh( 0 );
    if( !mesh ) {
      Message::Fatal( "No mesh data - " + proto->GetVisualName() );
      return Null;
    }

    btConvexHullShape* convexHull = new btConvexHullShape();
    for( int i = 0; i < mesh->numVert; i++ ) {
      zCVertex* vert = mesh->vertList[i];
      convexHull->addPoint( vert->position.ToBtVector3() );
    }

    convexHull->optimizeConvexHull();
    convexHull->setMargin( 0.0 );
    mesh->Release();
    return convexHull;
  }


  // Poly method (no optimize)
  inline btBvhTriangleMeshShape* ProgMeshProtoToConvexBvhTriangleMeshShape( zCProgMeshProto* proto ) {
    zCMesh* mesh = proto->GetMesh( 0 );
    if( !mesh ) {
      Message::Fatal( "No mesh data - " + proto->GetVisualName() );
      return Null;
    }
  
    btTriangleMesh* meshInterface = new btTriangleMesh();
    for( int i = 0; i < mesh->numPoly; i++ ) {
      zCPolygon* poly = mesh->polyList[i];

      // if( poly->material && poly->material->noCollDet ) // IsIgnorePoly( poly ) )
      //   continue;
  
      meshInterface->addTriangle(
        poly->vertex[0]->position.ToBtVector3(),
        poly->vertex[1]->position.ToBtVector3(),
        poly->vertex[2]->position.ToBtVector3()
      );
    }
  
    btBvhTriangleMeshShape* triangleMeshShape = new btBvhTriangleMeshShape( meshInterface, true, true );
    triangleMeshShape->setMargin( 0.0 );
    mesh->Release();
    return triangleMeshShape;
  }


  inline zCProgMeshProto* LoadPhysicModel( const zSTRING& fileName3DS ) {
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
    zCProgMeshProto* proto = LoadPhysicModel( fileName3DS );
    if( !proto )
      return Null;

    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btConvexTriangleMeshShape* trimesh = ProgMeshProtoToConvexTriangleMeshShape( proto );
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      trimesh->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, trimesh, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalTriangleMesh* polyMesh = new zPhysicalTriangleMesh();
    polyMesh->PolyMesh = trimesh;
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
    zCProgMeshProto* proto = LoadPhysicModel( fileName3DS );
    if( !proto )
      return Null;

    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btConvexHullShape* hullMesh = ProgMeshProtoToConvexHullShape( proto );
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      hullMesh->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, hullMesh, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalHullMesh* polyMesh = new zPhysicalHullMesh();
    polyMesh->PolyMesh = hullMesh;
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
    zCProgMeshProto* proto = LoadPhysicModel( fileName3DS );
    if( !proto )
      return Null;

    btTransform btTrafo = Mat4ToBtTransform( trafo );
    btBvhTriangleMeshShape* trimesh = ProgMeshProtoToConvexBvhTriangleMeshShape( proto );
    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      trimesh->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, trimesh, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalBvhTriangleMesh* polyMesh = new zPhysicalBvhTriangleMesh();
    polyMesh->PolyMesh = trimesh;
    polyMesh->RigidBody = body;
    return polyMesh;
  }






















#if 0
  inline void CopyPolyList( btTriangleMesh* triMesh, zCProgMeshProto* proto ) {
    zCMesh* mesh = proto->GetMesh( 0 );
    if( !mesh ) {
      cmd << "No mesh data " << endl;
      return;
    }

    cmd << "Polys num to copy " << mesh->numPoly << endl;
#if 1
    for( int i = 0; i < mesh->numPoly; i++ ) {
      zCPolygon* poly = mesh->polyList[i];

      triMesh->addTriangle(
        poly->vertex[0]->position.ToBtVector3(),
        poly->vertex[1]->position.ToBtVector3(),
        poly->vertex[2]->position.ToBtVector3()
      );
    }
#else
    triMesh->addTriangle(
      btVector3( -50.0, 00.0, 50.0 ),
      btVector3( 50.0, 00.0, 00.0 ),
      btVector3( 00.0, 50.0, 00.0 )
    );

    triMesh->addTriangle(
      btVector3( -50.0, 00.0, 50.0 ),
      btVector3( 50.0, 00.0, -50.0 ),
      btVector3( 00.0, 50.0, 00.0 )
    );

    triMesh->addTriangle(
      btVector3( -50.0, 00.0, -50.0 ),
      btVector3( 50.0, 00.0, 00.0 ),
      btVector3( 00.0, 50.0, 00.0 )
    );

    triMesh->addTriangle(
      btVector3( -50.0, 50.0, 00.0 ),
      btVector3( -50.0, -50.0, 00.0 ),
      btVector3( 50.0, 00.0, 00.0 )
    );
#endif
    mesh->Release();
  }



  inline btConvexHullShape* ProgMeshProtoToConvexHullShape( zCProgMeshProto* proto ) {
    zCMesh* mesh = proto->GetMesh( 0 );
    if( !mesh ) {
      cmd << "No mesh data " << endl;
      return Null;
    }

    btConvexHullShape* convexHull = new btConvexHullShape();
    for( int i = 0; i < mesh->numVert; i++ ) {
      zCVertex* vert = mesh->vertList[i];
      convexHull->addPoint( vert->position.ToBtVector3() );
    }

    convexHull->optimizeConvexHull();
    convexHull->setMargin( 0.0 );
    mesh->Release();
    return convexHull;
  }



  inline btConvexHullShape* ProgMeshProtoToConvexHullShapeDetailed( zCProgMeshProto* proto ) {
    zCMesh* mesh = proto->GetMesh( 0 );
    if( !mesh ) {
      cmd << "No mesh data " << endl;
      return Null;
    }

    btConvexHullShape* convexHull = new btConvexHullShape();
    for( int i = 0; i < mesh->numPoly; i++ ) {
      zCPolygon* poly = mesh->polyList[i];

      convexHull->addPoint( poly->vertex[0]->position.ToBtVector3() );
      convexHull->addPoint( poly->vertex[1]->position.ToBtVector3() );
      convexHull->addPoint( poly->vertex[2]->position.ToBtVector3() );
    }

    convexHull->optimizeConvexHull();
    convexHull->setMargin( 0.0 );
    mesh->Release();
    return convexHull;
  }



#if 0
  zPhysicalPolyMesh* zPhysicalPolyMesh::CreatePolyMesh( const zSTRING& fileName3DS, const zVEC3& position, float mass, bool useInertia ) {
    // zCVisual* visual = zCVisual::LoadVisual( fileName3DS );
    // Message::Box( visual == Null ? "Govno" : visual->_GetClassDef()->className );
    // 
    // zCMesh* mesh = visual->CastTo<zCMesh>();

    zCProgMeshProto* proto = zCVisual::LoadVisual( fileName3DS )->CastTo<zCProgMeshProto>();
    if( !proto )
      return Null;

    // Create base shape object
    btTransform t;
    t.setIdentity();
    t.setOrigin( position.ToBtVector3() );
    btCompoundShape* shape = new btCompoundShape();

    // Add mesh data to triangle shape object
    btTriangleMesh* meshInterface = new btTriangleMesh();
    CopyPolyList( meshInterface, proto );
    proto->Release();

    btBvhTriangleMeshShape* trimesh = new btBvhTriangleMeshShape( meshInterface, true, true );
    btTransform t2;
    t2.setOrigin( btVector3( 0.0, 0.0, 0.0 ) );
    shape->addChildShape( t2, trimesh );


    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      shape->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( t );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, shape, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalPolyMesh* polyMesh = new zPhysicalPolyMesh();
    polyMesh->PolyMesh = shape;
    polyMesh->RigidBody = body;
    return polyMesh;
  }
#endif

  zPhysicalPolyMesh* zPhysicalPolyMesh::CreatePolyMesh( const zSTRING& fileName3DS, const zVEC3& position, float mass, bool useInertia ) {
    btTransform t;
    t.setIdentity();
    t.setOrigin( position.ToBtVector3() );
    btCompoundShape* compound = new btCompoundShape();
    // btConeShape* coneShape = new btConeShape( 100, 100 );


#if 0
    btConvexHullShape* convexHull = new btConvexHullShape();
    convexHull->addPoint( btVector3( -50.0, 00.0, 50.0 ) );
    convexHull->addPoint( btVector3( -50.0, 00.0, -50.0 ) );
    convexHull->addPoint( btVector3( 50.0, 00.0, 00.0 ) );
    convexHull->addPoint( btVector3( 00.0, 50.0, 00.0 ) );
    convexHull->optimizeConvexHull();
    //convexHull->initializePolyhedralFeatures();
    btTransform t2;
    t2.setIdentity();
    convexHull->setMargin( 0.0 );
#endif

    zCProgMeshProto* proto = zCVisual::LoadVisual( fileName3DS )->CastTo<zCProgMeshProto>();
    btConvexHullShape* convexHull = ProgMeshProtoToConvexHullShapeDetailed( proto );
    btTransform t2;
    t2.setIdentity();
    compound->addChildShape( t2, convexHull );



    // btTransform t2;
    // t2.setIdentity();
    // compound->addChildShape( t2, coneShape );

    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      compound->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( t );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, compound, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalPolyMesh* cone = new zPhysicalPolyMesh();
    cone->PolyMesh = compound;
    cone->RigidBody = body;
    return cone;
  }
#endif
}