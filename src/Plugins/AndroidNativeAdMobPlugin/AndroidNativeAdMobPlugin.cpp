#include "AndroidNativeAdMobPlugin.h"

#include "AndroidNativeAdMobModule.h"

#include "Kernel/ModuleFactory.h"
#include "Kernel/ConstStringHelper.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( AndroidNativeAdMob, Mengine::AndroidNativeAdMobPlugin )
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    AndroidNativeAdMobPlugin::AndroidNativeAdMobPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    AndroidNativeAdMobPlugin::~AndroidNativeAdMobPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeAdMobPlugin::_initializePlugin()
    {
        this->addModuleFactory( STRINGIZE_STRING_LOCAL( "ModuleAndroidAdMob" )
            , Helper::makeModuleFactory<AndroidNativeAdMobModule>() );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void AndroidNativeAdMobPlugin::_finalizePlugin()
    {
    }
}
