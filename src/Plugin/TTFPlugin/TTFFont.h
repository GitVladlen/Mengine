#	pragma once

#	include "TTFServiceInterface.h"

#	include "Interface/MemoryInterface.h"
#	include "Interface/RenderSystemInterface.h"
#	include "Interface/TextInterface.h"

#	include "Config/String.h"

#	include "Factory/FactorablePtr.h"

#	include "Kernel/Servant.h"

#   include "Core/ServantBase.h"

#	include "ft2build.h"
#	include "freetype/freetype.h"
#	include "freetype/ftglyph.h"

#	include "Math/uv4.h"

#	include "stdex/stl_map.h"

namespace Menge
{
#	define MENGINE_TTF_FONT_GLYPH_HASH_SIZE 32

	class TTFFont
		: public ServantBase<TextFontInterface>
	{
	public:
		TTFFont();
		~TTFFont();

	public:
		bool initialize( FT_Library _library, const MemoryInterfacePtr & _memory, float _height );

	public:
		U32String prepareText( const String & _text ) override;
		bool hasGlyph( GlyphCode _char ) const override;
		bool getGlyph( GlyphCode _char, GlyphCode _next, Glyph * _glyph ) const override;

	protected:
		float getFontHeight() const override;

	protected:
		bool prepareGlyph_( uint32_t _ch );	

	protected:
		FT_Library m_library;

        MemoryInterfacePtr m_memory;

		FT_Face m_face;

		float m_height;
		float m_ascender;
		float m_advance;
		
		typedef stdex::vector<TTFGlyph> TMapTTFGlyphs;
		TMapTTFGlyphs m_glyphsHash[MENGINE_TTF_FONT_GLYPH_HASH_SIZE];
	};

	typedef stdex::intrusive_ptr<TTFFont> TTFFontPtr;
}