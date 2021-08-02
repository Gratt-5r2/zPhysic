// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

  inline void AddVobToPhysicWorld( zCVob* vob ) {
		bool hasMeshVisual = vob->GetVisual()->CastTo<zCProgMeshProto>() != Null;
		
		if( hasMeshVisual ) {
			zPhysicalBvhTriangleMesh* mesh = zPhysicalBvhTriangleMesh::CreatePolyMesh( vob->visual->GetVisualName(), vob->trafoObjToWorld );
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

	void zCVob::ThisVobAddedToWorld_Union( zCWorld* homeWorld ) {
		if( visual && collDetectionDynamic )
			AddVobToPhysicWorld( this );

		THISCALL( Hook_zCVob_ThisVobAddedToWorld )(homeWorld);
	};



	HOOK Hook_zCVob_ThisVobRemovedFromWorld PATCH( &zCVob::ThisVobRemovedFromWorld, &zCVob::ThisVobRemovedFromWorld_Union );

	void zCVob::ThisVobRemovedFromWorld_Union( zCWorld* homeWorld ) {

		THISCALL( Hook_zCVob_ThisVobRemovedFromWorld )(homeWorld);
	};
}