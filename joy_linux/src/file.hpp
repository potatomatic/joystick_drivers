#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <system_error>


namespace joy_linux
{
    /**
     * @brief A simple RAII resource manager for a file.
     *
     * Constructor opens the file, destructor closes it.
    */
    class File
    {
    public:
        /**
         * @brief Open a file and store its descriptor.
         *
         * @param file_name name of the file to open
         * @param flags file open flags, same as in open() (https://man7.org/linux/man-pages/man2/open.2.html)
         *
         * @throw @a std::system_error if open() fails
        */
        explicit File(std::string const& file_name, int flags)
        :   fd_ {::open(file_name.c_str(), flags)}
        {
            if (fd_ == -1)
                throw std::system_error(errno, std::system_category(), "open() failed for " + file_name);
        }

        /**
         * @brief Closes the stored descriptor with close()
         * (https://man7.org/linux/man-pages/man2/close.2.html)
        */
        ~File()
        {
            close(fd_);
        }

        /// @brief Write to a file
        /// see https://man7.org/linux/man-pages/man2/write.2.html
        ///
        /// @param buf pointer to the buffer to write
        /// @param n number of bytes to write
        ///
        /// @return number of bytes actually written
        ///
        /// @throw @a std::system_error on failure
        ///
        std::size_t write(void const * buf, std::size_t n)
        {
            auto result = ::write(fd_, buf, n);
            if (result == -1)
                throw std::system_error(errno, std::system_category(), "write() failed");

            return result;
        }

        /// @brief Attempts to read from file descriptor into a buffer.
        /// See https://man7.org/linux/man-pages/man2/read.2.html
        ///
        /// @param buf pointer to the buffer
        /// @param count number of bytes to read
        ///
        /// @return the number of bytes read
        ///
        /// @throw @a std::system_error on failure
        ///
        std::size_t read(void * buf, std::size_t count)
        {
            auto result = ::read(fd_, buf, count);
            if (result == -1)
                throw std::system_error {errno, std::system_category(), "read() failed"};

            return result;
        }

        /// @brief Control device
        /// see https://man7.org/linux/man-pages/man2/ioctl.2.html
        ///
        /// @param request device-dependent request code
        /// @param argp pointer to input/output argument structure
        ///
        /// @return on success, the returned value of @a ::ioctl()
        ///
        /// @throw @a std::system_error on failure
        ///
        int ioctl(unsigned long request, void * argp)
        {
            auto result = ::ioctl(fd_, request, argp);
            if (result == -1)
                throw std::system_error(errno, std::system_category(), "ioctl() failed");

            return result;
        }

        /**
         * @brief Return file descriptor
         *
         * @return the stored file descriptor
        */
        int descriptor() const noexcept
        {
            return fd_;
        }

    private:
        int const fd_;
    };
}