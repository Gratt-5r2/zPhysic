// Supported with union (c) 2020 Union team

// User API for zCProgMeshProto
// Add your methods here

btConvexHullShape* GetBtHullMesh();
btConvexTriangleMeshShape* GetBtTriangleMesh();
btBvhTriangleMeshShape* GetBtBvhTriangleMesh();
const zVEC3& GetVertex( const int& subMeshID, const int& triID, const int& vertID );
