#ifndef SCOPE_CONTAINER_H
#define SCOPE_CONTAINER_H

#include "release.h"

//
// A scope guard for container types:
//
//  std::vector
//  std::map
//
template <template <typename...> class C,
          typename... Args>
struct ScopeGuardContainer {

public:

    ScopeGuardContainer(const C<Args...> &container)
        : container(container) {}
    ~ScopeGuardContainer() {
        release(container);
    }

private:

    const C<Args...> &container;

};

template <template <typename...> class C,
          typename... Args>
ScopeGuardContainer<C, Args...>
makeScopeGuardContainer(const C<Args...> &container) {
    return ScopeGuardContainer<C, Args...>(container);
}

//
// A scope guard for containers allowing custom deleters
//
template <typename Deleter,
          template <typename...> class C,
          typename... Args>
struct ScopeGuardContainerCustom {

public:

    ScopeGuardContainerCustom(const C<Args...> &container, Deleter deleter)
        : container(container), deleter(deleter) {}
    ~ScopeGuardContainerCustom() {
        release(container.begin(), container.end(), deleter);
    }

private:

    const C<Args...> &container;
    const Deleter deleter;

};

template <typename Deleter,
          template <typename...> class C,
          typename... Args>
ScopeGuardContainerCustom<Deleter, C, Args...>
makeScopeGuardContainerCustom(const C<Args...> &container, Deleter deleter) {
    return ScopeGuardContainerCustom<Deleter, C, Args...>(container, deleter);
}

#endif
