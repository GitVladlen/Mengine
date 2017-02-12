#	pragma once

#	include "Interface/RenderSystemInterface.h"

namespace Menge
{
	class TTFAtlasServiceInterface
		: public ServiceInterface
	{
		SERVICE_DECLARE( "TTFAtlasService" )

	public:
		virtual RenderTextureInterfacePtr makeTextureGlyph( uint32_t _width, uint32_t _height, const void * _buffer, uint32_t _pitch, mt::uv4f & _uv ) = 0;
	};
}

#   define TTFATLAS_SERVICE( serviceProvider )\
    ((Menge::TTFAtlasServiceInterface *)SERVICE_GET(serviceProvider, Menge::TTFAtlasServiceInterface))
