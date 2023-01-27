#include <lyra/xstd/type_list.hpp>

// Only the template itself needs to be available
// to access all its functions by making use of ADL.
//
using lyra::xstd::type_list;

// Provide a total order predicate for types.
//
constexpr auto less = []<typename x, typename y> {
  return sizeof(x) <= sizeof(y);
};

// Check if a given type is an instance of the 'type_list' template.
//
namespace instance = lyra::xstd::instance;
static_assert(instance::type_list<type_list<>>);
static_assert(instance::type_list<type_list<int>>);
static_assert(instance::type_list<type_list<char>>);
static_assert(instance::type_list<type_list<int, char>>);
static_assert(instance::type_list<type_list<int, char, float>>);
static_assert(instance::type_list<  //
              type_list<int, type_list<char, float>, type_list<>>>);
//
static_assert(!instance::type_list<int>);
static_assert(!instance::type_list<float>);
static_assert(!instance::type_list<type_list<>&>);
static_assert(!instance::type_list<const type_list<>>);
static_assert(!instance::type_list<const type_list<>&>);

// Get the number of types inside a type list.
//
static_assert(size(type_list<>{}) == 0);
static_assert(size(type_list<int>{}) == 1);
static_assert(size(type_list<char>{}) == 1);
static_assert(size(type_list<int, char>{}) == 2);
static_assert(size(type_list<int, char, double>{}) == 3);

// Check whether a 'type_list' instance contains no types.
//
static_assert(empty(type_list<>{}));
static_assert(!empty(type_list<int>{}));
static_assert(!empty(type_list<int, char>{}));
static_assert(!empty(type_list<int, char, double>{}));

// Check if a condition holds for all types.
//
constexpr auto correct_alignment = []<typename x> { return alignof(x) >= 4; };
static_assert(for_all(type_list<>{}, correct_alignment));
static_assert(for_all(type_list<float>{}, correct_alignment));
static_assert(!for_all(type_list<char>{}, correct_alignment));
static_assert(for_all(type_list<float, double>{}, correct_alignment));
static_assert(!for_all(type_list<float, char>{}, correct_alignment));
static_assert(!for_all(type_list<short, char>{}, correct_alignment));

// Check if a type exists inside the list with a certain condition.
//
static_assert(!exists(type_list<>{}, correct_alignment));
static_assert(!exists(type_list<char>{}, correct_alignment));
static_assert(exists(type_list<float>{}, correct_alignment));
static_assert(exists(type_list<double>{}, correct_alignment));
static_assert(!exists(type_list<char, short>{}, correct_alignment));
static_assert(exists(type_list<char, float>{}, correct_alignment));
static_assert(exists(type_list<float, double>{}, correct_alignment));

// Check if a 'type_list' instance contains a specific type.
//
static_assert(!contains<int>(type_list<>{}));
static_assert(!contains<char>(type_list<>{}));
static_assert(contains<int>(type_list<int>{}));
static_assert(!contains<int>(type_list<char>{}));
static_assert(contains<int>(type_list<int, char>{}));
static_assert(contains<char>(type_list<int, char>{}));
static_assert(!contains<float>(type_list<int, char>{}));
//
static_assert(!contains(type_list<>{}, type_list<int>{}));
static_assert(!contains(type_list<>{}, type_list<char>{}));
static_assert(contains(type_list<int>{}, type_list<int>{}));
static_assert(!contains(type_list<char>{}, type_list<int>{}));
static_assert(contains(type_list<int, char>{}, type_list<int>{}));
static_assert(contains(type_list<int, char>{}, type_list<char>{}));
static_assert(!contains(type_list<int, char>{}, type_list<float>{}));

// For a simply type equality check.
//
using lyra::xstd::meta::equal;

// Access types of a 'type_list' instance by their index.
//
static_assert(equal<decltype(element<0>(type_list<int>{})), int>);
static_assert(equal<decltype(element<0>(type_list<int, char>{})), int>);
static_assert(equal<decltype(element<1>(type_list<int, char>{})), char>);
static_assert(equal<decltype(element<0>(type_list<double, int, char>{})),  //
                    double>);
static_assert(equal<decltype(element<1>(type_list<double, int, char>{})), int>);
static_assert(equal<decltype(element<2>(type_list<double, int, char>{})),  //
                    char>);

