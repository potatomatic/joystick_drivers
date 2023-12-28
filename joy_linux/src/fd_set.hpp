#pragma once

#include <sys/select.h>

#include <system_error>


namespace joy_linux
{
    /**
     * @brief Thin wrapper for @a fd_set
     * See https://man7.org/linux/man-pages/man2/select.2.html
    */
    class FdSet
    :   public ::fd_set
    {
    public:
        /**
         * @brief Create an empty @a fd_set
        */
        explicit FdSet() noexcept
        {
            FD_ZERO(this);
        }

        /// @brief Create @a fd_set and add a descriptor to it
        ///
        /// @param fd file descriptor
        ///
        explicit FdSet(int fd) noexcept
        :   FdSet {}
        {
            set(fd);
        }

        /// @brief Add file descriptor to set
        ///
        /// @param fd file descriptor to add.
        ///
        void set(int fd) noexcept
        {
            FD_SET(fd, this);
        }

        /// @brief Test if file descriptor is present in the set.
        ///
        /// @param fd file descriptor
        ///
        /// @return true if the file descriptor is present in the set, and false if it is not.
        ///
        bool isset(int fd) noexcept
        {
            return FD_ISSET(fd, this);
        }
    };

    /// @brief Thin wrapper for @a select(),
    /// see https://man7.org/linux/man-pages/man2/select.2.html
    ///
    /// @param nfds the highest-numbered file descriptor in any of the three sets
    ///   @a readfds, @a writefds, @a exceptfds, plus 1
    /// @param readfds the file descriptors in this set are watched to see if they are ready for reading. Can be nullptr.
    /// @param writefds the file descriptors in this set are watched to see if they are ready for writing. Can be nullptr.
    /// @param exceptfds the file descriptors in this set are watched for "exceptional conditions". Can be nullptr.
    /// @param timeout specifies the interval that the function should block waiting for a file descriptor to become ready.
    ///
    /// @return the number of file descriptors contained in the three returned descriptor sets.
    ///   The return value may be zero if the timeout expired before any file descriptors became ready.
    ///
    /// @throw @a std::system_error on failure
    ///
    inline std::size_t select(std::size_t nfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, timeval const& timeout)
    {
        timeval writeable_timeout = timeout;
        auto result = ::select(nfds, readfds, writefds, exceptfds, &writeable_timeout);
        if (result == -1)
            throw std::system_error {errno, std::system_category(), "select() failed"};

        return result;
    }
}