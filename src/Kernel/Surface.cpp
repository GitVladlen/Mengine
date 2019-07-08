#include "Surface.h"

#include "Kernel/Logger.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    Surface::Surface()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Surface::~Surface()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool Surface::initialize()
    {
        bool successful = this->_initialize();

        return successful;
    }
    //////////////////////////////////////////////////////////////////////////
    bool Surface::_initialize()
    {
        //Empty;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Surface::activate()
    {
        this->_activate();
    }
    //////////////////////////////////////////////////////////////////////////
    void Surface::deactivate()
    {
        this->_deactivate();
    }
    //////////////////////////////////////////////////////////////////////////
    void Surface::_activate()
    {
        //Empty
    }
    //////////////////////////////////////////////////////////////////////////
    void Surface::_deactivate()
    {
        //Empty
    }
    //////////////////////////////////////////////////////////////////////////
    bool Surface::update( const UpdateContext * _context )
    {
        MENGINE_UNUSED( _context );

        //Empty;

        return false;
    }
}