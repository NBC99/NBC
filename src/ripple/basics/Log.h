

#ifndef RIPPLE_BASICS_LOG_H_INCLUDED
#define RIPPLE_BASICS_LOG_H_INCLUDED

#include <ripple/basics/UnorderedContainers.h>
#include <boost/beast/core/string.hpp>
#include <ripple/beast/utility/Journal.h>
#include <boost/filesystem.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <utility>

namespace ripple {

enum LogSeverity
{
    lsINVALID   = -1,   
    lsTRACE     = 0,    
    lsDEBUG     = 1,    
    lsINFO      = 2,    
    lsWARNING   = 3,    
    lsERROR     = 4,    
    lsFATAL     = 5     
};


class Logs
{
private:
    class Sink : public beast::Journal::Sink
    {
    private:
        Logs& logs_;
        std::string partition_;

    public:
        Sink (std::string const& partition,
            beast::severities::Severity thresh, Logs& logs);

        Sink (Sink const&) = delete;
        Sink& operator= (Sink const&) = delete;

        void
        write (beast::severities::Severity level,
               std::string const& text) override;
    };

    
    class File
    {
    public:
        
        File ();

        
        ~File () = default;

        
        bool isOpen () const noexcept;

        
        bool open (boost::filesystem::path const& path);

        /** Close and re-open the system file associated with the log
            This assists in interoperating with external log management tools.
            @return `true` if the file was opened.
        */
        bool closeAndReopen ();

        /** Close the system file if it is open. */
        void close ();

        /** write to the log file.
            Does nothing if there is no associated system file.
        */
        void write (char const* text);

        /** write to the log file and append an end of line marker.
            Does nothing if there is no associated system file.
        */
        void writeln (char const* text);

        /** Write to the log file using std::string. */
        /** @{ */
        void write (std::string const& str)
        {
            write (str.c_str ());
        }

        void writeln (std::string const& str)
        {
            writeln (str.c_str ());
        }
        /** @} */

    private:
        std::unique_ptr <std::ofstream> m_stream;
        boost::filesystem::path m_path;
    };

    std::mutex mutable mutex_;
    std::map <std::string,
        std::unique_ptr<beast::Journal::Sink>,
            boost::beast::iless> sinks_;
    beast::severities::Severity thresh_;
    File file_;
    bool silent_ = false;

public:
    Logs(beast::severities::Severity level);

    Logs (Logs const&) = delete;
    Logs& operator= (Logs const&) = delete;

    virtual ~Logs() = default;

    bool
    open (boost::filesystem::path const& pathToLogFile);

    beast::Journal::Sink&
    get (std::string const& name);

    beast::Journal::Sink&
    operator[] (std::string const& name);

    beast::Journal
    journal (std::string const& name);

    beast::severities::Severity
    threshold() const;

    void
    threshold (beast::severities::Severity thresh);

    std::vector<std::pair<std::string, std::string>>
    partition_severities() const;

    void
    write (beast::severities::Severity level, std::string const& partition,
        std::string const& text, bool console);

    std::string
    rotate();

    /**
     * Set flag to write logs to stderr (false) or not (true).
     *
     * @param bSilent Set flag accordingly.
     */
    void
    silent (bool bSilent)
    {
        silent_ = bSilent;
    }

    virtual
    std::unique_ptr<beast::Journal::Sink>
    makeSink(std::string const& partition,
        beast::severities::Severity startingLevel);

public:
    static
    LogSeverity
    fromSeverity (beast::severities::Severity level);

    static
    beast::severities::Severity
    toSeverity (LogSeverity level);

    static
    std::string
    toString (LogSeverity s);

    static
    LogSeverity
    fromString (std::string const& s);

private:
    enum
    {
        maximumMessageCharacters = 12 * 1024
    };

    static
    void
    format (std::string& output, std::string const& message,
        beast::severities::Severity severity, std::string const& partition);
};

#ifndef JLOG
#define JLOG(x) if (!x) { } else x
#endif


/** Set the sink for the debug journal.

    @param sink unique_ptr to new debug Sink.
    @return unique_ptr to the previous Sink.  nullptr if there was no Sink.
*/
std::unique_ptr<beast::Journal::Sink>
setDebugLogSink(
    std::unique_ptr<beast::Journal::Sink> sink);

/** Returns a debug journal.
    The journal may drain to a null sink, so its output
    may never be seen. Never use it for critical
    information.
*/
beast::Journal
debugLog();

} 

#endif








