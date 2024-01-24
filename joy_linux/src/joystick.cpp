#include "joystick.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <system_error>


namespace joy_linux
{
    Joystick::Joystick(std::string const& device_name)
    :   fd_ {open(device_name.c_str(), O_RDONLY)}
    {
        if (fd_ < 0)
            throw std::system_error {errno, std::system_category(), "Failed to open " + device_name};

        // There seems to be a bug in the driver or something where the
        // initial events that are to define the initial state of the
        // joystick are not the values of the joystick when it was opened
        // but rather the values of the joystick when it was last closed.
        // Opening then closing and opening again is a hack to get more
        // accurate initial state data.
        close(fd_);
        fd_ = open(device_name.c_str(), O_RDONLY);

        if (fd_ < 0)
            throw std::system_error {errno, std::system_category(), "Failed to open " + device_name};
    }


    Joystick::~Joystick()
    {
        close(fd_);
    }
}