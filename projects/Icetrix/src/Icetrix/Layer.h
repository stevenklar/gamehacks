#pragma once

namespace Icetrix
{
    class Layer
    {
    public:
        Layer();
        ~Layer();

        virtual bool OnAttach()
        {
        }

        virtual bool OnUpdate()
        {
        }

        virtual void OnDetach()
        {
        }
    };
}