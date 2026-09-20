#ifndef SYSTEM_COLOR_PROPERTIES_HPP
#define SYSTEM_COLOR_PROPERTIES_HPP

#include <boost/array.hpp>
#include <cstdint>

#define COLOR_SIGN_UINT8 (std::string)"uint8"
#define COLOR_SIGN_FLOAT (std::string)"float"

#define FLOAT_LIMIT_COLOR(v) (v > 1.0f) ? 1.0f : (v < 0.0f) ? 0.0f : v

namespace mps {
namespace type_traits {
namespace types {
namespace properties {
/** -------------------------------------------------------------------------------------------------------------
 * Свойства типов объектов и векторов состояния
 * @brief The color_ struct - структура, описывающая цвет объекта
 * Подразумевается, что работа с цветом может быть произведения как с типом uint8_t,
 * так и с типов float
 * Базовым типом цвета является uint8_t
 ---------------------------------------------------------------------------------------------------------------*/
struct tp_color {
    using u_color_t = boost::array<uint8_t,4>;
    using f_color_t = boost::array<float,4>;

    /** color[0] = red, color[1] = green, color[2] = blue */
    u_color_t color_ = { 0, 0, 0, 0 };

    /** Constructors  */
    tp_color(){}
    tp_color(u_color_t c_color) : color_(c_color){}
    tp_color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) {
        color_[0] = r_, color_[1] = g_, color_[2] = b_, color_[3] = a_; }

    /** Convert rgb from uint8_t ot float */
    inline f_color_t to_float() {
        return {
            this->uint8_t_to_float(this->color_[0]),
                    this->uint8_t_to_float(this->color_[1]),
                    this->uint8_t_to_float(this->color_[2]),
                    this->uint8_t_to_float(this->color_[3])
        };
    }

    /**
     * @brief from_float - определение цвета из значений float
     * @param r_ - значение оттенка красного цвета
     * @param g_ - значение оттенка зеленого цвета
     * @param b_ - значение оттенка синего цвета
     * @param a_ - значение прозрачности
     */
    inline void from_float(float r_, float g_, float b_, float a_ = 1.0f) {
        this->color_[0] = this->float_to_uint8_t(r_);
        this->color_[1] = this->float_to_uint8_t(g_);
        this->color_[2] = this->float_to_uint8_t(b_);
        this->color_[3] = this->float_to_uint8_t(a_);
    }

    /**
     * @brief float_to_uint8_t - конвертация float в uint8_t
     * @param p_ - значение в виде float
     * @return сконвертированное в uint8_t значение
     */
    inline uint8_t float_to_uint8_t(float& p_) {
        return (p_ >= 1.0) ? 255 : p_ * 256;
    }

    /**
     * @brief uint8_t_to_float - конвертация значения цвета из uint8_t в float
     * @param p_ - значение цвета в виде uin8_t
     * @return сконвертированное в float значение
     */
    inline float uint8_t_to_float(uint8_t& p_) {
        uint32_t up_ = 0x3f800000 + p_ * 0x8080;
        return (float&)up_ + 256 - 257;
    }
};
}               /// <--- properties
}           /// <--- types
}       /// <--- type_traits
}   /// <--- mps

#endif
