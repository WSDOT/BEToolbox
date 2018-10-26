// dllmain.h : Declaration of module class.

class CBEToolboxToolExampleModule : public ATL::CAtlDllModuleT< CBEToolboxToolExampleModule >
{
public :
	DECLARE_LIBID(LIBID_BEToolboxToolExampleLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BETOOLBOXTOOLEXAMPLE, "{A974560E-2BC3-4EB5-B35B-2CBBB22C828D}")
};

extern class CBEToolboxToolExampleModule _AtlModule;
