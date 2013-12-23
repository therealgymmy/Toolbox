#ifndef SCOPE_H
#define SCOPE_H

//
// NEVER use scope(...) inside a destructor!
// It can lead to unexpected behavior.
//

#include <exception>
#include <type_traits>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

#define AUTOGEN_NAME2(prefix, func, line) \
    AUTOGEN__ ## prefix ## __ ## func ## __ ## line

#define AUTOGEN_NAME(prefix, func, line) \
    AUTOGEN_NAME2(prefix, func, line)

#define scope(arg, code) \
    auto AUTOGEN_NAME(scopeguard, __func__, __LINE__) = \
        makeScopeGuard( ScopeGuardType::arg, [&]() code )

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

    //
    // release for vector<PointerType, Allocator>
    //
    template <template <typename...> class C2,
              typename V, typename Alloc>
    static
    typename
    std::enable_if<std::is_class<C2<V, Alloc>>::value &&
                   std::is_pointer<V>::value,
                   void>::type
    release(const C2<V, Alloc> &container) {
        for (auto elem : container) delete elem;
    }

    //
    // release for map<PointerType, PointerType, Comparator, Allocator>
    //
    template <template <typename...> class C2,
              typename K, typename V,
              typename Comp, typename Alloc>
    static
    typename
    std::enable_if<std::is_class<C2<K, V, Comp, Alloc>>::value &&
                   std::is_pointer<K>::value &&
                   std::is_pointer<V>::value,
                   void>::type
    release(const C2<K, V, Comp, Alloc> &container) {
        for (auto itr : container) {
            delete itr.first;
            delete itr.second;
        }
    }

    //
    // release for map<PointerType, OtherType, Comparator, Allocator>
    //
    template <template <typename...> class C2,
              typename K, typename V,
              typename Comp, typename Alloc>
    static
    typename
    std::enable_if<std::is_class<C2<K, V, Comp, Alloc>>::value &&
                   std::is_pointer<K>::value &&
                   !std::is_pointer<V>::value,
                   void>::type
    release(const C2<K, V, Comp, Alloc> &container) {
        for (auto itr : container) delete itr.first;
    }

    //
    // release for map<OtherType, PointerType, Comparator, Allocator>
    //
    template <template <typename...> class C2,
              typename K, typename V,
              typename Comp, typename Alloc>
    static
    typename
    std::enable_if<std::is_class<C2<K, V, Comp, Alloc>>::value &&
                   !std::is_pointer<K>::value &&
                   std::is_pointer<V>::value,
                   void>::type
    release(const C2<K, V, Comp, Alloc> &container) {
        for (auto itr : container) delete itr.second;
    }
};

template <template <typename...> class C,
          typename... Args>
ScopeGuardContainer<C, Args...> makeScopeGuardContainer(const C<Args...> &container) {
    return ScopeGuardContainer<C, Args...>(container);
}

#endif//SCOPE_H