// Access the types wrapped by a 'type_list' given their index.
//
static_assert(slice<0>(type_list<int>{}) == type_list<int>{});
static_assert(slice<0>(type_list<int, char>{}) == type_list<int>{});
static_assert(slice<1>(type_list<int, char>{}) == type_list<char>{});
static_assert(slice<0>(type_list<double, int, char>{}) == type_list<double>{});
static_assert(slice<1>(type_list<double, int, char>{}) == type_list<int>{});
static_assert(slice<2>(type_list<double, int, char>{}) == type_list<char>{});

// Accessing the first element of a 'type_list' instance.
//
static_assert(equal<decltype(front(type_list<int>{})), int>);
static_assert(equal<decltype(front(type_list<char, int>{})), char>);
static_assert(equal<decltype(front(type_list<double, char, int>{})), double>);

// Access the first element wrapped by 'type_list'.
//
static_assert(front_slice(type_list<int>{}) == type_list<int>{});
static_assert(front_slice(type_list<char, int>{}) == type_list<char>{});
static_assert(front_slice(type_list<double, char, int>{}) ==
              type_list<double>{});
//
static_assert(*type_list<int>{} == type_list<int>{});
static_assert(*type_list<char, int>{} == type_list<char>{});
static_assert(*type_list<double, char, int>{} == type_list<double>{});

// Accessing the back
//
static_assert(equal<decltype(back(type_list<int>{})), int>);
static_assert(equal<decltype(back(type_list<char, int>{})), int>);
static_assert(equal<decltype(back(type_list<double, char, int>{})), int>);

// Access the last element wrapped by 'type_list'.
//
static_assert(back_slice(type_list<int>{}) == type_list<int>{});
static_assert(back_slice(type_list<char, int>{}) == type_list<int>{});
static_assert(back_slice(type_list<double, char, int>{}) == type_list<int>{});
//
static_assert(!type_list<int>{} == type_list<int>{});
static_assert(!type_list<char, int>{} == type_list<int>{});
static_assert(!type_list<double, char, int>{} == type_list<int>{});

// Add a type to the front of a 'type_list' instance.
//
static_assert(push_front<int>(type_list<>{}) == type_list<int>{});
static_assert(push_front<char>(type_list<int>{}) == type_list<char, int>{});
static_assert(push_front<float>(type_list<char, int>{}) ==
              type_list<float, char, int>{});
static_assert(push_front<type_list<>>(type_list<int>{}) ==
              type_list<type_list<>, int>{});

// Add a type to the back of a 'type_list' instance.
//
static_assert(push_back<int>(type_list<>{}) == type_list<int>{});
static_assert(push_back<char>(type_list<int>{}) == type_list<int, char>{});
static_assert(push_back<float>(type_list<char, int>{}) ==
              type_list<char, int, float>{});
static_assert(push_back<type_list<>>(type_list<int>{}) ==
              type_list<int, type_list<>>{});

// Concatenation of 'type_list' instances.
//
static_assert(concat(type_list<>{}, type_list<>{}) == type_list<>{});
static_assert(concat(type_list<int>{}, type_list<>{}) == type_list<int>{});
static_assert(concat(type_list<>{}, type_list<int>{}) == type_list<int>{});
static_assert(concat(type_list<int>{}, type_list<char>{}) ==
              type_list<int, char>{});
static_assert(concat(type_list<char>{}, type_list<int>{}) ==
              type_list<char, int>{});
static_assert(concat(type_list<char, float>{}, type_list<int>{}) ==
              type_list<char, float, int>{});
static_assert(concat(type_list<float>{}, type_list<char, int>{}) ==
              type_list<float, char, int>{});
//
static_assert(type_list<>{} + type_list<>{} == type_list<>{});
static_assert(type_list<int>{} + type_list<>{} == type_list<int>{});
static_assert(type_list<>{} + type_list<int>{} == type_list<int>{});
static_assert(type_list<int>{} + type_list<char>{} == type_list<int, char>{});
static_assert(type_list<char>{} + type_list<int>{} == type_list<char, int>{});
static_assert(type_list<char, float>{} + type_list<int>{} ==
              type_list<char, float, int>{});
static_assert(type_list<float>{} + type_list<char, int>{} ==
              type_list<float, char, int>{});

// Pop an element from the front.
//
static_assert(pop_front(type_list<int>{}) == type_list<>{});
static_assert(pop_front(type_list<char, int>{}) == type_list<int>{});
static_assert(pop_front(type_list<float, char, int>{}) ==
              type_list<char, int>{});
