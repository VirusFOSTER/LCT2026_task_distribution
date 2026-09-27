#ifndef TASK_DISTRIBUTION_TASK_TYPE_HPP
#define TASK_DISTRIBUTION_TASK_TYPE_HPP

#include <mps/mps_common/utils/json_io/json.hpp>
#include "utils/tp_position.hpp"
#include "utils/tp_time_window.hpp"
#include "tg_task.hpp"

namespace td {
namespace types {
/** ------------------------------------------------------------------------------------------------------
 * @brief The tp_task class - описание выполняемой задачи
 * Самое описание считывается из json-файла
 --------------------------------------------------------------------------------------------------------*/
class tp_task {
public:
    /**
     * @brief tp_task - конструктор
     * @param obj_cfg_ - описание задачи
     */
    explicit tp_task(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * деструктор
     */
    ~tp_task() = default;

    inline uint32_t task_uid() { return this->task_uid_; }
    inline tg_task task_type() const { return this->task_type_; }
    inline tp_position task_position() const { return this->position_; }
    inline tp_time_window time_window() const { return this->time_window_; }
    inline std::string region() const { return this->task_region_; }

    inline void set_task_uid(uint32_t uid_) { this->task_uid_ = uid_; }
    inline void set_task_type(tg_task type_) { this->task_type_ = type_; }
    inline void set_postion(tp_position pose_) { this->position_ = pose_; }
    inline void set_time_window(tp_time_window time_wind_) { this->time_window_ = time_wind_; }
    inline void set_region(const std::string& region_) { this->task_region_ = region_; }

    inline long long get_seconds_begin() { return this->get_seconds_from_start_day(this->time_window_.time_begin()); }
    inline long long get_seconds_end() { return this->get_seconds_from_start_day(this->time_window_.time_end()); }

private:
    /**
     * @brief get_seconds_from_start_day - метод получения времени в секунда относительно начала дня
     * @param time_str_ - указанное время
     * @return время в секундах относительно начала дня
     */
    long long get_seconds_from_start_day(const std::string& time_str_);

private:
    /**
     * @brief read_configuration - чтение конфигурации (описания исполняемой задачи)
     * @param obj_cfg_ - указатель на описание задачи
     * @return результат чтения описания задачи
     */
    bool read_configuration(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация описания задачи на валидность
     * @param obj_cfg_ - указатель на описание задачи
     * @return результат верификации на валидность
     */
    bool configuration_valid(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * @brief define_task - определение метки по типу задач
     * @param obj_cfg_ - указатель на описание задачи
     */
    void define_task(const mps::json::object::JsonObject* obj_cfg_);

private:
    bool description_valid_ = false;                /// <--- признак чтения описания задачи на исполнение

    uint32_t task_uid_ = 0;                         /// <--- уникальный идентификатор задачи на исоплнение
    tg_task task_type_ = tg_task::_tg_unknown_;     /// <--- тип задачи на выполнение
    tp_position position_;                          /// <--- положение задачи на карте
    tp_time_window time_window_;                    /// <--- временное окно выполнения задачи
    std::string task_region_ = "";                  /// <--- регион исполняемой задачи
};
}       /// <--- types
}   /// <--- td

#endif
