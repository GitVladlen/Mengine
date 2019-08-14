#include "NodeDebuggerPlugin.h"

#include "NodeDebuggerModule.h"

#include "Kernel/ModuleFactory.h"
#include "Kernel/ConstStringHelper.h"

//////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( NodeDebugger, Mengine::NodeDebuggerPlugin )
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    NodeDebuggerPlugin::NodeDebuggerPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    NodeDebuggerPlugin::~NodeDebuggerPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool NodeDebuggerPlugin::_initializePlugin()
    {
        this->addModuleFactory( STRINGIZE_STRING_LOCAL( "ModuleNodeDebugger" )
            , Helper::makeModuleFactory<NodeDebuggerModule>() );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void NodeDebuggerPlugin::_finalizePlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void NodeDebuggerPlugin::_destroyPlugin()
    {
    }
}
