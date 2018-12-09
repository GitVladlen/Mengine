#pragma once

namespace Mengine
{
    enum ENotificatorName
    {
        __NOTIFICATOR_BEGIN__ = 0,
        NOTIFICATOR_DEBUG_OPEN_FILE,
        NOTIFICATOR_DEBUG_CLOSE_FILE,
        NOTIFICATOR_CHANGE_WINDOW_RESOLUTION,
        NOTIFICATOR_DEVELOPMENT_RESOURCE_CREATE,
        NOTIFICATOR_RESOURCE_COMPILE,
        NOTIFICATOR_RESOURCE_RELEASE,
        NOTIFICATOR_CHANGE_SCENE_PREPARE_DESTROY,
        NOTIFICATOR_CHANGE_SCENE_DESTROY,
        NOTIFICATOR_CHANGE_SCENE_PREPARE_INITIALIZE,
        NOTIFICATOR_CHANGE_SCENE_INITIALIZE,
        NOTIFICATOR_CHANGE_SCENE_PREPARE_ENABLE,
        NOTIFICATOR_CHANGE_SCENE_ENABLE,
        NOTIFICATOR_CHANGE_SCENE_ENABLE_FINALLY,
        NOTIFICATOR_CHANGE_SCENE_PREPARE_COMPLETE,
        NOTIFICATOR_CHANGE_SCENE_COMPLETE,
        NOTIFICATOR_RESTART_SCENE_PREPARE_DISABLE,
        NOTIFICATOR_RESTART_SCENE_DISABLE,
        NOTIFICATOR_RESTART_SCENE_INITIALIZE,
        NOTIFICATOR_RESTART_SCENE_PREPARE_ENABLE,
        NOTIFICATOR_RESTART_SCENE_ENABLE,
        NOTIFICATOR_RESTART_SCENE_ENABLE_FINALLY,
        NOTIFICATOR_RESTART_SCENE_PREPARE_COMPLETE,
        NOTIFICATOR_RESTART_SCENE_COMPLETE,
        NOTIFICATOR_REMOVE_SCENE_PREPARE_DESTROY,
        NOTIFICATOR_REMOVE_SCENE_DESTROY,
        NOTIFICATOR_REMOVE_SCENE_PREPARE_COMPLETE,
        NOTIFICATOR_REMOVE_SCENE_COMPLETE,
        NOTIFICATOR_CHANGE_LOCALE_PREPARE,
        NOTIFICATOR_CHANGE_LOCALE,
        NOTIFICATOR_CHANGE_LOCALE_POST,
        NOTIFICATOR_DEBUG_TEXT_MODE,
        NOTIFICATOR_CHANGE_TEXT_ALIAS_ARGUMENTS,
        NOTIFICATOR_RELOAD_LOCALE_PREPARE,
        NOTIFICATOR_RELOAD_LOCALE,
        NOTIFICATOR_RELOAD_LOCALE_POST,
        NOTIFICATOR_ENGINE_FINALIZE,
        NOTIFICATOR_ENGINE_ENABLE_PACKAGES,
        __NOTIFICATOR_END__,
    };
}
