// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  inline void AddVobToPhysicWorld( zCVob* vob ) {
		// if( vob->GetVobType() == zVOB_TYPE_NSC ) {
		// 	zPhysicalCylinder* cylinder = zPhysicalCylinder::CreateCylinder( zVEC3( 25.0f, 25.0f, 180.0f ), vob->trafoObjToWorld );
		// 	zPhysicalVobProvider* provider = new zPhysicalVobProvider( cylinder, vob, zVOB_PROVIDER_VOB_TRAFO );
		// 	PhysicalWorld->AddObject( provider );
		// 	cylinder->Release();
		// 	provider->Release();
		// 	return;
		// }
		
		if( vob->GetVobType() == zVOB_TYPE_ITEM && vob->visual->CastTo<zCProgMeshProto>() ) {
			cmd << vob->visual->GetVisualName() << endl;
			zPhysicalHullMesh* sphere = zPhysicalHullMesh::CreatePolyMesh( vob->visual->GetVisualName(), vob->trafoObjToWorld, 10.0f );
			zPhysicalVobProvider* provider = new zPhysicalVobProvider( sphere, vob );
			PhysicalWorld->AddObject( provider );
			sphere->Release();
			provider->Release();
			return;
		}

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
			zVEC3 extents = (bbox.maxs - bbox.mins) / 2.0f;
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
			vobType == zVOB_TYPE_MOB    ||
			vobType == zVOB_TYPE_NSC;
	}
	
	void zCVob::ThisVobAddedToWorld_Union( zCWorld* homeWorld ) {
    if( homeWorld == ogame->GetGameWorld() )
      if( visual && collDetectionDynamic && IsNormalVob( this ) )
        AddVobToPhysicWorld( this );

		THISCALL( Hook_zCVob_ThisVobAddedToWorld )(homeWorld);
	};



	HOOK Hook_zCVob_ThisVobRemovedFromWorld PATCH( &zCVob::ThisVobRemovedFromWorld, &zCVob::ThisVobRemovedFromWorld_Union );

	void zCVob::ThisVobRemovedFromWorld_Union( zCWorld* homeWorld ) {
		if( homeWorld == ogame->GetGameWorld() ) {
			// cmd << "s" << endl;
			zPhysicalVobProvider* provider = zPhysicalVobProvider::FindProvider( this );
			if( provider ) {
				cmd << "Remove shit from wld" << endl;
				PhysicalWorld->RemoveObject( provider );
			}
		}

		THISCALL( Hook_zCVob_ThisVobRemovedFromWorld )(homeWorld);
	};





	HOOK Hook_zCVob_EndMovement PATCH( &zCVob::EndMovement, &zCVob::EndMovement_Union );

	void zCVob::EndMovement_Union( int hitTrafoChanged ) {
		THISCALL( Hook_zCVob_EndMovement )(hitTrafoChanged);
		// zPhysicalVobProvider* provider = zPhysicalVobProvider::FindPrivoder( this );
		// if( provider )
		// 	provider->GetRigidBody()->setWorldTransform( Mat4ToBtTransform( trafoObjToWorld ) );
	}
}