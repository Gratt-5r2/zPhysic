// Supported with union (c) 2020 Union team

// User API for oCNpc
// Add your methods here

void ThisVobAddedToWorld_Union( zCWorld* homeWorld );

zMAT4 GetCorrectedAbMatrix() {
  zMAT4 trafoWorld = trafoObjToWorld;
  zVEC3 translation = trafoWorld * visual->GetBBox3D().GetCenter();
  trafoWorld.SetTranslation( translation );
  return trafoWorld;
}



void SetCorrectedAbMatrix( const zMAT4& trafo ) {
  zMAT4 trafoWorld = trafo;
  zVEC3 translation = trafoWorld * -visual->GetBBox3D().GetCenter();
  trafoWorld.SetTranslation( translation );
  SetTrafoObjToWorld( trafoWorld );
}