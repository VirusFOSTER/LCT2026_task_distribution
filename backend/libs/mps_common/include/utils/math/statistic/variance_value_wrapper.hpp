#ifndef MPS_VARIANCE_VALUE_WRAPPER_HPP
#define MPS_VARIANCE_VALUE_WRAPPER_HPP

#include "math_expected_value_wrapper.hpp"

namespace mps {
namespace math {
template <typename T_value, typename T_weight = int>
class variance_value_wrapper {
public:
    using _value_t_ = T_value;
    using _weight_t_ = T_weight;
    using _mev_t_ = math_expected_value_wrapper<_value_t_, _weight_t_>;

    template <typename T_expect>
    explicit variance_value_wrapper(T_expect* expect_) : expect_value_(expect_) {
        this->square_expect_value_ = new T_expect(this->expect_value_->value() * this->expect_value_->value(),
                                                  this->expect_value_->weight());
    }

    ~variance_value_wrapper() = default;        // TODO

    void operator()(_value_t_ v_, _weight_t_ w_);

    variance_value_wrapper& operator+=(const variance_value_wrapper& vr_);
    variance_value_wrapper& operator-=(const variance_value_wrapper& vr_);

    inline _mev_t_* math_expected_value() const { return this->expect_value_; }
    inline _mev_t_* square_math_expected_value() const { return this->square_expect_value_; }
    inline _value_t_ variance_value() const { return this->variance_value_; }

    void reset(_value_t_ v_, _weight_t_ w_);

private:
    _mev_t_* expect_value_ = nullptr;
    _mev_t_* square_expect_value_ = nullptr;
    _value_t_ variance_value_;
};


//--------------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
void variance_value_wrapper<T_value, T_weight>::reset(_value_t_ v_, _weight_t_ w_) {
    if (this->expect_value_ && this->square_expect_value_) {
        this->expect_value_->reset(v_, w_);
        this->square_expect_value_->reset(v_ * v_, w_);
    }
}

//--------------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
void variance_value_wrapper<T_value, T_weight>::operator()(_value_t_ v_, _weight_t_ w_) {
    if (this->expect_value_ && this->square_expect_value_) {
        (*this->expect_value_)(v_, w_);
        (*this->square_expect_value_)(v_ * v_, w_);

        this->variance_value_ = this->square_expect_value_->value() -
                                this->expect_value_->value() * this->expect_value_->value();
    }
}

//--------------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
variance_value_wrapper<T_value, T_weight>&
variance_value_wrapper<T_value, T_weight>::operator+=(const variance_value_wrapper& vr_) {
    if (this->expect_value_ && this->square_expect_value_) {
        *this->expect_value_ += *vr_.math_expected_value();
        *this->square_expect_value_ += *vr_.square_math_expected_value();

        this->variance_value_ = this->square_expect_value_->value() -
                                this->expect_value_->value() * this->expect_value_->value();
    }

    return *this;
}

//--------------------------------------------------------------------------------------------

template <typename T_value, typename T_weight>
variance_value_wrapper<T_value, T_weight>&
variance_value_wrapper<T_value, T_weight>::operator-=(const variance_value_wrapper& vr_) {
    if (this->expect_value_ && this->square_expect_value_) {
        *this->expect_value_ -= *vr_.math_expected_value();
        *this->square_expect_value_ -= *vr_.square_math_expected_value();

        this->variance_value_ = this->square_expect_value_->value() -
                                this->expect_value_->value() * this->expect_value_->value();
    }

    return *this;
}
}       /// <--- math
}   /// <--- mps

#endif
