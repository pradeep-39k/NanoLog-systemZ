//
// Sumit: Summary of changes to orginal NanoLog
//
// - Copied NanoLog cpp and hpp from repo https://github.com/Iyengar111/NanoLog  (at commit 40a53c3)
// - Loglevel: Added DEBUG, renamed CRIT to ERROR. Default INFO.
// - Using Time::getTimestamp(). Own printTimestamp.
// - Removed thread_id, file, function from log lines.
// - Added thread name.
// - Takes timestamp from caller in LOG_INFO etc, instead of making getTimestamp() call.
// - initialize takes thread name and cpu affinity parameter.
// - file extention changed from .txt to .log
// - join() added.
// - Fix for "char buf[100]" not to be treated as string_literal_t
//
//

#ifndef NANO_LOG_SYSTEN_Z_HEADER_GUARD
#define NANO_LOG_SYSTEN_Z_HEADER_GUARD

#include <cstdint>
#include <memory>
#include <string>
#include <iosfwd>
#include <type_traits>

namespace nanolog_systen_z
{
    enum class LogLevel : uint8_t
    {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

    class NanoLogLine
    {
    public:
        NanoLogLine(LogLevel level, int64_t timestamp);
        ~NanoLogLine();

        NanoLogLine(NanoLogLine &&) = default;
        NanoLogLine &operator=(NanoLogLine &&) = default;

        void stringify(std::ostream &os);

        NanoLogLine &operator<<(char arg);
        NanoLogLine &operator<<(int32_t arg);
        NanoLogLine &operator<<(uint32_t arg);
        NanoLogLine &operator<<(int64_t arg);
        NanoLogLine &operator<<(uint64_t arg);
        NanoLogLine &operator<<(double arg);
        NanoLogLine &operator<<(std::string const &arg);

        template <size_t N>
        NanoLogLine &operator<<(const char (&arg)[N])
        {
            encode(string_literal_t(arg));
            return *this;
        }

        template <size_t N>
        NanoLogLine &operator<<(char (&arg)[N]) // Fix for "char buf[100]" not to be treated as string_literal_t. Defining separate template for non-const char[].
        {
            encode(arg);
            return *this;
        }

        template <typename Arg>
        typename std::enable_if<std::is_same<Arg, char const *>::value, NanoLogLine &>::type
        operator<<(Arg const &arg)
        {
            encode(arg);
            return *this;
        }

        template <typename Arg>
        typename std::enable_if<std::is_same<Arg, char *>::value, NanoLogLine &>::type
        operator<<(Arg const &arg)
        {
            encode(arg);
            return *this;
        }

        struct string_literal_t
        {
            explicit string_literal_t(char const *s) : m_s(s) {}
            char const *m_s;
        };

    private:
        char *buffer();

        template <typename Arg>
        void encode(Arg arg);

        template <typename Arg>
        void encode(Arg arg, uint8_t type_id);

        void encode(char *arg);
        void encode(char const *arg);
        void encode(string_literal_t arg);
        void encode_c_string(char const *arg, size_t length);
        void resize_buffer_if_needed(size_t additional_bytes);
        void stringify(std::ostream &os, char *start, char const *const end);

    private:
        size_t m_bytes_used;
        size_t m_buffer_size;
        std::unique_ptr<char[]> m_heap_buffer;
        char m_stack_buffer[256 - 2 * sizeof(size_t) - sizeof(decltype(m_heap_buffer)) - 8 /* Reserved */];
    };

    struct NanoLog
    {
        /*
         * Ideally this should have been operator+=
         * Could not get that to compile, so here we are...
         */
        bool operator==(NanoLogLine &);
    };

    void set_log_level(LogLevel level);

    bool is_logged(LogLevel level);

    /*
     * Non guaranteed logging. Uses a ring buffer to hold log lines.
     * When the ring gets full, the previous log line in the slot will be dropped.
     * Does not block producer even if the ring buffer is full.
     * ring_buffer_size_mb - LogLines are pushed into a mpsc ring buffer whose size
     * is determined by this parameter. Since each LogLine is 256 bytes,
     * ring_buffer_size = ring_buffer_size_mb * 1024 * 1024 / 256
     */
    struct NonGuaranteedLogger
    {
        NonGuaranteedLogger(uint32_t ring_buffer_size_mb_) : ring_buffer_size_mb(ring_buffer_size_mb_) {}
        uint32_t ring_buffer_size_mb;
    };

    /*
     * Provides a guarantee log lines will not be dropped.
     */
    struct GuaranteedLogger
    {
    };

    /*
     * Ensure initialize() is called prior to any log statements.
     * log_directory - where to create the logs. For example - "/tmp/"
     * log_file_name - root of the file name. For example - "nanolog"
     * This will create log files of the form -
     * /tmp/nanolog.1.txt
     * /tmp/nanolog.2.txt
     * etc.
     * log_file_roll_size_mb - mega bytes after which we roll to next log file.
     */
    void initialize(GuaranteedLogger gl, std::string const &log_directory, std::string const &log_file_name, uint32_t log_file_roll_size_mb,
                    std::string const &thread_name /*empty for no thread name*/, int cpuAffinity /*-1 for no cpu affinity*/);
    // void initialize(NonGuaranteedLogger ngl, std::string const &log_directory, std::string const &log_file_name, uint32_t log_file_roll_size_mb,
    //                 std::string const &thread_name /*empty for no thread name*/, int cpuAffinity /*-1 for no cpu affinity*/);

    // Explicitly wait for logger thread to join after flushing all data.
    // Optional call, data is anyway flushed on gracefull program end.
    // Logger should not be called after this call.
    void join();

} // namespace nanolog_systen_z

#define NANO_LOG(LEVEL, timestamp) nanolog_systen_z::NanoLog() == nanolog_systen_z::NanoLogLine(LEVEL, timestamp)
#define LOG_DEBUG(timestamp) nanolog_systen_z::is_logged(nanolog_systen_z::LogLevel::DEBUG) && NANO_LOG(nanolog_systen_z::LogLevel::DEBUG, timestamp)
#define LOG_INFO(timestamp) nanolog_systen_z::is_logged(nanolog_systen_z::LogLevel::INFO) && NANO_LOG(nanolog_systen_z::LogLevel::INFO, timestamp)
#define LOG_WARN(timestamp) nanolog_systen_z::is_logged(nanolog_systen_z::LogLevel::WARN) && NANO_LOG(nanolog_systen_z::LogLevel::WARN, timestamp)
#define LOG_ERROR(timestamp) nanolog_systen_z::is_logged(nanolog_systen_z::LogLevel::ERROR) && NANO_LOG(nanolog_systen_z::LogLevel::ERROR, timestamp)

#endif /* NANO_LOG_HEADER_GUARD */
