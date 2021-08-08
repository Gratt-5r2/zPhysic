
// This file added in headers queue
// File: "Headers.h"

namespace GOTHIC_ENGINE {
  static int g_collDyn  = 0;
  static int g_collStat = 0;

#define PUSH_COLLDET(obj, newVal)         \
  g_collDyn  = obj->collDetectionDynamic; \
  g_collStat = obj->collDetectionStatic;  \
  obj->collDetectionDynamic = False;      \
  obj->collDetectionStatic  = False;

#define POP_COLLDET(obj)                  \
  obj->collDetectionDynamic = g_collDyn;  \
  obj->collDetectionStatic  = g_collStat;


  zPhysicalWorld* PhysicalWorld = Null;


  inline int* FindSpacerMarker() {
    auto list = CPlugin::GetPluginList().GetNext();
    while( list ) {
      HMODULE module = list->GetData()->GetModule();
      int* pointer = (int*)GetProcAddress( module, "SpacerIsActive" );
      if( pointer )
        return pointer;

      list = list->GetNext();
    }

    return Null;
  }

  static bool IsOnEditor() {
    static int* spacerMarker = FindSpacerMarker();
    if( spacerMarker )
      return *spacerMarker;

    return !player || player->homeWorld == Null;
  }
}