#ifndef SYSTEM_DELETE_COPY_CONSTRUCTOR_TYPE_HPP
#define SYSTEM_DELETE_COPY_CONSTRUCTOR_TYPE_HPP

namespace mps {
namespace type_traits {
namespace types {
namespace properties {
/**
 * Шаблонная структура для удаления конструктора копирования
 */
template <bool>
struct del_copy_constructor_;

template <>
struct del_copy_constructor_<true> {
    del_copy_constructor_(const del_copy_constructor_&) = delete;
    del_copy_constructor_& operator=(const del_copy_constructor_&) = delete;
protected:
    ~del_copy_constructor_() = default;
};

template <>
struct del_copy_constructor_<false>{
protected:
    ~del_copy_constructor_() = default;
};
}               /// <--- properties
}           /// <--- types
}       /// <--- type_traits
}   /// <--- mps

#endif
