#ifndef SCOPE_H
#define SCOPE_H

//
// NEVER use scope(...) inside a destructor!
// It can lead to unexpected behavior.
//

#include <exception>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#define AUTOGEN_NAME2(prefix, func, line) \
    __ ## prefix ## __ ## func ## __ ## line

#define AUTOGEN_NAME(prefix, func, line) \
    AUTOGEN_NAME2(prefix, func, line)

#define scope(arg, code) \
    auto AUTOGEN_NAME(scopeguard, __func__, __LINE__) = \
        makeScopeGuard( ScopeGuardType::arg, [&]() code )

enum class ScopeGuardType {
    exit, success, failure
};

template <typename F>
struct ScopeGuard {

    const ScopeGuardType t;
    const F f;

    ScopeGuard(ScopeGuardType t, F f) : t(t), f(f) {}
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
};

template <typename F>
ScopeGuard<F> makeScopeGuard(ScopeGuardType t, F f) {
    return ScopeGuard<F>(t, f);
}

#endif//SCOPE_H
