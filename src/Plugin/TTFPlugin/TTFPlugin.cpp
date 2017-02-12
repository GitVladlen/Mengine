#	include "TTFPlugin.h"

#	include "Interface/StringizeInterface.h"

#	include "Core/PixelFormat.h"

#	include "Logger/Logger.h"

//////////////////////////////////////////////////////////////////////////
SERVICE_EXTERN( TTFAtlasService );
//////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( TTF, Menge::TTFPlugin )
//////////////////////////////////////////////////////////////////////////
namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	TTFPlugin::TTFPlugin()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TTFPlugin::_initialize()
	{
		if( FT_Init_FreeType( &m_library ) )
		{
			return false;
		}

		SERVICE_CREATE( m_serviceProvider, TTFAtlasService );
			
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void TTFPlugin::_finalize()
	{
	}
}