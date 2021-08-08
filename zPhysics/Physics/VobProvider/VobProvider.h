// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
  bool ProviderSortFuncByVob( class zPhysicalVobProvider* const& left, zCVob& const right );
  bool ProviderSortFunc( class zPhysicalVobProvider* const& left, class zPhysicalVobProvider* const& right );

  enum zPhysicalVobProviderType {
    zVOB_PROVIDER_PHYSICAL_TRAFO,
    zVOB_PROVIDER_VOB_TRAFO
  };

  class zPhysicalVobProvider : public zPhysicalObjectBase {
    static ArraySorted<zPhysicalVobProvider*, ProviderSortFunc> Providers;
    zPhysicalMesh* PhysicalMesh;
    union {
      zCVob* ConnectedVob;
      class zCVobPhysical* ConnectedPhysicalVob;
    };
    zMAT4 SavedVobTrafo;
    zPhysicalVobProviderType Type;
    bool GizmoModeEnabled;
    bool InternalProvider;
    bool UpdateByGizmo();
    bool IsVobPhysicsEnabled();
  public:

    zPhysicalVobProvider( zPhysicalMesh* physicalMesh, zCVob* connectedVob, const zPhysicalVobProviderType& type = zPhysicalVobProviderType::zVOB_PROVIDER_PHYSICAL_TRAFO );
    void Update();
    void SetProviderType( const zPhysicalVobProviderType& type );
    void SetVelocity( const zVEC3& vec );
    void SetPosition( const zVEC3& vec );
    void SetMatrix( const zMAT4& trafo );
    zPhysicalMesh* GetPhysicalMesh() const;
    zCVob* GetConnectedVob() const;
    virtual btRigidBody* GetRigidBody();
    virtual btSoftBody* GetSoftBody();
    virtual btCollisionObject* GetCollisionObject();
    virtual ~zPhysicalVobProvider();
    static zPhysicalVobProvider* FindProvider( zCVob* vob );
    static void UpdateAllProvider();
    static void DeleteAllProvider();
  };


  class zPhysicalNpcProvider : public zPhysicalObjectBase {
    static Array<zPhysicalNpcProvider*> Providers;
    zPhysicalMesh* PhysicalMesh;
    oCNpc* ConnectedNpc;
    zVEC3 NextPosition;
  public:

    zPhysicalNpcProvider( oCNpc* npc );
    void Update();
    void PostUpdate();
    virtual btRigidBody* GetRigidBody();
    virtual btSoftBody* GetSoftBody();
    virtual btCollisionObject* GetCollisionObject();
    virtual ~zPhysicalNpcProvider();
    static void UpdateAllProvider();
    static void PostUpdateAllProvider();
  };
}