#ifndef SCOPE_H
#define SCOPE_H

//
// NEVER use scope(...) inside a destructor!
// It can lead to unexpected behavior.
//

#include <exception>
#include "scope_helper.h"
#include "scope_container.h"                // scope guard for container types

//
// Macro: `scope`
//
// Syntax:
//
//   scope(condition, {
//       // code block
//   });
//
//   condition -> enum of type ScopeGuardType
//                exit: code block execute on exit from scope
//                success: code block execute on normal exit from scope
//                failure: code block execute on exception-induced exit from scope
//
// Example:
//
//   scope(exit, {
//       delete dynamicAllocatedInteger;
//       delete dynamicAllocatedObject;
//   });
//
//   scope(success, {
//       if (debug) saveToLog();
//   });
//
//   scope(failure, {
//       rollback();
//       delete cache;
//   });
//
#define scope(arg, code) \
    auto AUTOGEN_NAME(scopeguard) = \
        makeScopeGuard(ScopeGuardType::arg, [&]() code)

//
// Macro: `release_container`
//
// Syntax:
//
//   release_container(container_name);
//
#define release_container(container) \
    do {\
        auto AUTOGEN_NAME(scopeguard_container__##container) = \
            makeScopeGuardContainer(container); \
    } while (0)

//
// Macro: `release_container_with_deleter`
//
// Syntax:
//
//   release_container_with_deleter(container_name, iterator_name {
//       // custom clean up code
//   });
//
//   container_name: name of your container variable
//   iterator_name: name of the iterator that you will address in the code block
//
// Example:
//
//   map<int*, pair<int*, object*>> map_container;
//   // ... allocate stuff
//   release_container_with_deleter(map_container, itr, {
//       delete itr->first;
//       delete itr->second.first;
//       delete itr->second.second;
//   });
//
#define release_container_with_deleter(container, iterator, code) \
    do { \
        auto AUTOGEN_NAME(scopeguard_container_custom__##container) = \
            makeScopeGuardContainerCustom(container, [](decltype(container.cbegin()) iterator) code); \
    } while (0)

enum class ScopeGuardType {
    exit, success, failure
};

template <typename F>
class ScopeGuard {

public:

    ScopeGuard(const ScopeGuardType &t, const F &f) : t(t), f(f) {}
    ScopeGuard(ScopeGuardType &&t, F &&f) : t(t), f(f) {}
    ~ScopeGuard() {
        switch (t) {
            case ScopeGuardType::exit:
                f();
                break;

            case ScopeGuardType::success:
                if (!std::uncaught_exception()) f();
                break;

            case ScopeGuardType::failure:
                if (std::uncaught_exception()) f();
                break;
        }
    }

private:

    const ScopeGuardType t;
    const F f;

};

template <typename F>
ScopeGuard<F> makeScopeGuard(ScopeGuardType t, F f) {
    return ScopeGuard<F>(t, f);
}

#endif//SCOPE_H
