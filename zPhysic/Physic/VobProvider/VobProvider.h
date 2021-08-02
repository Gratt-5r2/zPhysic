// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class zPhysicalVobProvider {
		static Array<zPhysicalVobProvider*> Providers;
		zPhysicalMesh* PhysicalMesh;
		zCVob* ConnectedVob;
	public:

		zPhysicalVobProvider( zPhysicalMesh* physicalMesh, zCVob* connectedVob );
		void Update();
		void SetVelocity( const zVEC3& vec );
		zPhysicalMesh* GetPhysicalMesh();
		virtual ~zPhysicalVobProvider();
		static void UpdateAllProvider();
		static void DeleteAllProvider();
	};
}