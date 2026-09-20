#ifndef SYSTEM_MEMBER_TYPE_DETECTOR_HPP
#define SYSTEM_MEMBER_TYPE_DETECTOR_HPP

#define GENERATE_HAS_MEMBER_TYPE(Type)                                              \
    template <class T>                                                              \
    class CHasMemberType_##Type {                                                   \
    private:                                                                        \
    using Yes = char[2];                                                            \
    using No = char[1];                                                             \
                                                                                    \
    struct Fallback { struct Type {}; };                                            \
    struct Derived : T, Fallback { };                                               \
                                                                                    \
    template <class U>                                                              \
    static No& check(typename U::Type*);                                            \
                                                                                    \
    template <class U>                                                              \
    static Yes& check (U*);                                                         \
                                                                                    \
    public:                                                                         \
    static constexpr bool _result =                                                 \
    sizeof(check<Derived>(nullptr)) == sizeof(Yes);                                 \
    };                                                                              \
                                                                                    \
    template <class T>                                                              \
    struct has_member_type_##Type                                                   \
    : public boost::integral_constant<bool,CHasMemberType_##Type<T>::_result> {     \
};


#endif
