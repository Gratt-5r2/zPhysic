// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  zCLASS_UNION_DEFINITION( zCVobPhysical, zCVob, 0, 0 );



  zCVobPhysical::zCVobPhysical() : zCVob() {
    Provider = Null;
    IsPhysicEnabled = false;
    Mass = 10.0f;
    Physic = False;
    Friction = 0.1f;
    Restitution = 0.1f;
  }



  void zCVobPhysical::PhysicEnable() {
    bool canEnablePhysic = Physic == 1 || (Physic == 2 && player->homeWorld);

    if( !Provider || IsPhysicEnabled || !canEnablePhysic )
      return;

    PhysicalWorld->AddObject( Provider );
    IsPhysicEnabled = true;
    cmd << Col16( CMD_GREEN | CMD_INT ) << "Physic enabled" << Col16() << endl;
  }



  void zCVobPhysical::PhysicDisable() {
    if( !Provider || !IsPhysicEnabled )
      return;

    IsPhysicEnabled = false;
    PhysicalWorld->RemoveObject( Provider );
    cmd << Col16( CMD_YELLOW | CMD_INT ) << "Physic disabled" << Col16() << endl;
  }



  zPhysicalMesh* zCVobPhysical::CreatePhysicalMesh( const zTPhysicalVobCollisionMethod& method, const zSTRING& otherShape ) {
    if( method == zCOLL_METHOD_VOBSHAPE ) {
      zCProgMeshProto* progMeshProto = visual->CastTo<zCProgMeshProto>();
      if( progMeshProto ) {
        zPhysicalHullMesh* physicalMesh = zPhysicalHullMesh::CreatePolyMesh( progMeshProto->GetVisualName(), trafoObjToWorld, Mass, true );
        return physicalMesh;
      }
    }

    if( method == zCOLL_METHOD_OTHERSHAPE ) {
      zCProgMeshProto* progMeshProto = visual->CastTo<zCProgMeshProto>();
      if( progMeshProto ) {
        zPhysicalHullMesh* physicalMesh = zPhysicalHullMesh::CreatePolyMesh( otherShape, trafoObjToWorld, Mass, true );
        return physicalMesh;
      }
    }

    if( method == zCOLL_METHOD_SPHERE ) {
      zTBBox3D bbox = visual->GetBBox3D();
      zPhysicalSphere* physicalMesh = zPhysicalSphere::CreateSphere( bbox.mins.Distance( bbox.maxs ) * 0.5f, trafoObjToWorld, Mass, true );
      return physicalMesh;
    }

    if( method == zCOLL_METHOD_CYLINDER ) {
      zTBBox3D bbox = visual->GetBBox3D();
      zPhysicalCylinder* physicalMesh = zPhysicalCylinder::CreateCylinder( bbox.maxs - bbox.mins, trafoObjToWorld, Mass, true );
      return physicalMesh;
    }

    if( method == zCOLL_METHOD_CONE ) {
      zTBBox3D bbox = visual->GetBBox3D();
      zPhysicalCone* physicalMesh = zPhysicalCone::CreateCone( (bbox.mins[VX] + bbox.mins[VZ]) * 0.5f, bbox.maxs[VY] - bbox.mins[VY], trafoObjToWorld, Mass, true );
      return physicalMesh;
    }

    // zCOLL_METHOD_BOX
    zTBBox3D bbox = visual->GetBBox3D();
    zPhysicalCylinder* physicalMesh = zPhysicalCylinder::CreateCylinder( bbox.maxs - bbox.mins, trafoObjToWorld, Mass, true );
    return physicalMesh;
  }



  void zCVobPhysical::SetPositionWorld( zVEC3 const& position ) {
    zCVob::SetPositionWorld( position );
    Provider->SetPosition( position );
  }



  void zCVobPhysical::SetTrafoWorld( zMAT4 const& trafo ) {
    zCVob::SetTrafoObjToWorld( trafo );
    Provider->SetMatrix( trafo );
  }



  void zCVobPhysical::SetMass( const float& mass ) {
    Mass = mass;
  }



  void zCVobPhysical::SetVisual( zCVisual* instance ) {
    SetVisual( instance, zCOLL_METHOD_BOX );
  }



  void zCVobPhysical::SetVisual( zCVisual* instance, const zTPhysicalVobCollisionMethod& method, const zSTRING& otherShape ) {
    if( visual == instance )
      return;

    PhysicDisable();
    if( Provider ) {
      Provider->Release();
      Provider = Null;
    }

    if( !instance ) {
      zCVob::SetVisual( Null );
      return;
    }

    zCVob::SetVisual( instance );
    zPhysicalMesh* mesh = CreatePhysicalMesh( method, otherShape );
    if( !mesh )
      return;

    Provider = new zPhysicalVobProvider( mesh, this );
    Provider->SetEnableVobReference( false );
    mesh->Release();

    if( homeWorld && homeWorld == ogame->GetGameWorld() )
      PhysicEnable();
  }



  void zCVobPhysical::SetVisual( zSTRING const& visualName ) {
    SetVisual( visualName, zCOLL_METHOD_BOX );
  }



  void zCVobPhysical::SetVisual( zSTRING const& visualName, const zTPhysicalVobCollisionMethod& method, const zSTRING& otherShape ) {
    if( s_ignoreVisuals )
      return;

    if( visualName.IsEmpty() )
      return SetVisual( Null );

    zSTRING visualNameUC = visualName;
    visualNameUC.Upper();

    if( visual && visual->GetVisualName() == visualNameUC )
      return;

    zCVisual* instance = zCVisual::LoadVisual( visualNameUC );
    if( !instance || instance == visual )
      return;

    SetVisual( instance, method, otherShape );
    instance->Release();
  }

  

  void zCVobPhysical::ThisVobAddedToWorld( zCWorld* world ) {
    if( visual )
      visual->HostVobAddedToWorld( this, homeWorld );

    PhysicEnable();
  }



  void zCVobPhysical::ThisVobRemovedFromWorld( zCWorld* world ) {
    PhysicDisable();
    if( visual )
      visual->HostVobRemovedFromWorld( this, homeWorld );
  }



  void zCVobPhysical::Archive( zCArchiver& ar ) {
    static string enumShape  = "BOX;SPHERE;CYLINDER;CONE;VOB SHAPE;OTHER SHAPE";
    static string enumPhysic = "DISABLED;ENABLED;IN GAME ONLY";
    zCVob::Archive( ar );

    zVEC3 linearVelocity;
    zVEC3 angularVelocity;

    if( Provider ) {
      auto rigidBody  = Provider->GetRigidBody();
      linearVelocity  = rigidBody->getInterpolationLinearVelocity();
      angularVelocity = rigidBody->getInterpolationAngularVelocity();
      Friction        = rigidBody->getFriction();
      Restitution      = rigidBody->getRestitution();
    }

    ar.WriteGroupBegin( "Physics" );

    ar.WriteEnum  ( "Physics", enumPhysic, Physic );
    ar.WriteFloat ( "Mass",                Mass );
    ar.WriteFloat ( "Friction",            Friction );
    ar.WriteFloat ( "Restitution",         Restitution );
    ar.WriteVec3  ( "LinearVelocity",      linearVelocity );
    ar.WriteVec3  ( "AngularVelocity",     angularVelocity );
    ar.WriteEnum  ( "Shape", enumShape,    Shape );
    ar.WriteString( "OtherShape",          OtherShape );

    ar.WriteGroupEnd( "Physics" );
  }



  void zCVobPhysical::Unarchive( zCArchiver& ar ) {
    zVEC3 linearVelocity;
    zVEC3 angularVelocity;

    zCVob::Unarchive( ar );

    ar.ReadInt   ( "Physic",          Physic );
    ar.ReadFloat ( "Mass",            Mass );
    ar.ReadFloat ( "Friction",        Friction );
    ar.ReadFloat ( "Restitution",     Restitution );
    ar.ReadVec3  ( "LinearVelocity",  linearVelocity );
    ar.ReadVec3  ( "AngularVelocity", angularVelocity );
    ar.ReadInt   ( "Shape",           Shape );
    ar.ReadString( "OtherShape",      OtherShape );

    zCVisual* visualCopy = visual;
    visualCopy->AddRef();
    SetVisual( Null );
    SetVisual( visualCopy, (zTPhysicalVobCollisionMethod)Shape, OtherShape );

    if( Provider ) {
      auto rigidBody = Provider->GetRigidBody();
      rigidBody->setInterpolationLinearVelocity( linearVelocity.ToBtVector3() );
      rigidBody->setInterpolationAngularVelocity( angularVelocity.ToBtVector3() );
      rigidBody->setFriction( Friction );
      rigidBody->setRestitution( Restitution );
    }
  }



  zCVobPhysical::~zCVobPhysical() {
    if( Provider ) {
      PhysicDisable();
      Provider->Release();
    }
  }
}