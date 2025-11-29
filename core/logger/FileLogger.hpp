#include <filesystem>
#include <fstream>
#include <string>

#include "ILogger.hpp"

namespace logger {

class FileLogger {
   public:
    using path_t = std::filesystem::path;
    FileLogger(const path_t& filePath);

    template <IMessage TMessage>
    void log(TMessage message);
    void log(const std::string& message);

    ~FileLogger();

   private:
    const std::filesystem::path _fileName;
    std::ofstream _file;
};


template <IMessage TMessage>
inline void FileLogger::log(TMessage message)
{
    log('[' + message.type() + "] " + message.what());
}

}  // namespace logger