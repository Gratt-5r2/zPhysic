// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	Array<zPhysicalMesh*> PhysicalMeshQueue;

  inline void AddVobToPhysicWorld( zCVob* vob ) {
		if( vob->GetVobType() == zVOB_TYPE_NSC ) {
			zPhysicalCylinder* cylinder = zPhysicalCylinder::CreateCylinder( zVEC3( 25.0f, 25.0f, 180.0f ), vob->trafoObjToWorld );
			zPhysicalVobProvider* provider = new zPhysicalVobProvider( cylinder, vob, zVOB_PROVIDER_VOB_TRAFO );
			return;
		}

		bool hasMeshVisual = vob->GetVisual()->CastTo<zCProgMeshProto>() != Null;
		
		if( hasMeshVisual ) {
			zPhysicalBvhTriangleMesh* mesh = zPhysicalBvhTriangleMesh::CreatePolyMesh( vob->visual->GetVisualName(), vob->trafoObjToWorld );
			if( mesh )
        PhysicalMeshQueue += mesh;
		}
		else {
			zTBBox3D bbox = vob->GetBBox3DLocal();
			zVEC3 extents = (bbox.maxs - bbox.mins) / 2.0f;
			zPhysicalBox* mesh = zPhysicalBox::CreateBox( extents, GetVobBBoxTrafo( vob ) );
			PhysicalMeshQueue += mesh;
		}
	}




	HOOK Hook_zCVob_ThisVobAddedToWorld PATCH( &zCVob::ThisVobAddedToWorld, &zCVob::ThisVobAddedToWorld_Union );

	inline bool IsNormalVob( zCVob* vob ) {
		int vobType = vob->GetVobType();
		return
			vobType == zVOB_TYPE_NORMAL ||
			vobType == zVOB_TYPE_ITEM ||
			vobType == zVOB_TYPE_MOB ||
			vobType == zVOB_TYPE_NSC;
	}
	
	void zCVob::ThisVobAddedToWorld_Union( zCWorld* homeWorld ) {
		if( visual && collDetectionDynamic && IsNormalVob( this ) )
			AddVobToPhysicWorld( this );

		THISCALL( Hook_zCVob_ThisVobAddedToWorld )(homeWorld);
	};



	HOOK Hook_zCVob_ThisVobRemovedFromWorld PATCH( &zCVob::ThisVobRemovedFromWorld, &zCVob::ThisVobRemovedFromWorld_Union );

	void zCVob::ThisVobRemovedFromWorld_Union( zCWorld* homeWorld ) {

		THISCALL( Hook_zCVob_ThisVobRemovedFromWorld )(homeWorld);
	};
}