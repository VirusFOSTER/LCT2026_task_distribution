#ifndef MPS_MATH_OPTIMIZATION_ROUND_VALUES_HPP
#define MPS_MATH_OPTIMIZATION_ROUND_VALUES_HPP

#include <cstdint>
#include <cmath>

#define _int_(x)    (int)(x)
#define _uint8_(x)  (uint8_t)(x)
#define _uint16_(x) (uint16_t)(x)
#define _uint32_(x) (uint32_t)(x)

namespace mps {
namespace math {
/** ---------------------------------------------------------------------------------
 * @brief round_value - функция округления числа
 * Данная функция выполняет округление числа в рамках погрешности
 * Примеры:
 *  1) v_ = 11.12332, er_ = 0.5, v_ -> 11.00
 *  2) v_ = 12.234, er_ = 0.25, v_ -> 12.25
 *  3) v_ = 12.12, er_ = 0.25, v_ = 12.00
 * @param v_ - округляемое значение
 * @param er_ - ошибка, около которой ведется округление значения
 * @param p_ - количество знаков после запятой
 -----------------------------------------------------------------------------------*/
template <typename T, typename U>
inline void round_value(T& v_, U er_, uint8_t p_) {
    uint32_t s_ = std::pow(10, p_);

    auto t1_ = _int_(v_ * s_);
    auto t2_ = _int_(er_ * s_);

    return v_ = (T)(_int_(v_)) + (T)(_int_((t1_ % s_) / t2_)) * er_ +
                ((T)t2_ / s_) * (_int_((_int_(v_ * s_) % t2_) * 2 / t2_));
}
}       /// <--- math
}   /// <--- mps

#endif
