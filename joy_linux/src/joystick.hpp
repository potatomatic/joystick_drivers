#pragma once

#include <string>


namespace joy_linux
{
    class Joystick
    {
    public:
        Joystick(std::string const& device_name);
        ~Joystick();

    private:
        int fd_;
    };
}