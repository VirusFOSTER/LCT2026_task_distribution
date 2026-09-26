#ifndef TASK_DISTRIBUTION_COMPONENT_HPP
#define TASK_DISTRIBUTION_COMPONENT_HPP

#include <mps/mps_process_traits/components_traits/base_component/base_functional_component.hpp>
#include <mps/mps_common/utils/json_io/json.hpp>
#include "common/messages/msg_list_instances.hpp"
#include "common/messages/msg_times_table.hpp"

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
    _Iinstances_t_* ireader_instance_ = nullptr;        /// <--- указатель на интерфейс читателя исполнителей задач
    _Itimes_t_* ireader_time_ = nullptr;                /// <--- указатель на интерфейс читателя временной таблицы
};
}       /// <--- component
}   /// <--- td

#endif
