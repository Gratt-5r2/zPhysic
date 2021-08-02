
// This file added in headers queue
// File: "Headers.h"

namespace GOTHIC_ENGINE {
#define PUSH_COLLDET(obj, newVal)							\
	int m_collDyn  = obj->collDetectionDynamic;	\
	int m_collStat = obj->collDetectionStatic;	\
	obj->collDetectionDynamic = False;					\
	obj->collDetectionStatic  = False;

#define POP_COLLDET(obj)									\
	obj->collDetectionDynamic = m_collDyn;	\
	obj->collDetectionStatic  = m_collStat;


  zPhysicalWorld* PhysicalWorld = Null;
}