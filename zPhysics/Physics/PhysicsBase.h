// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  enum zTPhysicalMeshType {
    zPHY_MSH_EMPTY,
    zPHY_MSH_RIGID,
    zPHY_MSH_SOFT
  };


  class zPhysicalObjectBase : public zCObject {
  public:
    virtual btCollisionObject* GetCollisionObject() = 0;
    virtual btRigidBody* GetRigidBody() = 0;
    virtual btSoftBody* GetSoftBody() = 0;

  protected:
    zPhysicalObjectBase();
    virtual ~zPhysicalObjectBase();
  };


  class zPhysicalMesh : public zPhysicalObjectBase {
  protected:
    static zTPhysicalMeshType Type;

    union {
      btCollisionObject* CollisionObject;
      btRigidBody* RigidBody;
      btSoftBody* SoftBody;
    };

    zPhysicalMesh();
  public:

    virtual void SetVelocity( const zVEC3& vec );
    virtual void SetPosition( const zVEC3& vec );
    virtual void SetMatrix( const zMAT4& trafo );

    // Get the mesh body
    virtual btCollisionObject* GetCollisionObject();
    virtual btRigidBody* GetRigidBody();
    virtual btSoftBody* GetSoftBody();
    virtual zTPhysicalMeshType GetMeshType();
    
    // Get a shape data of the mesh body
    virtual btCollisionShape* GetCollisionShape() = 0;
    virtual ~zPhysicalMesh();
  };


  class zPhysicalRigidMesh : public zPhysicalMesh {
    static zTPhysicalMeshType Type;
  protected:
    btCollisionShape* CollisionShape;
  public:
    virtual btCollisionShape* GetCollisionShape();
    virtual zTPhysicalMeshType GetMeshType();
    static zTPhysicalMeshType GetMeshTypeStatic();
  };


  class zPhysicalSoftMesh : public zPhysicalMesh {
    static zTPhysicalMeshType Type;
  public:
    virtual btCollisionShape* GetCollisionShape();
    virtual zTPhysicalMeshType GetMeshType();
    static zTPhysicalMeshType GetMeshTypeStatic();
  };


  btTransform Mat4ToBtTransform( zMAT4 zTrafo );
  zMAT4 BtTransformToMat4( btTransform btTrafo );
  zMAT4 GetVobBBoxTrafo( zCVob* vob );
}