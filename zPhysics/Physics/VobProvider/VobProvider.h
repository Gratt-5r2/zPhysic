// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	bool ProviderSortFuncByVob( class zPhysicalVobProvider* const& left, zCVob& const right );
	bool ProviderSortFunc( class zPhysicalVobProvider* const& left, class zPhysicalVobProvider* const& right );

	enum zPhysicalVobProviderType {
		zVOB_PROVIDER_PHYSICAL_TRAFO,
		zVOB_PROVIDER_VOB_TRAFO
	};

	class zPhysicalVobProvider : public zPhysicalObjectBase {
		static ArraySorted<zPhysicalVobProvider*, ProviderSortFunc> Providers;
		zPhysicalMesh* PhysicalMesh;
		zCVob* ConnectedVob;
		zPhysicalVobProviderType Type;
		bool EnableVobReference;
	public:

		zPhysicalVobProvider( zPhysicalMesh* physicalMesh, zCVob* connectedVob, const zPhysicalVobProviderType& type = zVOB_PROVIDER_PHYSICAL_TRAFO );
		void Update();
		void SetProviderType( const zPhysicalVobProviderType& type );
		void SetVelocity( const zVEC3& vec );
		void SetPosition( const zVEC3& vec );
		void SetMatrix( const zMAT4& trafo );
		void SetEnableVobReference( bool enable );
		zPhysicalMesh* GetPhysicalMesh() const;
		zCVob* GetConnectedVob() const;
		virtual btRigidBody* GetRigidBody();
		virtual ~zPhysicalVobProvider();
		static zPhysicalVobProvider* FindProvider( zCVob* vob );
		static void UpdateAllProvider();
		static void DeleteAllProvider();
	};
}