#pragma once

#include <thread>

namespace coin {

namespace _impl {

class ThreadGuard {
public:
    enum class DtorAction { join, detach };
    
    ThreadGuard(std::thread&& t, DtorAction a)
        : _t(std::move(t)), _action(a) {}

    ~ThreadGuard() {
        if (_t.joinable()) {
            if (_action == DtorAction::join) {
                _t.join();
            } 
            else {
                _t.detach();
            }
        }
    }

    ThreadGuard(ThreadGuard&&)            = default;
    ThreadGuard& operator=(ThreadGuard&&) = default;

    std::thread& get() { return _t; }

private:
    std::thread _t;
    DtorAction  _action;
}; // ThreadGuard

} // ns _impl

using ThreadGuard = _impl::ThreadGuard;
using Action = ThreadGuard::DtorAction;


} // ns mq


