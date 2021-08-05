// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  class zPhysicalObjectBase : public zCObject {
  public:
    virtual btRigidBody* GetRigidBody() = 0;

  protected:
    zPhysicalObjectBase();
    virtual ~zPhysicalObjectBase();
  };


  btTransform Mat4ToBtTransform( zMAT4 zTrafo );
  zMAT4 BtTransformToMat4( btTransform btTrafo );
  zMAT4 GetVobBBoxTrafo( zCVob* vob );
}