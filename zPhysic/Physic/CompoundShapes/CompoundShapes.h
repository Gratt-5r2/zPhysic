// Supported with union (c) 2020 Union team
// Union HEADER file
#define USE_VERTEX_METHOD

namespace GOTHIC_ENGINE {
  class zPhysicalTriangleMesh : public zPhysicalMesh {
  protected:
    btConvexTriangleMeshShape* PolyMesh;
    zPhysicalTriangleMesh() {}
  public:

    static zPhysicalTriangleMesh* CreatePolyMesh( const zSTRING& fileName3DS, const zVEC3& position = zVEC3( 0.0f, 0.0f, 0.0f ), float mass = 0.0f, bool useInertia = false );
    static zPhysicalTriangleMesh* CreatePolyMesh( const zSTRING& fileName3DS, const zMAT4& trafo, float mass = 0.0f, bool useInertia = false );
  };


  class zPhysicalHullMesh : public zPhysicalMesh {
  protected:
    btConvexHullShape* PolyMesh;
    zPhysicalHullMesh() {}
  public:

    static zPhysicalHullMesh* CreatePolyMesh( const zSTRING& fileName3DS, const zVEC3& position = zVEC3( 0.0f, 0.0f, 0.0f ), float mass = 0.0f, bool useInertia = false );
    static zPhysicalHullMesh* CreatePolyMesh( const zSTRING& fileName3DS, const zMAT4& trafo, float mass = 0.0f, bool useInertia = false );
  };


  class zPhysicalBvhTriangleMesh : public zPhysicalMesh {
  protected:
    btBvhTriangleMeshShape* PolyMesh;
    zPhysicalBvhTriangleMesh() {}
  public:

    static zPhysicalBvhTriangleMesh* CreatePolyMesh( const zSTRING& fileName3DS, const zVEC3& position = zVEC3( 0.0f, 0.0f, 0.0f ), float mass = 0.0f, bool useInertia = false );
    static zPhysicalBvhTriangleMesh* CreatePolyMesh( const zSTRING& fileName3DS, const zMAT4& trafo, float mass = 0.0f, bool useInertia = false );
  };


  class zPhysicalCompoundMesh : public zPhysicalMesh {
  protected:
    btCompoundShape* PolyMesh;
    zPhysicalCompoundMesh() {}
  public:

    static zPhysicalCompoundMesh* CreateCompoundMesh( const zSTRING& fileName3DS, const zVEC3& position = zVEC3( 0.0f, 0.0f, 0.0f ), float mass = 0.0f, bool useInertia = false );
  };
}