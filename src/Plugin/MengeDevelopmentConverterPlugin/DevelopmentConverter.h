#	pragma once

#	include "Interface/ConverterInterface.h"

namespace Menge
{
	class DevelopmentConverter
        : public ServantBase<ConverterInterface>
	{
	public:
		DevelopmentConverter();
		~DevelopmentConverter();

	public:
		const String & getConvertExt() const override;

	public:
		void setOptions( ConverterOptions * _options ) override;

	public:
		bool validateVersion( const InputStreamInterfacePtr & _stream ) const override;

	protected:
		ConverterOptions m_options;

		String m_convertExt;
	};
}
