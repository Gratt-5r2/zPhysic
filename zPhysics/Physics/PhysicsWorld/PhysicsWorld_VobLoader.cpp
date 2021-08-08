// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  inline void AddVobToPhysicWorld( zCVob* vob ) {
    bool hasMeshVisual = vob->GetVisual()->CastTo<zCProgMeshProto>() != Null;
    if( hasMeshVisual ) {
      zPhysicalBvhTriangleMesh* mesh = zPhysicalBvhTriangleMesh::CreatePolyMesh( vob->visual->GetVisualName(), vob->trafoObjToWorld );
      if( mesh ) {
        PhysicalWorld->AddObject( mesh );
        mesh->Release();
      }
    }
    else {
      zTBBox3D bbox = vob->GetBBox3DLocal();
      zVEC3 extents = (bbox.maxs - bbox.mins) * 0.5f;
      zPhysicalBox* mesh = zPhysicalBox::CreateBox( extents, GetVobBBoxTrafo( vob ) );
      PhysicalWorld->AddObject( mesh );
      mesh->Release();
    }
  }


  HOOK Hook_zCVob_ThisVobAddedToWorld PATCH( &zCVob::ThisVobAddedToWorld, &zCVob::ThisVobAddedToWorld_Union );

  inline bool IsNormalVob( zCVob* vob ) {
    int vobType = vob->GetVobType();
    return
      vobType == zVOB_TYPE_NORMAL ||
      vobType == zVOB_TYPE_ITEM   ||
      vobType == zVOB_TYPE_MOB;
  }
  
  void zCVob::ThisVobAddedToWorld_Union( zCWorld* homeWorld ) {
    if( homeWorld == ogame->GetGameWorld() )
      if( visual && collDetectionDynamic && IsNormalVob( this ) )
        AddVobToPhysicWorld( this );

    THISCALL( Hook_zCVob_ThisVobAddedToWorld )(homeWorld);
  };


  inline void AddNpcToPhysicWorld( zCVob* vob ) {
    zPhysicalNpcProvider* provider = new zPhysicalNpcProvider( (oCNpc*)vob );
    PhysicalWorld->AddObject( provider );
    provider->Release();
  }


  HOOK Hook_oCNpc_ThisVobAddedToWorld PATCH( &oCNpc::ThisVobAddedToWorld, &oCNpc::ThisVobAddedToWorld_Union );

  void oCNpc::ThisVobAddedToWorld_Union( zCWorld* homeWorld ) {
    AddNpcToPhysicWorld( this );
    THISCALL( Hook_oCNpc_ThisVobAddedToWorld )(homeWorld);
  }



  HOOK Hook_zCVob_ThisVobRemovedFromWorld PATCH_IF( &zCVob::ThisVobRemovedFromWorld, &zCVob::ThisVobRemovedFromWorld_Union, 0 );

  void zCVob::ThisVobRemovedFromWorld_Union( zCWorld* homeWorld ) {
    THISCALL( Hook_zCVob_ThisVobRemovedFromWorld )(homeWorld);
  };
}