//
static_assert(--type_list<int>{} == type_list<>{});
static_assert(--type_list<char, int>{} == type_list<int>{});
static_assert(--type_list<float, char, int>{} == type_list<char, int>{});

// Pop an element from the back.
//
static_assert(pop_back(type_list<int>{}) == type_list<>{});
static_assert(pop_back(type_list<char, int>{}) == type_list<char>{});
static_assert(pop_back(type_list<float, char, int>{}) ==
              type_list<float, char>{});
//
static_assert(type_list<int> {} -- == type_list<>{});
static_assert(type_list<char, int> {} -- == type_list<char>{});
static_assert(type_list<float, char, int> {} -- == type_list<float, char>{});

// Reverse the order of types.
//
static_assert(reverse(type_list<>{}) == type_list<>{});
static_assert(reverse(type_list<int>{}) == type_list<int>{});
static_assert(reverse(type_list<char>{}) == type_list<char>{});
static_assert(reverse(type_list<int, char>{}) == type_list<char, int>{});
static_assert(reverse(type_list<char, int>{}) == type_list<int, char>{});
static_assert(reverse(type_list<char, int, float>{}) ==
              type_list<float, int, char>{});
//
static_assert(~type_list<>{} == type_list<>{});
static_assert(~type_list<int>{} == type_list<int>{});
static_assert(~type_list<char>{} == type_list<char>{});
static_assert(~type_list<int, char>{} == type_list<char, int>{});
static_assert(~type_list<char, int>{} == type_list<int, char>{});
static_assert(~type_list<char, int, float>{} == type_list<float, int, char>{});

// Insert types at a specific position.
//
static_assert(insert<0, int>(type_list<>{}) == type_list<int>{});
static_assert(insert<0, char>(type_list<int>{}) == type_list<char, int>{});
static_assert(insert<1, char>(type_list<int>{}) == type_list<int, char>{});
static_assert(insert<0, float>(type_list<int, char>{}) ==
              type_list<float, int, char>{});
static_assert(insert<1, float>(type_list<int, char>{}) ==
              type_list<int, float, char>{});
static_assert(insert<2, float>(type_list<int, char>{}) ==
              type_list<int, char, float>{});

// Insert types by using a predicate.
//
static_assert(insert<int>(type_list<>{}, less) == type_list<int>{});
static_assert(insert<char>(type_list<>{}, less) == type_list<char>{});
static_assert(insert<int>(type_list<char>{}, less) == type_list<char, int>{});
static_assert(insert<unsigned>(type_list<char, int>{}, less) ==
              type_list<char, unsigned, int>{});
static_assert(insert<short>(type_list<char, int>{}, less) ==
              type_list<char, short, int>{});
static_assert(insert<double>(type_list<char, int>{}, less) ==
              type_list<char, int, double>{});

// Remove types at a specific position.
//
static_assert(remove<0>(type_list<int>{}) == type_list<>{});
static_assert(remove<0>(type_list<int, char>{}) == type_list<char>{});
static_assert(remove<1>(type_list<int, char>{}) == type_list<int>{});
static_assert(remove<0>(type_list<float, int, char>{}) ==
              type_list<int, char>{});
static_assert(remove<1>(type_list<float, int, char>{}) ==
              type_list<float, char>{});
static_assert(remove<2>(type_list<float, int, char>{}) ==
              type_list<float, int>{});

// Remove types by using a predicate.
//
constexpr auto too_big = []<typename x> { return sizeof(x) > 2; };
static_assert(remove(type_list<>{}, too_big) == type_list<>{});
static_assert(remove(type_list<char>{}, too_big) == type_list<char>{});
static_assert(remove(type_list<int>{}, too_big) == type_list<>{});
static_assert(remove(type_list<char, int>{}, too_big) == type_list<char>{});
static_assert(remove(type_list<int, char>{}, too_big) == type_list<char>{});
static_assert(remove(type_list<int, unsigned>{}, too_big) == type_list<>{});
static_assert(remove(type_list<short, char>{}, too_big) ==
              type_list<short, char>{});
static_assert(remove(type_list<int, unsigned, char, short>{}, too_big) ==
              type_list<char, short>{});
//
// static_assert(remove(type_list<char, short, int, unsigned, float>{},
//                      []<size_t i, typename type> { return (i % 2) != 0; }) ==
//               type_list<char, int, float>{});

