#ifndef TASK_DISTRIBUTION_POSITION_TYPE_HPP
#define TASK_DISTRIBUTION_POSITION_TYPE_HPP

#include <mps/mps_common/utils/json_io/json.hpp>

namespace td {
namespace types {
/** ---------------------------------------------------------------------------------------------
 * @brief The tp_position class - описание положения задачи в геодезических координатах
 ------------------------------------------------------------------------------------------------*/
class tp_position {
public:
    explicit tp_position();
    tp_position(const tp_position& pose_);
    tp_position(tp_position&& pose_) noexcept;
    ~tp_position() = default;

    tp_position& operator=(const tp_position& pose_);
    tp_position& operator=(tp_position&& pose_) noexcept;

    /**
     * @brief longitude - получение долготы (расположение задачи)
     * @return долгота (расположение задачи)
     */
    inline float longitude() const { return this->longitude_; }

    /**
     * @brief latitude - получение широты (расположение задачи)
     * @return широта (расположение задачи)
     */
    inline float latitude() const { return this->latitude_; }

    /**
     * @brief set_longitude - метод обновления долготы (расположение задачи)
     * @param lg_ - новое значение долготы (расположение задачи)
     */
    inline void set_longitude(float lg_) { this->longitude_ = lg_; }

    /**
     * @brief set_latitude - метод обновления широты (расположение задачи)
     * @param lt_ - новое значение широты (расположение задачи)
     */
    inline void set_latitude(float lt_) { this->latitude_ = lt_; }

private:
    float longitude_ = 0.0f;            /// <--- долгота
    float latitude_ = 0.0f;             /// <--- широта
};
}       /// <--- types
}   /// <--- td

#endif
