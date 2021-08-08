// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  const zVEC3& zCProgMeshProto::GetVertex( const int& subMeshID, const int& triID, const int& vertID ) {
    return posList[subMeshList[subMeshID].wedgeList[subMeshList[subMeshID].triList[triID].wedge[vertID]].position];
  }


  // Vertex method
  btConvexHullShape* zCProgMeshProto::GetBtHullMesh() {
    btConvexHullShape* convexHull = new btConvexHullShape();
    for( int subMeshID = 0; subMeshID < numSubMeshes; subMeshID++ ) {
      auto material = subMeshList[subMeshID].material;
      if( material && material->noCollDet )
        continue;

      for( int triID = 0; triID < subMeshList[subMeshID].triList.GetNum(); triID++ ) {
        convexHull->addPoint( GetVertex( subMeshID, triID, VA ).ToBtVector3() );
        convexHull->addPoint( GetVertex( subMeshID, triID, VB ).ToBtVector3() );
        convexHull->addPoint( GetVertex( subMeshID, triID, VC ).ToBtVector3() );
      }
    }

    if( convexHull->getNumVertices() == 0 ) {
      delete convexHull;
      return Null;
    }

    convexHull->optimizeConvexHull();
    convexHull->setMargin( 0.0 );
    return convexHull;
  }


  // Poly method
  inline btConvexTriangleMeshShape* zCProgMeshProto::GetBtTriangleMesh() {
    btTriangleMesh* meshInterface = new btTriangleMesh();
    for( int subMeshID = 0; subMeshID < numSubMeshes; subMeshID++ ) {
      auto material = subMeshList[subMeshID].material;
      if( material && material->noCollDet )
        continue;

      for( int triID = 0; triID < subMeshList[subMeshID].triList.GetNum(); triID++ ) {
        meshInterface->addTriangle(
          GetVertex( subMeshID, triID, VA ).ToBtVector3(),
          GetVertex( subMeshID, triID, VB ).ToBtVector3(),
          GetVertex( subMeshID, triID, VC ).ToBtVector3()
        );
      }
    }

    if( meshInterface->getNumTriangles() == 0 ) {
      delete meshInterface;
      return Null;
    }

    btConvexTriangleMeshShape* triangleMeshShape = new btConvexTriangleMeshShape( meshInterface );
    triangleMeshShape->setMargin( 0.0 );
    return triangleMeshShape;
  }


  // Poly method (no optimize)
  btBvhTriangleMeshShape* zCProgMeshProto::GetBtBvhTriangleMesh() {
    btTriangleMesh* meshInterface = new btTriangleMesh();
    for( int subMeshID = 0; subMeshID < numSubMeshes; subMeshID++ ) {
      auto material = subMeshList[subMeshID].material;
      if( material && material->noCollDet )
        continue;

      for( int triID = 0; triID < subMeshList[subMeshID].triList.GetNum(); triID++ ) {
        meshInterface->addTriangle(
          GetVertex( subMeshID, triID, VA ).ToBtVector3(),
          GetVertex( subMeshID, triID, VB ).ToBtVector3(),
          GetVertex( subMeshID, triID, VC ).ToBtVector3()
        );
      }
    }

    if( meshInterface->getNumTriangles() == 0 ) {
      delete meshInterface;
      return Null;
    }

    btBvhTriangleMeshShape* triangleMeshShape = new btBvhTriangleMeshShape( meshInterface, true, true );
    triangleMeshShape->setMargin( 0.0 );
    return triangleMeshShape;
  }
}