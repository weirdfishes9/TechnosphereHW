#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "process.h"

std::vector<char*> change_argv(std::string& path, 
        std::vector<std::string>& args){
    std::vector<char*> changed;
    changed.push_back(path.data());
    for (std::string& s : args) {
        changed.push_back(s.data());
    }
    changed.push_back(nullptr);
    return changed;
}

namespace process
{

Process::Process(const std::string& path, 
        const std::vector<std::string>& argv) {
    if ((pipe(pipe_r_) < 0) || (pipe(pipe_w_) < 0)) {
        throw;
    }

    fork_pid_ = fork();
    if (fork_pid_ < 0 ) {
        throw;
    }
    if (fork_pid_ == 0) {
        std::string c_path{path};
        std::vector<std::string> c_args{argv};
        std::vector<char*> params = ::change_argv(c_path, c_args);
        if ((dup2(pipe_w_[0], STDIN_FILENO) < 0) ||
                (dup2(pipe_r_[1], STDOUT_FILENO) < 0)) {
            throw;
        }
        ::close(pipe_r_[0]);
        ::close(pipe_w_[1]);
        execvp(path.c_str(), params.data());
        exit(EXIT_FAILURE);
    }
    ::close(pipe_r_[1]);
    ::close(pipe_w_[0]);
}

Process::~Process() {
    close();
}

size_t Process::write(const void* data, size_t len) {
    size_t were_written = ::write(pipe_w_[1], data, len);
    if (were_written < 0) {
        throw;
    }
    return were_written;
}

void Process::writeExact(const void* data, size_t len) {
    size_t were_written;
    while (len > 0) {
        were_written = ::write(pipe_w_[1], data, len);
        if (were_written < 0) {
            throw;
        }
        len -= were_written;
    }
}

size_t Process::read(void* data, size_t len) {
    size_t were_read = ::read(pipe_r_[0], data, len);
    if (were_read < 0) {
        throw;
    }
    return were_read;
}

void Process::readExact(void* data, size_t len) {
    size_t were_read;
    while (len > 0) {
        were_read = ::read(pipe_r_[0], data, len);
        if (were_read < 0) {
            throw;
        }
        len -= were_read;
    }
}

void Process::closeStdin() {
    ::close(pipe_w_[1]);
}

void Process::close() {
    ::close(pipe_r_[0]);
    ::close(pipe_w_[1]);
    kill(fork_pid_, SIGINT);
    waitpid(fork_pid_, nullptr, 0);
}

} //namespace process