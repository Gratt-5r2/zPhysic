// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  enum zTPhysicalVobCollisionMethod {
    zCOLL_METHOD_BOX,
    zCOLL_METHOD_SPHERE,
    zCOLL_METHOD_CYLINDER,
    zCOLL_METHOD_CONE,
    zCOLL_METHOD_VOBSHAPE,
    zCOLL_METHOD_OTHERSHAPE
  };

	class zCVobPhysical : public zCVob {
		zPhysicalVobProvider* Provider;
    bool IsPhysicEnabled;

    struct {
      int Physic;
      float Mass;
      float Friction;
      float Restitution;
      int Shape;
      zSTRING OtherShape;
    };

    zPhysicalMesh* CreatePhysicalMesh( const zTPhysicalVobCollisionMethod& method, const zSTRING& otherShape = "" );
	public:

    zCLASS_UNION_DECLARATION( zCVobPhysical );

    zCVobPhysical();
    void PhysicEnable();
    void PhysicDisable();
    void SetPositionWorld( zVEC3 const& position );
    void SetTrafoWorld( zMAT4 const& trafo );
    void SetMass( const float& mass );
    virtual void SetVisual( zCVisual* instance );
    virtual void SetVisual( zCVisual* instance, const zTPhysicalVobCollisionMethod& method, const zSTRING& otherShape = "" );
    virtual void SetVisual( zSTRING const& visualName );
    virtual void SetVisual( zSTRING const& visualName, const zTPhysicalVobCollisionMethod& method, const zSTRING& otherShape = "" );
    virtual void ThisVobAddedToWorld( zCWorld* );
    virtual void ThisVobRemovedFromWorld( zCWorld* );
    virtual void Archive( zCArchiver& ar );
    virtual void Unarchive( zCArchiver& ar );
    virtual ~zCVobPhysical();
  };
}