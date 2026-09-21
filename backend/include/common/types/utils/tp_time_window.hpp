#ifndef TASK_DISTRIBUTION_TIME_WINDOW_HPP
#define TASK_DISTRIBUTION_TIME_WINDOW_HPP

#include <mps/mps_common/utils/json_io/json.hpp>

namespace td {
namespace types {
/** -------------------------------------------------------------------------------------------------------------
 * @brief The tp_time_window class - описание временного окна задачи
 * Временное окно - интервал времени, в который должно начаться обслуживание задачи
 ---------------------------------------------------------------------------------------------------------------*/
class tp_time_window {
public:
    explicit tp_time_window() = default;
    explicit tp_time_window(const mps::json::object::JsonObject* obj_cfg_);
    tp_time_window(const tp_time_window& tw_);
    tp_time_window(tp_time_window&& tw_) noexcept;
    ~tp_time_window() = default;

    tp_time_window& operator=(const mps::json::object::JsonObject* obj_cfg_);
    tp_time_window& operator=(const tp_time_window& tw_);
    tp_time_window& operator=(tp_time_window&& tw_);

    inline bool description_valid() const { return this->description_valid_; }

    /**
     * @brief time_begin - получение начала временного окна задачи
     * @return начало временного окна задачи
     */
    inline std::string time_begin() const { return this->time_begin_; }

    /**
     * @brief time_end - получение окончания временного окна задачии
     * @return окончание временного окна задачи
     */
    inline std::string time_end() const { return this->time_end_; }

private:
    /**
     * @brief read_configuration - метод чтения описания временного окна задачи
     * @param obj_cfg_ - указатель на описание временного окна задачи
     * @return результат чтения описания временного окна задачи
     */
    bool read_configuration(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация описания временного окна задачи на валидность
     * @param obj_cfg_ - указатель на описание временного окна
     * @return результат верификации
     */
    bool configuration_valid(const mps::json::object::JsonObject* obj_cfg_);

private:
    bool description_valid_ = false;        /// <--- признак чтения описания временного окна

    std::string time_begin_ = "";           /// <--- время начала выполнения задачи
    std::string time_end_ = "";             /// <--- время окончания выполнения задачи
};
}       /// <--- types
}   /// <--- td

#endif
