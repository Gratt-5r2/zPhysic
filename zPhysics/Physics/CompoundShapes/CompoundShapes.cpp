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


  zCMesh* zCProgMeshProto::GetMeshCollDet() {
    zCMesh* mesh = zNEW( zCMesh );
    int numPoly = 0;

    for( int n = 0; n < numSubMeshes; n++ ) {
      if( subMeshList[n].material && subMeshList[n].material->noCollDet )
        continue;

      numPoly += subMeshList[n].triList.GetNum();
    }

    zCMesh::S_InitVertexMergeCache( mesh );

    mesh->AllocPolys( numPoly );
    mesh->AllocVerts( numPoly * 3 );

    for( int i = 0; i < numSubMeshes; i++ ) {

      cmd << ">> " << endl;
      if( subMeshList[i].material && subMeshList[i].material->noCollDet ) {
        cmd << subMeshList[i].material->GetName() << "  " << subMeshList[i].material->noCollDet << endl;
        continue;
      }

      for( int p = 0; p < subMeshList[i].triList.GetNum(); p++ ) {
        
        
        // if( subMeshList[i].material )
        //   cmd << subMeshList[i].material->GetName() << "  " << subMeshList[i].material->noCollDet << endl;


        zVEC3 a, b, c;
        zVEC2	f, g, h;
        a = posList[subMeshList[i].wedgeList[subMeshList[i].triList[p].wedge[0]].position];
        b = posList[subMeshList[i].wedgeList[subMeshList[i].triList[p].wedge[1]].position];
        c = posList[subMeshList[i].wedgeList[subMeshList[i].triList[p].wedge[2]].position];
        f = subMeshList[i].wedgeList[subMeshList[i].triList[p].wedge[0]].texUV;
        g = subMeshList[i].wedgeList[subMeshList[i].triList[p].wedge[1]].texUV;
        h = subMeshList[i].wedgeList[subMeshList[i].triList[p].wedge[2]].texUV;
        zCPolygon* newPoly = mesh->AddPoly();
        newPoly->AddVertex( mesh->AddVertexSmart( a ) );
        newPoly->AddVertex( mesh->AddVertexSmart( b ) );
        newPoly->AddVertex( mesh->AddVertexSmart( c ) );

        newPoly->SetMapping( 0, f );
        newPoly->SetMapping( 1, g );
        newPoly->SetMapping( 2, h );
        newPoly->polyPlane = subMeshList[i].triPlaneList[subMeshList[i].triPlaneIndexList[p]];
        newPoly->SetMaterial( subMeshList[i].material );

      };
    }

    mesh->meshName = this->GetObjectName();
    zCMesh::S_DeleteVertexMergeCache();
    return mesh;
  }


  Array<zCMesh*> zCProgMeshProto::GetMeshListCollDet() {
    Array<zCMesh*> meshList;

    for( int i = 0; i < numSubMeshes; i++ ) {
      if( subMeshList[i].material->noCollDet )
        continue;

      int numPoly = subMeshList[i].triList.GetNum();
      zCMesh* mesh = new zCMesh();
      zCMesh::S_InitVertexMergeCache( mesh );
      mesh->AllocPolys( numPoly );
      mesh->AllocVerts( numPoly * 3 );

      for( int p = 0; p < numPoly; p++ ) {
        zVEC3 a, b, c;
        zVEC2	f, g, h;
        a = posList[subMeshList[i].wedgeList[subMeshList[i].triList[p].wedge[0]].position];
        b = posList[subMeshList[i].wedgeList[subMeshList[i].triList[p].wedge[1]].position];
        c = posList[subMeshList[i].wedgeList[subMeshList[i].triList[p].wedge[2]].position];
        f = subMeshList[i]        .wedgeList[subMeshList[i].triList[p].wedge[0]].texUV;
        g = subMeshList[i]        .wedgeList[subMeshList[i].triList[p].wedge[1]].texUV;
        h = subMeshList[i]        .wedgeList[subMeshList[i].triList[p].wedge[2]].texUV;

        zCPolygon* newPoly = mesh->AddPoly();
        newPoly->AddVertex( mesh->AddVertexSmart( a ) );
        newPoly->AddVertex( mesh->AddVertexSmart( b ) );
        newPoly->AddVertex( mesh->AddVertexSmart( c ) );

        newPoly->SetMapping( 0, f );
        newPoly->SetMapping( 1, g );
        newPoly->SetMapping( 2, h );
        newPoly->polyPlane = subMeshList[i].triPlaneList[subMeshList[i].triPlaneIndexList[p]];
        newPoly->SetMaterial( subMeshList[i].material );

        meshList.Insert( mesh );
      }

      zCMesh::S_DeleteVertexMergeCache();
    }

    return meshList;
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
      if( poly && poly->material && poly->material->noCollDet )
        continue;

      meshInterface->addTriangle(
        poly->vertex[0]->position.ToBtVector3(),
        poly->vertex[1]->position.ToBtVector3(),
        poly->vertex[2]->position.ToBtVector3()
      );
    }

    mesh->Release();

    if( meshInterface->getNumTriangles() == 0 ) {
      delete meshInterface;
      return Null;
    }

    btBvhTriangleMeshShape* triangleMeshShape = new btBvhTriangleMeshShape( meshInterface, true, true );
    triangleMeshShape->setMargin( 0.0 );
    return triangleMeshShape;
  }



  inline Array<btBvhTriangleMeshShape*> ProgMeshProtoToConvexBvhTriangleMeshShapeList( zCProgMeshProto* proto ) {
    Array<btBvhTriangleMeshShape*> triangleMeshShapeList;

    Array<zCMesh*> meshList = proto->GetMeshListCollDet();

    for( uint mesh_i = 0; mesh_i < meshList.GetNum(); mesh_i++ ) {
      zCMesh* mesh = meshList[mesh_i];

      btTriangleMesh* meshInterface = new btTriangleMesh();
      for( int i = 0; i < mesh->numPoly; i++ ) {
        zCPolygon* poly = mesh->polyList[i];

        meshInterface->addTriangle(
          poly->vertex[0]->position.ToBtVector3(),
          poly->vertex[1]->position.ToBtVector3(),
          poly->vertex[2]->position.ToBtVector3()
        );
      }

      btBvhTriangleMeshShape* triangleMeshShape = new btBvhTriangleMeshShape( meshInterface, true, true );
      triangleMeshShape->setMargin( 0.0 );
      mesh->Release();

      triangleMeshShapeList += triangleMeshShape;
    }

    return triangleMeshShapeList;
  }


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
    zCProgMeshProto* proto = LoadPhysicalModel( fileName3DS );
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
    zCProgMeshProto* proto = LoadPhysicalModel( fileName3DS );
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







  zPhysicalCompoundMesh* zPhysicalCompoundMesh::CreateCompoundMesh( const zSTRING& fileName3DS, const zVEC3& position, float mass, bool useInertia ) {
    zMAT4 trafo;
    trafo.MakeIdentity();
    trafo.SetTranslation( position );
    return CreateCompoundMesh( fileName3DS, trafo, mass, useInertia );
  }



  zPhysicalCompoundMesh* zPhysicalCompoundMesh::CreateCompoundMesh( const zSTRING& fileName3DS, const zMAT4& trafo, float mass, bool useInertia ) {
    zCProgMeshProto* proto = LoadPhysicalModel( fileName3DS );
    if( !proto )
      return Null;

    btTransform btTrafo = Mat4ToBtTransform( trafo );
    Array<btBvhTriangleMeshShape*> trimeshList = ProgMeshProtoToConvexBvhTriangleMeshShapeList( proto );

    btCompoundShape* compoundShape = new btCompoundShape();

    for( uint i = 0; i < trimeshList.GetNum(); i++ ) {
      btBvhTriangleMeshShape* trimesh = trimeshList[i];



      btTransform btTrafoLocal;
      btTrafoLocal.setIdentity();
      compoundShape->addChildShape( btTrafoLocal, trimesh );
    }

    btVector3 inertia( 0.0, 0.0, 0.0 );
    if( useInertia && mass != 0.0 )
      compoundShape->calculateLocalInertia( mass, inertia );

    btMotionState* motionState = new btDefaultMotionState( btTrafo );
    btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo( mass, motionState, compoundShape, inertia );
    btRigidBody* body = new btRigidBody( rigidBodyInfo );

    zPhysicalCompoundMesh* polyMesh = new zPhysicalCompoundMesh();
    polyMesh->CompoPolyMesh = compoundShape;
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