// Trim the front.
//
static_assert(trim_front<0>(type_list<>{}) == type_list<>{});
static_assert(trim_front<0>(type_list<int>{}) == type_list<int>{});
static_assert(trim_front<1>(type_list<int>{}) == type_list<>{});
static_assert(trim_front<0>(type_list<int, char>{}) == type_list<int, char>{});
static_assert(trim_front<1>(type_list<int, char>{}) == type_list<char>{});
static_assert(trim_front<2>(type_list<int, char>{}) == type_list<>{});
static_assert(trim_front<0>(type_list<int, char, float>{}) ==
              type_list<int, char, float>{});
static_assert(trim_front<1>(type_list<int, char, float>{}) ==
              type_list<char, float>{});
static_assert(trim_front<2>(type_list<int, char, float>{}) ==
              type_list<float>{});
static_assert(trim_front<3>(type_list<int, char, float>{}) == type_list<>{});

// Trim the back.
//
static_assert(trim_back<0>(type_list<>{}) == type_list<>{});
static_assert(trim_back<0>(type_list<int>{}) == type_list<int>{});
static_assert(trim_back<1>(type_list<int>{}) == type_list<>{});
static_assert(trim_back<0>(type_list<int, char>{}) == type_list<int, char>{});
static_assert(trim_back<1>(type_list<int, char>{}) == type_list<int>{});
static_assert(trim_back<2>(type_list<int, char>{}) == type_list<>{});
static_assert(trim_back<0>(type_list<int, char, float>{}) ==
              type_list<int, char, float>{});
static_assert(trim_back<1>(type_list<int, char, float>{}) ==
              type_list<int, char>{});
static_assert(trim_back<2>(type_list<int, char, float>{}) == type_list<int>{});
static_assert(trim_back<3>(type_list<int, char, float>{}) == type_list<>{});

// Get a subrange of types from a given 'type_list' instance.
//
static_assert(range<0, 0>(type_list<>{}) == type_list<>{});
static_assert(range<0, 0>(type_list<int>{}) == type_list<>{});
static_assert(range<1, 1>(type_list<int>{}) == type_list<>{});
static_assert(range<0, 1>(type_list<int>{}) == type_list<int>{});
static_assert(range<0, 0>(type_list<int, char>{}) == type_list<>{});
static_assert(range<0, 1>(type_list<int, char>{}) == type_list<int>{});
static_assert(range<0, 2>(type_list<int, char>{}) == type_list<int, char>{});
static_assert(range<1, 2>(type_list<int, char>{}) == type_list<char>{});
static_assert(range<2, 2>(type_list<int, char>{}) == type_list<>{});
static_assert(range<0, 0>(type_list<int, char, float>{}) == type_list<>{});
static_assert(range<0, 1>(type_list<int, char, float>{}) == type_list<int>{});
static_assert(range<0, 2>(type_list<int, char, float>{}) ==
              type_list<int, char>{});
static_assert(range<0, 3>(type_list<int, char, float>{}) ==
              type_list<int, char, float>{});
static_assert(range<0, 1>(type_list<int, char, float>{}) == type_list<int>{});
static_assert(range<1, 2>(type_list<int, char, float>{}) == type_list<char>{});
static_assert(range<2, 3>(type_list<int, char, float>{}) == type_list<float>{});
static_assert(range<0, 2>(type_list<int, char, float>{}) ==
              type_list<int, char>{});
static_assert(range<1, 3>(type_list<int, char, float>{}) ==
              type_list<char, float>{});

// Swap two elements.
//
static_assert(swap<0, 0>(type_list<int>{}) == type_list<int>{});
//
static_assert(swap<0, 0>(type_list<int, char>{}) == type_list<int, char>{});
static_assert(swap<0, 1>(type_list<int, char>{}) == type_list<char, int>{});
static_assert(swap<1, 0>(type_list<int, char>{}) == type_list<char, int>{});
static_assert(swap<1, 1>(type_list<int, char>{}) == type_list<int, char>{});
//
static_assert(swap<0, 0>(type_list<int, char, float>{}) ==
              type_list<int, char, float>{});
static_assert(swap<0, 1>(type_list<int, char, float>{}) ==
              type_list<char, int, float>{});
static_assert(swap<0, 2>(type_list<int, char, float>{}) ==
              type_list<float, char, int>{});
static_assert(swap<1, 0>(type_list<int, char, float>{}) ==
              type_list<char, int, float>{});
static_assert(swap<1, 1>(type_list<int, char, float>{}) ==
              type_list<int, char, float>{});
