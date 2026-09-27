#ifndef VROOM_PROBLEM_SOLVER_MODULE_HPP
#define VROOM_PROBLEM_SOLVER_MODULE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <optional>
#include <cstdint>

#include "structures/vroom/input/input.h"
#include "structures/vroom/job.h"
#include "structures/vroom/vehicle.h"
#include "structures/vroom/time_window.h"
#include "structures/vroom/location.h"
#include "structures/vroom/amount.h"
#include "structures/vroom/cost_wrapper.h"
#include "utils/exception.h"

constexpr uint32_t SHIFT_START = 9  * 3600;
constexpr uint32_t SHIFT_END   = 18 * 3600;

namespace td {
namespace algorithms {
/** --------------------------------------------------------------------------------------------------
 * @brief The vroom_problem_solver class - модуль решения задачи о назначениях
 * Решение выполняется посредством использования библиотеки VROOM
 * Данный модуль учитывает:
 *  - тип транспортного средства, используемого каждым исполнителем задач;
 *  - компетенции исполнителей задач;
 *  - временные окна исполнения задач;
 *  - стартовые позиции всех исоплнителей задач (мультидепо);
 *  - длительность выпонения задач
 -----------------------------------------------------------------------------------------------------*/
class vroom_problem_solver {
    using _vehicles_t_ = std::vector<vroom::Vehicle>;
    using _tasks_t_ = std::vector<vroom::Job>;
    using _ar_skills_t_ = std::vector<vroom::Skills>;
    using _mtx_moving_t_ = std::pair<std::string,vroom::Matrix<vroom::UserDuration>>;
    using _time_service_t_ = std::vector<vroom::UserDuration>;
    using _time_wind_t_ = std::pair<vroom::UserDuration, vroom::UserDuration>;

public:
    /**
     * @brief vroom_problem_solver - конструктор (по умолчанию)
     */
    explicit vroom_problem_solver(vroom::TimeWindow day_wind_);

    /**
     * деструктор
     */
    ~vroom_problem_solver() { this->reset(); }

    /**
     * @brief init_problem - инициализация решаемой проблемы (задачи назначения)
     */
    inline void init_problem() {
        this->reset();
        this->problem_ = new vroom::Input;
    }

    /**
     * @brief set_instances - метод установления исоплнителей задач
     * @param vehicles_ - текущие исполнители задач
     */
    void set_instances(_vehicles_t_& vehicles_);

    /**
     * @brief set_tasks - метод установления исполняемых задач
     * @param tasks_ - массив задач на исполнение
     */
    void set_tasks(_tasks_t_& tasks_);

    /**
     * @brief set_moving_maxtricies - метод установления матриц времен перемещения между задачами
     * @param mtx_ - массив матриц времен движения между задачами
     */
    void set_moving_maxtricies(std::vector<_mtx_moving_t_>& mtx_);

    /**
     * @brief set_global_time_work - метод установления времени рабочего дня
     * @param time_ - рабочее время
     */
    inline void set_global_time_work(vroom::TimeWindow& time_) { this->global_time_work_ = time_; }

    /**
     * @brief solve_problem - решение задачи о назначениях
     * @return результат решения
     */
    vroom::Solution solve_problem(const unsigned exploration_level_ = 5,
                                  const unsigned nb_threads_ = 4);

private:
    /**
     * @brief reset - метод сброса текущего состояния решаемой проблемы
     */
    void reset();

private:
    vroom::Input* problem_ = nullptr;                       /// <--- Решаемая проблема

    _vehicles_t_ instances_ = {};                           /// <--- массив исполнителей задач
    _tasks_t_ tasks_ = {};                                  /// <--- массив задач на исоплнение
    std::vector<_mtx_moving_t_> moving_matricies_ = {};     /// <--- Временные матрицы путей

    vroom::TimeWindow global_time_work_;    /// <--- Рабочий день исоплнителей задач (длительность)
};
}       /// <--- algorithms
}   /// <--- td

#endif
