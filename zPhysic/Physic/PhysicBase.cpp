// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  zPhysicalObjectBase::~zPhysicalObjectBase() {

  }

  btTransform Mat4ToBtTransform( zMAT4 zTrafo ) {
    zCQuat zRotation;
    zRotation.Matrix4ToQuat( zTrafo );
    zRotation = zRotation.Inverse();

    btQuaternion btRotation;
    btRotation.set128( *(btSimdFloat4*)&zRotation );

    btTransform btTrafo;
    btTrafo.setIdentity();
    btTrafo.setRotation( btRotation );
    btTrafo.setOrigin( zTrafo.GetTranslation().ToBtVector3() );
    return btTrafo;
  }

  zMAT4 BtTransformToMat4( btTransform btTrafo ) {
    zCQuat vobRotation;
    btQuaternion& meshRotation = btTrafo.getRotation();
    *(btSimdFloat4*)&vobRotation = meshRotation.get128();

    btVector3 meshOrigin = btTrafo.getOrigin();
    zVEC3 vobOrigin = meshOrigin;

    zMAT4 zTrafo;
    zTrafo.MakeIdentity();
    vobRotation.Inverse().QuatToMatrix4( zTrafo );
    zTrafo.SetTranslation( vobOrigin );
    return zTrafo;
  }

  zMAT4 GetVobBBoxTrafo( zCVob* vob ) {
    zMAT4 trafo = vob->trafoObjToWorld;
    zTBBox3D bbox = vob->GetBBox3DLocal();
    zVEC3 offset = bbox.GetCenter();
    zVEC3 vobPositionWorld = trafo.GetTranslation();
    zVEC3 centerOfVob = vobPositionWorld + offset;
    trafo.SetTranslation( centerOfVob );
    cmd << string::Combine( "%i  %i  %i", (int)offset[VX], (int)offset[VY], (int)offset[VY] ) << endl;
    return trafo;
  }
}