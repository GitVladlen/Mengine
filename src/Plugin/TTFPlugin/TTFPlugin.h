#	pragma once

#	include "Core/PluginBase.h"

#	include "ft2build.h"
#	include "freetype/freetype.h"
#	include "freetype/ftglyph.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	class TTFPlugin
		: public PluginBase
	{
		PLUGIN_DECLARE( "TTF" )

	public:
		TTFPlugin();

	protected:
		bool _initialize() override;
		void _finalize() override;

	protected:
		FT_Library m_library;
	};
}