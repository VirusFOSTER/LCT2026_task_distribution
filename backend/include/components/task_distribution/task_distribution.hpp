#ifndef TASK_DISTRIBUTION_COMPONENT_HPP
#define TASK_DISTRIBUTION_COMPONENT_HPP

#include <mps/mps_process_traits/components_traits/base_component/base_functional_component.hpp>
#include "common/messages/msg_list_instances.hpp"
#include "common/messages/msg_times_table.hpp"
#include "common/messages/msg_tasks_list.hpp"

#include "components/task_distribution/vroom_problem_solver.hpp"

namespace td {
namespace component {
/** -------------------------------------------------------------------------------------------------------------------
 * @brief The fc_task_distribution class - [фунциональная компонента] модуль распределения задач между исполнителями
 * Основной модуль!
 * При распределении задач учитывает следующие особенности:
 *  - стартовое положение всех исполнителей задач
 *  - временные окна
 *  - типы задач
 *  - навыки исполнителей
 * В случае поступления каких-либо задач высшего приоритета (например, авария) может переназначить выполнение задач в
 * зависимости от:
 *  - текущего положения исполнителя
 *  - текущего состояния работ
 ---------------------------------------------------------------------------------------------------------------------*/
class fc_task_distribution : public mps::process::component::base::base_functional_component {
    using _Iinstances_t_ = mps::process::interface::Isequence_reader<td::msg::msg_list_instances>;
    using _Itimes_t_ = mps::process::interface::Isequence_reader<td::msg::msg_time_table>;
    using _Itasks_t_ = mps::process::interface::Isequence_reader<td::msg::msg_tasks_list>;

public:
    /**
     * @brief fc_task_distribution - конструктор
     * @param fc_name_ - уникальное имя компоненты
     */
    explicit fc_task_distribution(const std::string& fc_name_);

    /**
     * деструктор
     */
    ~fc_task_distribution();

    /**
     * @brief init метод инициализации компоненты
     * @return результат инициализации
     */
    bool init();

    /**
     * @brief run - головная процедура нити
     */
    void run();

private:
    /**
     * @brief make_solve_problem - метод построения решения задачи о назначениях (VRP)
     * @param tasks_ - указатель список задач
     * @param instances_ - указатель на список исполнителей
     * @param time_table_ - указатель на матрицу времен пути
     */
    void make_solve_problem(const msg::msg_tasks_list* const tasks_,
                            const msg::msg_list_instances* const instances_,
                            const msg::msg_time_table* const time_table_);

    /**
     * @brief make_vehicles - метод формирования описания исполнителей задач в формате vroom-фреймворка
     * @param instances_ - описание исполнителей задач
     * @param tasks_ - описание задач
     * @param time_table_ - таблица времен пути
     * @return массив исполнителей задач в формате vroom
     */
    std::vector<vroom::Vehicle> make_vehicles(const msg::msg_tasks_list* const tasks_,
                                              const msg::msg_list_instances* const instances_,
                                              const msg::msg_time_table* const time_table_);

    /**
     * @brief make_jobs - метод формирования описания задач в формате vroom-фреймворка
     * @param tasks_ - описание задач
     * @return массив выполняемых задач в формает vroom
     */
    std::vector<vroom::Job> make_jobs(const msg::msg_tasks_list* const tasks_);

    /**
     * @brief define_profile - конвертация метки движения в тип string
     * @param tg_ - метка движения исполнителя задач
     * @return метка в строковой переменной
     */
    std::string define_profile(types::tg_moving tg_);

    /**
     * @brief reset - сброс компоненты
     */
    void reset();

private:
    _Iinstances_t_* ireader_instance_ = nullptr;        /// <--- указатель на интерфейс читателя исполнителей задач
    _Itimes_t_* ireader_time_ = nullptr;                /// <--- указатель на интерфейс читателя временной таблицы
    _Itasks_t_* ireader_tasks_ = nullptr;               /// <--- указатель на интерфейс читателя списка задач на исполнение
};
}       /// <--- component
}   /// <--- td

#endif
