#pragma once

#include "Tasks/TaskHttpBase.h"

namespace Mengine
{
    class TaskHttpPost
        : public TaskHttpBase
    {
    public:
        TaskHttpPost( const String & _url, const cURLPostParams & _params, int32_t _timeout, const LambdacURLReceiver & _lambda );
        ~TaskHttpPost() override;

    protected:
        bool _onRun() override;

    protected:
        cURLPostParams m_params;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef GOAP::IntrusivePtr<TaskHttpPost> TaskHttpPostPtr;
    //////////////////////////////////////////////////////////////////////////
}
