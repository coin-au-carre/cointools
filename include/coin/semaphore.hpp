#include <mutex>
#include <condition_variable>

namespace coin {

class semaphore {
public:
    using native_handle_type = std::condition_variable::native_handle_type;

    explicit semaphore(size_t n = 0);
    semaphore(const semaphore&) = delete;
    semaphore& operator=(const semaphore&) = delete;

    void notify();
    void wait();
    bool try_wait();
    template<class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& d);
    template<class Clock, class Duration>
    bool wait_until(const std::chrono::time_point<Clock, Duration>& t);

    native_handle_type native_handle();

private:
    size_t                  count;
    std::mutex              mutex;
    std::condition_variable cv;
};

inline semaphore::semaphore(size_t n) : count{n} {}

inline void semaphore::notify() {
    std::lock_guard<std::mutex> lock{mutex};
    ++count;
    cv.notify_one();
}

inline void semaphore::wait() {
    std::unique_lock<std::mutex> lock{mutex};
    cv.wait(lock, [&]{ return count > 0; });
    --count;
}

inline bool semaphore::try_wait() {
    std::lock_guard<std::mutex> lock{mutex};

    if (count > 0) {
        --count;
        return true;
    }

    return false;
}

template<class Rep, class Period>
bool semaphore::wait_for(const std::chrono::duration<Rep, Period>& d) {
    std::unique_lock<std::mutex> lock{mutex};
    auto finished = cv.wait_for(lock, d, [&]{ return count > 0; });

    if (finished)
        --count;

    return finished;
}

template<class Clock, class Duration>
bool semaphore::wait_until(const std::chrono::time_point<Clock, Duration>& t) {
    std::unique_lock<std::mutex> lock{mutex};
    auto finished = cv.wait_until(lock, t, [&]{ return count > 0; });

    if (finished)
        --count;

    return finished;
}

inline semaphore::native_handle_type semaphore::native_handle() {
    return cv.native_handle();
}

} // ns coin
