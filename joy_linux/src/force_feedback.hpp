#pragma once

#include <string>


namespace joy_linux
{
    class ForceFeedback
    {
    public:
        explicit ForceFeedback(std::string const& device_name);

    private:
        int const fd_;
    };
}