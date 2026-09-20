#ifndef MPS_MATH_EXPECTED_VALUE_WRAPPER_HPP
#define MPS_MATH_EXPECTED_VALUE_WRAPPER_HPP

#include <utility>

namespace mps {
namespace math {
/**
 * @brief The math_expected_value_wrapper class
 */
template <typename T_value, typename T_weight = int>
class math_expected_value_wrapper {
public:
    using _value_t_ = T_value;
    using _weight_t_ = T_weight;

    /**
     * @brief math_expected_value_wrapper
     */
    explicit math_expected_value_wrapper() = default;

    /**
     * @brief math_expected_value_wrapper
     * @param v_
     * @param w_
     */
    math_expected_value_wrapper(const _value_t_& v_, const _weight_t_& w_) : value_(v_), weight_(w_) { }

    /**
     * @brief math_expected_value_wrapper
     * @param v_
     * @param w_
     */
    math_expected_value_wrapper(_value_t_&& v_, _weight_t_&& w_) noexcept : value_(std::move(v_)), weight_(std::move(w_)) {}

    /**
     * @brief operator ()
     */
    virtual void operator()(_value_t_, _weight_t_) = 0;

    /**
     * @brief operator +=
     * @return
     */
    virtual math_expected_value_wrapper& operator+=(math_expected_value_wrapper&) = 0;

    /**
     * @brief operator -=
     * @return
     */
    virtual math_expected_value_wrapper& operator-=(math_expected_value_wrapper&) = 0;

    /**
     * @brief reset
     */
    virtual void reset(_value_t_, _weight_t_) = 0;

    /**
     * @brief value
     * @return
     */
    inline _value_t_ value() const { return this->value_; }

    /**
     * @brief weight
     * @return
     */
    inline _weight_t_ weight() const { return this->weight_; }

protected:
    _value_t_ value_;       /// <---
    _weight_t_ weight_;     /// <---
};
}       /// <--- math
}   /// <--- mps

#endif
