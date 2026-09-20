#ifndef MPS_MEAN_VALUE_WRAPPER_HPP
#define MPS_MEAN_VALUE_WRAPPER_HPP

#include "math_expected_value_wrapper.hpp"

namespace mps {
namespace math {
template <typename T_value, typename T_weight = int>
class mean_value_wrapper : public math_expected_value_wrapper<T_value, T_weight> {
public:
    using _value_t_ = T_value;
    using _weight_t_ = T_weight;

    mean_value_wrapper(const _value_t_& v_, const _weight_t_& w_);
    mean_value_wrapper(_value_t_&& v_, _weight_t_&& w_) noexcept;

    ~mean_value_wrapper() = default;

    void operator()(_value_t_ v_, _weight_t_ w_);

    mean_value_wrapper& operator+=(const mean_value_wrapper& wr_);
    mean_value_wrapper& operator-=(const mean_value_wrapper& wr_);

    inline void reset(_value_t_ v_, _weight_t_ w_) { this->value_ = v_, this->weight_ = w_; }
    inline void reset(mean_value_wrapper& mv_);

private:
    math_expected_value_wrapper<_value_t_, _weight_t_>& operator+=(math_expected_value_wrapper<_value_t_, _weight_t_>& wr_);
    math_expected_value_wrapper<_value_t_, _weight_t_>& operator-=(math_expected_value_wrapper<_value_t_, _weight_t_>& wr_);
};


//---------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
mean_value_wrapper<T_value, T_weight>::mean_value_wrapper(const _value_t_& v_, const _weight_t_& w_) :
    math_expected_value_wrapper<T_value, T_weight>(v_, w_) { }

//---------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
mean_value_wrapper<T_value, T_weight>::mean_value_wrapper(_value_t_&& v_, _weight_t_&& w_) noexcept :
    math_expected_value_wrapper<T_value, T_weight>(v_, w_) { }

//---------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
void mean_value_wrapper<T_value,T_weight>::operator()(_value_t_ v_, _weight_t_ w_) {
    auto cw_ = this->weight_;

    this->weight_ += w_;
    this->value_ = (this->value_ * cw_ + v_ * w_) / this->weight_;
}

//---------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
mean_value_wrapper<T_value, T_weight>& mean_value_wrapper<T_value, T_weight>::operator+=(const mean_value_wrapper& wr_) {
    (*this)(wr_.value(), wr_.weight());
    return *this;
}

//---------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
mean_value_wrapper<T_value, T_weight>& mean_value_wrapper<T_value, T_weight>::operator-=(const mean_value_wrapper& wr_) {
    (*this)(wr_.value(), -wr_.weight());
    return *this;
}

//---------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
math_expected_value_wrapper<T_value, T_weight>&
mean_value_wrapper<T_value, T_weight>::operator+=(math_expected_value_wrapper<_value_t_, _weight_t_>& wr_) {
    (*this)(wr_.value(), wr_.weight());
    return *this;
}

//---------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
math_expected_value_wrapper<T_value, T_weight>&
mean_value_wrapper<T_value, T_weight>::operator-=(math_expected_value_wrapper<_value_t_, _weight_t_>& wr_) {
    (*this)(wr_.value(), -wr_.weight());
    return *this;
}
}       /// <--- math
}   /// <--- mps

#endif
