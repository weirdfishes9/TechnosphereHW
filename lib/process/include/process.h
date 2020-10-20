#ifndef PROC
#define PROC

#include <string>
#include <vector>

namespace process
{

class Process {
    pid_t fork_pid_;
    int pipe_r_[2];
    int pipe_w_[2];
public:
    explicit Process(const std::string& path, 
        const std::vector<std::string> &argv);
    ~Process();

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    void closeStdin();

    void close();
};

} //namespace process

#endif
