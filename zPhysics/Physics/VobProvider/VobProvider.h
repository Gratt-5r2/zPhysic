// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	enum zPhysicalVobProviderType {
		zVOB_PROVIDER_PHYSICAL_TRAFO,
		zVOB_PROVIDER_VOB_TRAFO
	};

	class zPhysicalVobProvider {
		static Array<zPhysicalVobProvider*> Providers;
		zPhysicalMesh* PhysicalMesh;
		zCVob* ConnectedVob;
		zPhysicalVobProviderType Type;
	public:

		zPhysicalVobProvider( zPhysicalMesh* physicalMesh, zCVob* connectedVob, const zPhysicalVobProviderType& type = zVOB_PROVIDER_PHYSICAL_TRAFO );
		void Update();
		void SetProviderType( const zPhysicalVobProviderType& type );
		void SetVelocity( const zVEC3& vec );
		zPhysicalMesh* GetPhysicalMesh();
		virtual ~zPhysicalVobProvider();
		static void UpdateAllProvider();
		static void DeleteAllProvider();
	};
}