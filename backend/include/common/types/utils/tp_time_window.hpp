#ifndef TASK_DISTRIBUTION_TIME_WINDOW_HPP
#define TASK_DISTRIBUTION_TIME_WINDOW_HPP

#include <iostream>
#include <cstring>

namespace td {
namespace types {
/** -------------------------------------------------------------------------------------------------------------
 * @brief The tp_time_window class - описание временного окна задачи
 * Временное окно - интервал времени, в который должно начаться обслуживание задачи
 ---------------------------------------------------------------------------------------------------------------*/
class tp_time_window {
public:
    explicit tp_time_window() = default;
    tp_time_window(const tp_time_window& tw_);
    tp_time_window(tp_time_window&& tw_) noexcept;
    ~tp_time_window() = default;

    tp_time_window& operator=(const tp_time_window& tw_);
    tp_time_window& operator=(tp_time_window&& tw_);

    /**
     * @brief set_time_begin - метод установления начала временного окна
     * @param t_ - новое значение начала временного окна в формате строки
     */
    inline void set_time_begin(const std::string& t_) { this->time_begin_ = t_; }

    /**
     * @brief set_time_end - метод установления окончания временного окна
     * @param t_ - новое значение окончания временного окна в формате строки
     */
    inline void set_time_end(const std::string& t_) { this->time_end_ = t_; }

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
    std::string time_begin_ = "";           /// <--- время начала выполнения задачи
    std::string time_end_ = "";             /// <--- время окончания выполнения задачи
};
}       /// <--- types
}   /// <--- td

#endif
