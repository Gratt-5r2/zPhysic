// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  enum zTPhysicalVobCollisionMethod {
    zCOLL_METHOD_BOX,
    zCOLL_METHOD_SPHERE,
    zCOLL_METHOD_CYLINDER,
    zCOLL_METHOD_CONE,
    zCOLL_METHOD_CAPSULE,
    zCOLL_METHOD_VOBSHAPE,
    zCOLL_METHOD_OTHERSHAPE
  };


  enum zTPhysicalVobPhysicsMethod {
    zPHY_METHOD_DISABLED,
    zPHY_METHOD_ENABLED,
    zPHY_METHOD_ENABLED_INGAMEONLY,
    zPHY_METHOD_ENABLED_PREVIEW
  };


  class zCVobPhysical : public zCVob {
    zPhysicalVobProvider* Provider;
    bool PhysicsEnabled;

    struct {
      int Physics;
      float Mass;
      float Friction;
      float Restitution;
      zVEC3 LinearVelocity;
      zVEC3 AngularVelocity;
      int Shape;
      zSTRING OtherShape;
    };

    zVEC3 BBoxOffset;
    zPhysicalMesh* CreatePhysicalMesh( const zTPhysicalVobCollisionMethod& method, const zSTRING& otherShape = "" );
  public:

    zCLASS_UNION_DECLARATION( zCVobPhysical );

    zCVobPhysical();
    void PhysicsEnable();
    void PhysicsDisable();
    bool IsPhysicsEnabled() const;
    void SetPositionWorld( zVEC3 const& position );
    void SetTrafoWorld( zMAT4 const& trafo );
    void SetMass( const float& mass );
    zMAT4 GetCorrectedAbMatrix();
    void SetCorrectedAbMatrix( const zMAT4& trafo );
    virtual void SetVisual( zCVisual* instance );
    virtual void SetVisual( zCVisual* instance, const zTPhysicalVobCollisionMethod& method, const zSTRING& otherShape = "" );
    virtual void SetVisual( zSTRING const& visualName );
    virtual void SetVisual( zSTRING const& visualName, const zTPhysicalVobCollisionMethod& method, const zSTRING& otherShape = "" );
    virtual void ApplyPhysics();
    virtual void ProviderCallback();
    virtual void ThisVobAddedToWorld( zCWorld* );
    virtual void ThisVobRemovedFromWorld( zCWorld* );
    virtual void Archive( zCArchiver& ar );
    virtual void Unarchive( zCArchiver& ar );
    virtual ~zCVobPhysical();
  };
}