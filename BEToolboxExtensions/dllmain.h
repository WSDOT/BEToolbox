// dllmain.h : Declaration of module class.
#include "Resource.h"
class CBEToolboxExtensionsModule : public ATL::CAtlDllModuleT< CBEToolboxExtensionsModule >
{
public :
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BETOOLBOXTOOLEXTENSIONS, "{FA007EC4-0C74-4CD9-AC17-48D0EA83A83D}")
};

extern class CBEToolboxExtensionsModule _AtlModule;
