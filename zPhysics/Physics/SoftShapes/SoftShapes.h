// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class zPhyslcalSoftTriangleMesh : public zPhysicalSoftMesh {
  protected:
    btConvexTriangleMeshShape* PolyMesh;
    zPhyslcalSoftTriangleMesh() : zPhysicalSoftMesh() {}
  public:

    static zPhyslcalSoftTriangleMesh* CreatePolyMesh( const zSTRING& fileName3DS, const zVEC3& position = zVEC3( 0.0f, 0.0f, 0.0f ), float mass = 0.0f, bool useInertia = false );
    static zPhyslcalSoftTriangleMesh* CreatePolyMesh( const zSTRING& fileName3DS, const zMAT4& trafo, float mass = 0.0f, bool useInertia = false );

	};
}