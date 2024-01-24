#include "force_feedback.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <system_error>


namespace joy_linux
{
    ForceFeedback::ForceFeedback(std::string const& device_name)
    :   fd_ {open(device_name.c_str(), O_RDWR)}
    {
        if (fd_ < 0)
            throw std::system_error {errno, std::system_category(), "Failed to open " + device_name};
    }
}