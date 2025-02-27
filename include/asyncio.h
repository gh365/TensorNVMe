#pragma once

#include <fcntl.h>
#include <functional>
#include <torch/torch.h>

using callback_t = std::function<void()>;

enum IOType
{
    WRITE,
    READ
};

enum WaitType
{
    WAIT,
    NOWAIT
};

struct IOData
{
    IOType type;
    callback_t callback;
    const iovec *iov;

    IOData(IOType type, callback_t callback = nullptr, const iovec *iov = nullptr) : type(type), callback(callback), iov(iov) {}
    ~IOData()
    {
        if (iov)
            delete iov;
    }
};

class AsyncIO
{
public:
    virtual ~AsyncIO() = default;

    virtual void write(int fd, void *buffer, size_t n_bytes, unsigned long long offset, callback_t callback) = 0;
    virtual void read(int fd, void *buffer, size_t n_bytes, unsigned long long offset, callback_t callback) = 0;
    virtual void writev(int fd, const iovec *iov, unsigned int iovcnt, unsigned long long offset, callback_t callback) = 0;
    virtual void readv(int fd, const iovec *iov, unsigned int iovcnt, unsigned long long offset, callback_t callback) = 0;

    virtual void get_event(WaitType wt) = 0;
    virtual void sync_write_events() = 0;
    virtual void sync_read_events() = 0;
    virtual void register_h2d(unsigned int num_tensors) = 0;
    virtual void register_tasks(unsigned int num_tasks) = 0;
    virtual void sync_h2d() = 0;
    virtual void synchronize() = 0;

    virtual void register_file(int fd) = 0;
    virtual void write_tensor(int fd, torch::Tensor t, unsigned long long offset, callback_t callback, std::optional<torch::Tensor> pinned) = 0;
};