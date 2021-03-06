#include "AstralaxParticlePlugin.h"

#include "AstralaxInterface.h"

#include "AstralaxParticleModule.h"

#include "Kernel/Logger.h"
#include "Kernel/ModuleFactory.h"
#include "Kernel/ConstStringHelper.h"

//////////////////////////////////////////////////////////////////////////
SERVICE_EXTERN( AstralaxService );
SERVICE_EXTERN( AstralaxSystem );
//////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( AstralaxParticlePlugin, Mengine::AstralaxParticlePlugin );
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    AstralaxParticlePlugin::AstralaxParticlePlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    AstralaxParticlePlugin::~AstralaxParticlePlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool AstralaxParticlePlugin::_initializePlugin()
    {
        LOGGER_INFO( "Initializing Astralax Particle System..." );

        SERVICE_CREATE( AstralaxService );
        SERVICE_CREATE( AstralaxSystem );

        this->addModuleFactory( STRINGIZE_STRING_LOCAL( "ModuleAstralaxParticle" )
            , Helper::makeModuleFactory<AstralaxParticleModule>() );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void AstralaxParticlePlugin::_finalizePlugin()
    {
        SERVICE_FINALIZE( AstralaxService );
        SERVICE_FINALIZE( AstralaxSystem );
    }
    //////////////////////////////////////////////////////////////////////////
    void AstralaxParticlePlugin::_destroyPlugin()
    {
        SERVICE_DESTROY( AstralaxService );
        SERVICE_DESTROY( AstralaxSystem );
    }
}