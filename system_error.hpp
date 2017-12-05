#pragma once
#include "type_traits.hpp"
#include "string.hpp"
#include "stdexcept.hpp"

namespace std
{
    class error_condition;
    class error_code;

    class error_category
    {
    public:
        constexpr error_category() noexcept { }
        inline virtual ~error_category() { }

        error_category(const error_category& other) = delete;

    public:
        inline bool operator ==(const error_category& rhs) const noexcept {
            return name() == rhs.name(); // Address equality
        };
        inline bool operator !=(const error_category& rhs) const noexcept {
            return !operator==(rhs);
        }
        inline bool operator <(const error_category& rhs) const noexcept {
            return false; // What is this even supposed to mean? - jsren
        }

    public:
        virtual const char* name() const noexcept = 0;

        virtual error_condition default_error_condition(int code) const noexcept = 0;

        virtual bool equivalent(int code, const error_condition& condition) const noexcept = 0;

        virtual bool equivalent(const error_code& code, int condition) const noexcept = 0;

        virtual string message(int condition) const = 0;
    };

    const std::error_category& generic_category() noexcept;
    const std::error_category& system_category() noexcept;

    class system_error : public runtime_error
    {

    };

    template <class T>
    struct is_error_code_enum : public false_type { };
    template <class T>
    struct is_error_condition_enum : public false_type { };


    enum class errc
    {
        address_family_not_supported,       //  EAFNOSUPPORT
        address_in_use,                     //  EADDRINUSE
        address_not_available,              //  EADDRNOTAVAIL
        already_connected,                  //  EISCONN
        argument_list_too_long,             //  E2BIG
        argument_out_of_domain,             //  EDOM
        bad_address,                        //  EFAULT
        bad_file_descriptor,                //  EBADF
        bad_message,                        //  EBADMSG
        broken_pipe,                        //  EPIPE
        connection_aborted,                 //  ECONNABORTED
        connection_already_in_progress,     //  EALREADY
        connection_refused,                 //  ECONNREFUSED
        connection_reset,                   //  ECONNRESET
        cross_device_link,                  //  EXDEV
        destination_address_required,       //  EDESTADDRREQ
        device_or_resource_busy,            //  EBUSY
        directory_not_empty,                //  ENOTEMPTY
        executable_format_error,            //  ENOEXEC
        file_exists,                        //  EEXIST
        file_too_large,                     //  EFBIG
        filename_too_long,                  //  ENAMETOOLONG
        function_not_supported,             //  ENOSYS
        host_unreachable,                   //  EHOSTUNREACH
        identifier_removed,                 //  EIDRM
        illegal_byte_sequence,              //  EILSEQ
        inappropriate_io_control_operation, //  ENOTTY
        interrupted,                        //  EINTR
        invalid_argument,                   //  EINVAL
        invalid_seek,                       //  ESPIPE
        io_error,                           //  EIO
        is_a_directory,                     //  EISDIR
        message_size,                       //  EMSGSIZE
        network_down,                       //  ENETDOWN
        network_reset,                      //  ENETRESET
        network_unreachable,                //  ENETUNREACH
        no_buffer_space,                    //  ENOBUFS
        no_child_process,                   //  ECHILD
        no_link,                            //  ENOLINK
        no_lock_available,                  //  ENOLCK
        no_message_available,               //  ENODATA
        no_message,                         //  ENOMSG
        no_protocol_option,                 //  ENOPROTOOPT
        no_space_on_device,                 //  ENOSPC
        no_stream_resources,                //  ENOSR
        no_such_device_or_address,          //  ENXIO
        no_such_device,                     //  ENODEV
        no_such_file_or_directory,          //  ENOENT
        no_such_process,                    //  ESRCH
        not_a_directory,                    //  ENOTDIR
        not_a_socket,                       //  ENOTSOCK
        not_a_stream,                       //  ENOSTR
        not_connected,                      //  ENOTCONN
        not_enough_memory,                  //  ENOMEM
        not_supported,                      //  ENOTSUP
        operation_canceled,                 //  ECANCELED
        operation_in_progress,              //  EINPROGRESS
        operation_not_permitted,            //  EPERM
        operation_not_supported,            //  EOPNOTSUPP
        operation_would_block,              //  EWOULDBLOCK
        owner_dead,                         //  EOWNERDEAD
        permission_denied,                  //  EACCES
        protocol_error,                     //  EPROTO
        protocol_not_supported,             //  EPROTONOSUPPORT
        read_only_file_system,              //  EROFS
        resource_deadlock_would_occur,      //  EDEADLK
        resource_unavailable_try_again,     //  EAGAIN
        result_out_of_range,                //  ERANGE
        state_not_recoverable,              //  ENOTRECOVERABLE
        stream_timeout,                     //  ETIME
        text_file_busy,                     //  ETXTBSY
        timed_out,                          //  ETIMEDOUT
        too_many_files_open_in_system,      //  ENFILE
        too_many_files_open,                //  EMFILE
        too_many_links,                     //  EMLINK
        too_many_symbolic_link_levels,      //  ELOOP
        value_too_large,                    //  EOVERFLOW
        wrong_protocol_type,                //  EPROTOTYPE
    };

    template<>
    struct is_error_condition_enum<errc> : true_type { };

}

namespace __abi {
    extern int __ecode_from_enum(std::errc) noexcept;
}

namespace std
{

    class error_code
    {
    private:
        int _value = 0;
        const error_category* _category = &system_category();

    public:
        inline error_code() = default;
        inline error_code(int ec, const error_category& ecat) : _value(ec), _category(&ecat) { }

        template<typename Enum, class=enable_if_t<is_error_condition_enum<Enum>::value>>
        inline error_code(Enum ecode) {
            *this = make_error_code(ecode);
        }
    };

    error_code make_error_code(errc e) noexcept;
    error_condition make_error_condition(errc e) noexcept;

    // Comparison operators:
    bool operator==(const error_code& lhs, const error_code& rhs) noexcept;
    bool operator==(const error_code& lhs, const error_condition& rhs) noexcept;
    bool operator==(const error_condition& lhs, const error_code& rhs) noexcept;
    bool operator==(const error_condition& lhs, const error_condition& rhs) noexcept;
    bool operator!=(const error_code& lhs, const error_code& rhs) noexcept;
    bool operator!=(const error_code& lhs, const error_condition& rhs) noexcept;
    bool operator!=(const error_condition& lhs, const error_code& rhs) noexcept;
    bool operator!=(const error_condition& lhs, const error_condition& rhs) noexcept;

    // Hash support
    template <class T> struct hash;
    template <> struct hash<error_code>;
}
