#	pragma once

#	include "Kernel/Node.h"

namespace Menge
{
	class Isometric
		: public Node
	{
		DECLARE_VISITABLE( Node )

	public:
		Isometric();

	protected:
		void render( const RenderObjectState * _state, unsigned int _debugMask ) override;
	};
}