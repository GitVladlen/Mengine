#	include "Win32ThreadIdentity.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	Win32ThreadIdentity::Win32ThreadIdentity( HANDLE _handle )
		: m_handle(_handle)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	HANDLE Win32ThreadIdentity::getHandle() const
	{
		return m_handle;
	}
}