static_assert(swap<1, 2>(type_list<int, char, float>{}) ==
              type_list<int, float, char>{});
static_assert(swap<2, 0>(type_list<int, char, float>{}) ==
              type_list<float, char, int>{});
static_assert(swap<2, 1>(type_list<int, char, float>{}) ==
              type_list<int, float, char>{});
static_assert(swap<2, 2>(type_list<int, char, float>{}) ==
              type_list<int, char, float>{});

// Merge two sorted 'type_list' instances.
//
static_assert(merge(type_list<>{}, type_list<>{}, less) == type_list<>{});
//
static_assert(merge(type_list<>{}, type_list<int>{}, less) == type_list<int>{});
static_assert(merge(type_list<int>{}, type_list<>{}, less) == type_list<int>{});
static_assert(merge(type_list<>{}, type_list<char, int>{}, less) ==
              type_list<char, int>{});
static_assert(merge(type_list<char, int>{}, type_list<>{}, less) ==
              type_list<char, int>{});
//
static_assert(merge(type_list<char>{}, type_list<int>{}, less) ==
              type_list<char, int>{});
static_assert(merge(type_list<int>{}, type_list<char>{}, less) ==
              type_list<char, int>{});
//
static_assert(merge(type_list<char, short>{}, type_list<int>{}, less) ==
              type_list<char, short, int>{});
static_assert(merge(type_list<char>{}, type_list<short, int>{}, less) ==
              type_list<char, short, int>{});
//
static_assert(merge(type_list<char, short>{}, type_list<int, double>{}, less) ==
              type_list<char, short, int, double>{});
static_assert(merge(type_list<char, int>{}, type_list<short, double>{}, less) ==
              type_list<char, short, int, double>{});
static_assert(merge(type_list<char, double>{}, type_list<short, int>{}, less) ==
              type_list<char, short, int, double>{});
static_assert(merge(type_list<short, int>{}, type_list<char, double>{}, less) ==
              type_list<char, short, int, double>{});
//
static_assert(merge(type_list<char>{}, type_list<short, int, double>{}, less) ==
              type_list<char, short, int, double>{});
static_assert(merge(type_list<short>{}, type_list<char, int, double>{}, less) ==
              type_list<char, short, int, double>{});
static_assert(merge(type_list<int>{}, type_list<char, short, double>{}, less) ==
              type_list<char, short, int, double>{});
static_assert(merge(type_list<double>{}, type_list<char, short, int>{}, less) ==
              type_list<char, short, int, double>{});

// Sort a list of types.
//
static_assert(sort(type_list<>{}, less) == type_list<>{});
//
static_assert(sort(type_list<int>{}, less) == type_list<int>{});
static_assert(sort(type_list<char>{}, less) == type_list<char>{});
//
static_assert(sort(type_list<char, int>{}, less) == type_list<char, int>{});
static_assert(sort(type_list<int, char>{}, less) == type_list<char, int>{});
static_assert(sort(type_list<unsigned, int>{}, less) ==
              type_list<unsigned, int>{});
static_assert(sort(type_list<int, unsigned>{}, less) ==
              type_list<int, unsigned>{});
//
static_assert(sort(type_list<char, short, int>{}, less) ==
              type_list<char, short, int>{});
static_assert(sort(type_list<char, int, short>{}, less) ==
              type_list<char, short, int>{});
static_assert(sort(type_list<short, char, int>{}, less) ==
              type_list<char, short, int>{});
static_assert(sort(type_list<short, int, char>{}, less) ==
              type_list<char, short, int>{});
static_assert(sort(type_list<int, char, short>{}, less) ==
              type_list<char, short, int>{});
static_assert(sort(type_list<int, short, char>{}, less) ==
              type_list<char, short, int>{});
//
static_assert(sort(type_list<short, unsigned, char, int>{},
                   []<typename x, typename y> {
                     return sizeof(x) >= sizeof(y);
                   }) == type_list<unsigned, int, short, char>{});
static_assert(sort(type_list<short, unsigned, char, int>{},
                   []<typename x, typename y> {
                     return sizeof(x) > sizeof(y);
                   }) == type_list<int, unsigned, short, char>{});

// Transform
//
static_assert(transform(type_list<float, void, int>{}, []<typename x> {
                if constexpr (equal<x, void>)
                  return type_list<type_list<>>{};
                else
                  return type_list<x>{};
              }) == type_list<float, type_list<>, int>{});
