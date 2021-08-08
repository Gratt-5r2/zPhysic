// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  zCLASS_UNION_DEFINITION( zCVobPhysical, zCVob, 0, 0 );



  zCVobPhysical::zCVobPhysical() : zCVob() {
    Provider         = Null;
    PhysicsEnabled   = false;
    Mass             = 10.0f;
    Physics          = zPHY_METHOD_DISABLED;
    Friction         = 0.7f;
    Restitution      = 0.1f;
    LinearVelocity   = 0.0f;
    AngularVelocity  = 0.0f;
    Shape            = zCOLL_METHOD_BOX;
  }



  void zCVobPhysical::PhysicsEnable() {
    bool canEnablePhysic = Physics == zPHY_METHOD_ENABLED || zPHY_METHOD_ENABLED_PREVIEW || (Physics == zPHY_METHOD_ENABLED_INGAMEONLY && !IsOnEditor());

    if( !Provider || PhysicsEnabled || !canEnablePhysic )
      return;

    auto rigidBody = Provider->GetRigidBody();
    PhysicalWorld->AddObject( Provider );
    rigidBody->setWorldTransform( Mat4ToBtTransform( trafoObjToWorld ) );
    rigidBody->setFriction( Friction );
    rigidBody->setRestitution( Restitution );
    rigidBody->setInterpolationLinearVelocity( LinearVelocity.ToBtVector3() );
    rigidBody->setInterpolationAngularVelocity( AngularVelocity.ToBtVector3() );
    PhysicsEnabled = true;
    cmd << Col16( CMD_GREEN | CMD_INT ) << "Physics enabled" << Col16() << endl;
  }



  void zCVobPhysical::PhysicsDisable() {
    if( !Provider || !PhysicsEnabled )
      return;

    PhysicsEnabled = false;
    PhysicalWorld->RemoveObject( Provider );
    cmd << Col16( CMD_YELLOW | CMD_INT ) << "Physics disabled" << Col16() << endl;
  }



  bool zCVobPhysical::IsPhysicsEnabled() const {
    return PhysicsEnabled;
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
      zPhysicalCylinder* physicalMesh = zPhysicalCylinder::CreateCylinder( (bbox.maxs - bbox.mins) * 0.5f, trafoObjToWorld, Mass, true );
      return physicalMesh;
    }

    if( method == zCOLL_METHOD_CONE ) {
      zTBBox3D bbox = visual->GetBBox3D();
      zPhysicalCone* physicalMesh = zPhysicalCone::CreateCone( (bbox.mins[VX] + bbox.maxs[VZ]) * 0.5f, bbox.maxs[VY] - bbox.mins[VY], trafoObjToWorld, Mass, true );
      return physicalMesh;
    }

    if( method == zCOLL_METHOD_CAPSULE ) {
      zTBBox3D bbox = visual->GetBBox3D();
      zPhysicalCapsule* physicalMesh = zPhysicalCapsule::CreateCapsule( (bbox.mins[VX] + bbox.maxs[VZ]) * 0.5f, bbox.maxs[VY] - bbox.mins[VY], trafoObjToWorld, Mass, true );
      return physicalMesh;
    }

    // zCOLL_METHOD_BOX as Default
    zTBBox3D bbox = visual->GetBBox3D();
    zPhysicalBox* physicalMesh = zPhysicalBox::CreateBox( (bbox.maxs - bbox.mins) * 0.5f, trafoObjToWorld, Mass, true );
    return physicalMesh;
  }



  void zCVobPhysical::SetPositionWorld( zVEC3 const& position ) {
    zCVob::SetPositionWorld( position );
    if( Provider )
      Provider->SetPosition( position );
  }



  void zCVobPhysical::SetTrafoWorld( zMAT4 const& trafo ) {
    zCVob::SetTrafoObjToWorld( trafo );
    if( Provider )
      Provider->SetMatrix( trafo );
  }



  void zCVobPhysical::SetMass( const float& mass ) {
    Mass = mass;
  }



  zMAT4 zCVobPhysical::GetCorrectedAbMatrix() {
    if( Physics == zPHY_METHOD_ENABLED_PREVIEW )
      return zMAT4::GetIdentity();

    if( Shape >= zCOLL_METHOD_VOBSHAPE )
      return trafoObjToWorld;

    zMAT4 trafoWorld = trafoObjToWorld;
    zVEC3 translation = trafoWorld * BBoxOffset;
    trafoWorld.SetTranslation( translation );
    return trafoWorld;
  }



  void zCVobPhysical::SetCorrectedAbMatrix( const zMAT4& trafo ) {
    if( Physics == zPHY_METHOD_ENABLED_PREVIEW )
      return;

    if( Shape >= zCOLL_METHOD_VOBSHAPE )
      return SetTrafoObjToWorld( trafo );

    zMAT4 trafoWorld = trafo;
    zVEC3 translation = trafoWorld * -BBoxOffset;
    trafoWorld.SetTranslation( translation );
    SetTrafoObjToWorld( trafoWorld );
  }



  void zCVobPhysical::SetVisual( zCVisual* instance ) {
    SetVisual( instance, zCOLL_METHOD_BOX );
  }



  void zCVobPhysical::SetVisual( zCVisual* instance, const zTPhysicalVobCollisionMethod& method, const zSTRING& otherShape ) {
    if( visual == instance )
      return;

    PhysicsDisable();
    if( Provider ) {
      Provider->Release();
      Provider = Null;
    }

    if( !instance ) {
      zCVob::SetVisual( Null );
      return;
    }

    zCVob::SetVisual( instance );
    BBoxOffset = visual->GetBBox3D().GetCenter();
    zPhysicalMesh* mesh = CreatePhysicalMesh( method, otherShape );
    if( !mesh )
      return;

    Provider = new zPhysicalVobProvider( mesh, this );
    mesh->Release();

    if( homeWorld && homeWorld == ogame->GetGameWorld() )
      PhysicsEnable();
  }



  void zCVobPhysical::ApplyPhysics() {
    if( !visual )
      return;

    PhysicsDisable();
    if( Provider ) {
      Provider->Release();
      Provider = Null;
    }

    zPhysicalMesh* mesh = CreatePhysicalMesh( (zTPhysicalVobCollisionMethod)Shape, OtherShape );
    if( !mesh )
      return;

    btRigidBody* rigidBody = mesh->GetRigidBody();
    rigidBody->setWorldTransform( Mat4ToBtTransform( trafoObjToWorld ) );
    rigidBody->setFriction( Friction );
    rigidBody->setRestitution( Restitution );
    rigidBody->setLinearVelocity( LinearVelocity.ToBtVector3() );
    rigidBody->setAngularVelocity( AngularVelocity.ToBtVector3() );

    Provider = new zPhysicalVobProvider( mesh, this );
    mesh->Release();

    if( homeWorld && homeWorld == ogame->GetGameWorld() )
      PhysicsEnable();
  }



  // TODO
  static void DrawPhysicalMeshGrid( int collMethod, btCollisionShape* collisionShape, const btTransform& trafo ) {
    if( collMethod == zCOLL_METHOD_VOBSHAPE || collMethod == zCOLL_METHOD_OTHERSHAPE ) {
      btConvexHullShape* hull = (btConvexHullShape*)collisionShape;

      int numEdges = hull->getNumPoints();
      const btVector3* points = hull->getPoints();
      for( uint i = 0; i < numEdges - 2; i += 3 ) {
        zlineCache->Line3D( trafo * points[i + 0], trafo * points[i + 1], GFX_AQUA, 1 );
        zlineCache->Line3D( trafo * points[i + 1], trafo * points[i + 2], GFX_AQUA, 1 );
        zlineCache->Line3D( trafo * points[i + 2], trafo * points[i + 0], GFX_AQUA, 1 );
      }
    }

    if( collMethod == zCOLL_METHOD_SPHERE ) {
      btSphereShape* sphere = (btSphereShape*)collisionShape;
    }

    if( collMethod == zCOLL_METHOD_CYLINDER ) {
      btCylinderShape* cylinder = (btCylinderShape*)collisionShape;
      btVector3 extents = cylinder->getHalfExtentsWithMargin();
      float radius =  cylinder->getRadius();

      float step = PI * 0.1f;
      for( float angle = step; angle < PI2; angle += step ) {
        btVector3 pointA = btVector3( cos( angle - step ) * extents[VX], extents[VY], sin( angle - step ) * extents[VZ] );
        btVector3 pointB = btVector3( cos( angle        ) * extents[VX], extents[VY], sin( angle        ) * extents[VZ] );
        zlineCache->Line3D( trafo * pointA, trafo * pointB, GFX_AQUA, 0 );
      }

      for( float angle = step; angle < PI2; angle += step ) {
        btVector3 pointA = btVector3( cos( angle - step ) * extents[VX], -extents[VY], sin( angle - step ) * extents[VZ] );
        btVector3 pointB = btVector3( cos( angle        ) * extents[VX], -extents[VY], sin( angle        ) * extents[VZ] );
        btVector3 pointC = btVector3( pointA[VX], extents[VY], pointA[VZ] );
        zlineCache->Line3D( trafo * pointA, trafo * pointC, GFX_AQUA, 0 );
        zlineCache->Line3D( trafo * pointA, trafo * pointC, GFX_AQUA, 0 );
      }
    }

    if( collMethod == zCOLL_METHOD_CONE ) {
      btConeShape* cone = (btConeShape*)collisionShape;
    }

    if( collMethod == zCOLL_METHOD_CAPSULE ) {
      btCapsuleShape* cone = (btCapsuleShape*)collisionShape;
    }

    // zCOLL_METHOD_BOX as Default
    btBoxShape* box = (btBoxShape*)collisionShape;
  }



  inline void RenderVobForce( zCVob* vob, const zMAT4& placeTrafo ) {
    zMAT4 trafoObjToWorld   = vob->trafoObjToWorld;
    int visualAlphaEnabled  = vob->visualAlphaEnabled;
    float visualAlpha       = vob->visualAlpha;
    vob->trafoObjToWorld    = placeTrafo;
    vob->visualAlphaEnabled = True;
    vob->visualAlpha       *= 0.5f;

    zTRenderContext con;
    con.cam                 = zCCamera::activeCam;
    con.vob                 = vob;
    con.world               = vob->homeWorld;

    vob->visual->Render( con );
    vob->trafoObjToWorld    = trafoObjToWorld;
    vob->visualAlphaEnabled = visualAlphaEnabled;
    vob->visualAlpha        = visualAlpha;
  }



  void zCVobPhysical::ProviderCallback() {
    if( Physics == zPHY_METHOD_ENABLED_INGAMEONLY )
      if( LinearVelocity.Length() > 0.0f )
        if( GetDistanceToVob( *ogame->GetCameraVob() ) < 5000.0f )
          zlineCache->Line3D( GetPositionWorld(), GetPositionWorld() + LinearVelocity, GFX_YELLOW, True );

    if( Physics == zPHY_METHOD_ENABLED_PREVIEW ) {

      zTBBox3D bbox;
      btVector3 btMin;
      btVector3 btMax;

      Provider->GetRigidBody()->getAabb( btMin, btMax );

      bbox.mins = btMin;
      bbox.maxs = btMax;
      bbox.Draw( GFX_AQUA );

      zMAT4 trafo = BtTransformToMat4( Provider->GetRigidBody()->getWorldTransform() );
      RenderVobForce( this, trafo );
      DrawPhysicalMeshGrid( Shape, Provider->GetPhysicalMesh()->GetCollisionShape(), Mat4ToBtTransform( trafo ) );
    }
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
    if( !instance )
      return;

    if( instance == visual ) {
      instance->Release();
      return;
    }

    SetVisual( instance, method, otherShape );
    instance->Release();
  }

  

  void zCVobPhysical::ThisVobAddedToWorld( zCWorld* world ) {
    if( visual )
      visual->HostVobAddedToWorld( this, homeWorld );

    PhysicsEnable();
  }



  void zCVobPhysical::ThisVobRemovedFromWorld( zCWorld* world ) {
    PhysicsDisable();
    if( visual )
      visual->HostVobRemovedFromWorld( this, homeWorld );
  }



  void zCVobPhysical::Archive( zCArchiver& ar ) {
    static string enumShape  = "BOX;SPHERE;CYLINDER;CONE;CAPSULE;VOB SHAPE;OTHER SHAPE";
    static string enumPhysic = "DISABLED;ENABLED;IN GAME ONLY;PREVIEW";
    zCVob::Archive( ar );

    if( Provider && PhysicsEnabled && Physics != zPHY_METHOD_ENABLED_PREVIEW ) {
      auto rigidBody  = Provider->GetRigidBody();
      LinearVelocity  = rigidBody->getInterpolationLinearVelocity();
      AngularVelocity = rigidBody->getInterpolationAngularVelocity();
      Friction        = rigidBody->getFriction();
      Restitution     = rigidBody->getRestitution();
    }

    ar.WriteGroupBegin( "Physics" );

    ar.WriteEnum  ( "Enabled", enumPhysic, Physics );
    ar.WriteFloat ( "Mass",                Mass );
    ar.WriteFloat ( "Friction",            Friction );
    ar.WriteFloat ( "Restitution",         Restitution );
    ar.WriteVec3  ( "LinearVelocity",      LinearVelocity );
    ar.WriteVec3  ( "AngularVelocity",     AngularVelocity );
    ar.WriteEnum  ( "Shape", enumShape,    Shape );
    ar.WriteString( "OtherShape",          OtherShape );

    ar.WriteGroupEnd( "Physics" );
  }



  void zCVobPhysical::Unarchive( zCArchiver& ar ) {
    zCVob::Unarchive( ar );

    ar.ReadEnum  ( "Enabled",         Physics );
    ar.ReadFloat ( "Mass",            Mass );
    ar.ReadFloat ( "Friction",        Friction );
    ar.ReadFloat ( "Restitution",     Restitution );
    ar.ReadVec3  ( "LinearVelocity",  LinearVelocity );
    ar.ReadVec3  ( "AngularVelocity", AngularVelocity );
    ar.ReadEnum  ( "Shape",           Shape );
    ar.ReadString( "OtherShape",      OtherShape );

    ApplyPhysics();
  }



  zCVobPhysical::~zCVobPhysical() {
    if( Provider ) {
      PhysicsDisable();
      Provider->Release();
    }
  }
}