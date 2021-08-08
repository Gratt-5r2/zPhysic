// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  class zPhysicalSphere : public zPhysicalRigidMesh {
  protected:
    zPhysicalSphere() : zPhysicalRigidMesh() {}
  public:

    static zPhysicalSphere* CreateSphere( float radius, const zVEC3& position = zVEC3( 0.0f, 0.0f, 0.0f ), float mass = 0.0f, bool useInertia = false );
    static zPhysicalSphere* CreateSphere( float radius, const zMAT4& trafo, float mass = 0.0f, bool useInertia = false );
  };


  class zPhysicalBox : public zPhysicalRigidMesh {
  protected:
    zPhysicalBox() : zPhysicalRigidMesh() {}
  public:

    static zPhysicalBox* CreateBox( const zVEC3& halfExtents, const zVEC3& position = zVEC3( 0.0f, 0.0f, 0.0f ), float mass = 0.0f, bool useInertia = false );
    static zPhysicalBox* CreateBox( const zVEC3& halfExtents, const zMAT4& trafo, float mass = 0.0f, bool useInertia = false );
  };


  class zPhysicalCylinder : public zPhysicalRigidMesh {
  protected:
    zPhysicalCylinder() : zPhysicalRigidMesh() {}
  public:

    static zPhysicalCylinder* CreateCylinder( const zVEC3& halfExtents, const zVEC3& position = zVEC3( 0.0f, 0.0f, 0.0f ), float mass = 0.0f, bool useInertia = false );
    static zPhysicalCylinder* CreateCylinder( const zVEC3& halfExtents, const zMAT4& trafo, float mass = 0.0f, bool useInertia = false );
  };


  class zPhysicalCone : public zPhysicalRigidMesh {
  protected:
    zPhysicalCone() : zPhysicalRigidMesh() {}
  public:

    static zPhysicalCone* CreateCone( const float& radius, const float& height, const zVEC3& position = zVEC3( 0.0f, 0.0f, 0.0f ), float mass = 0.0f, bool useInertia = false );
    static zPhysicalCone* CreateCone( const float& radius, const float& height, const zMAT4& trafo, float mass = 0.0f, bool useInertia = false );
  };


  class zPhysicalCapsule : public zPhysicalRigidMesh {
  protected:
    zPhysicalCapsule() : zPhysicalRigidMesh() {}
  public:

    static zPhysicalCapsule* CreateCapsule( const float& radius, const float& height, const zVEC3& position = zVEC3( 0.0f, 0.0f, 0.0f ), float mass = 0.0f, bool useInertia = false );
    static zPhysicalCapsule* CreateCapsule( const float& radius, const float& height, const zMAT4& trafo, float mass = 0.0f, bool useInertia = false );
  };
}