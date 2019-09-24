#pragma once

namespace Icetrix
{
    class Layer
    {
    public:
        virtual bool OnAttach()
        {
			return true;
        }

        virtual bool OnUpdate()
        {
			return true;
        }

        virtual void OnDetach()
        {
        }
    };
}