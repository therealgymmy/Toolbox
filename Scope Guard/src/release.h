#ifndef RELEASE_H
#define RELEASE_H

#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>

//
// release for single iterator with custom deleter
//
template <typename Deleter, typename Iterator>
inline
void release(Iterator itr, const Deleter &deleter) {
    deleter(itr);
}

//
// release for iterator range with custom deleter
//
template <typename Deleter, typename Iterator>
inline
void release(Iterator begin, Iterator end, const Deleter &deleter) {
    for (Iterator itr = begin; itr != end; ++itr) release(itr, deleter);
}

//
// release for vector<PointerType, Allocator>
//
template <template <typename...> class C2,
          typename V, typename Alloc>
inline
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
inline
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
inline
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
inline
typename
std::enable_if<std::is_class<C2<K, V, Comp, Alloc>>::value &&
               !std::is_pointer<K>::value &&
               std::is_pointer<V>::value,
               void>::type
release(const C2<K, V, Comp, Alloc> &container) {
    for (auto itr : container) delete itr.second;
}

#endif
