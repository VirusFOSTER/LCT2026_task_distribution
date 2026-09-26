#ifndef DATABASE_QUERY_COMPONENT_HPP
#define DATABASE_QUERY_COMPONENT_HPP

#include <mps/mps_process_traits/components_traits/base_component/base_functional_component.hpp>
#include "common/messages/msg_request.hpp"
#include "common/messages/msg_list_instances.hpp"
#include "common/messages/msg_tasks_list.hpp"

namespace td {
namespace component {
/**---------------------------------------------------------------------------------------------------------------
 * @brief The fc_database_query class - [функциональная компонента] модуль запросов в базу данных
 * В зависимости от поступаемого от пользователя запроса данный модуль:
 *  - формирует описание всех исполнителей задач;
 *  - формирует описание доступных исполнителей задач;
 *  - формирует описание всех задач на обработку;
 *  - формирует описание всех выполненных задач;
 *  - формриует описание всех невыполненных задач;
 *  - формирует описание всех задач, выполняемых на текущий момент времени
 *  и т.д.
 ---------------------------------------------------------------------------------------------------------------*/
class fc_database_query : public mps::process::component::base::base_functional_component {
    using _Irequest_t_ = mps::process::interface::Isequence_reader<msg::msg_request>;
    using _Irtasks_t_ = mps::process::interface::Isequence_reader<msg::msg_tasks_list>;
    using _Iinstances_t_ = mps::process::interface::Isequence_writer<msg::msg_list_instances>;
    using _Iwtasks_t_ = mps::process::interface::Isequence_writer<msg::msg_tasks_list>;

public:
    /**
     * @brief fc_database_query - конструктор
     * @param fc_name_ - уникальное наименование компоненты
     */
    explicit fc_database_query(const std::string& fc_name_);

    /**
     * деструктор
     */
    ~fc_database_query();

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
     * @brief reset - сброс компоненты
     */
    void reset();

private:
    _Irequest_t_* ireader_request_ = nullptr;       /// <--- указатель на интерфейс читателя запросов пользователя
    _Irtasks_t_* ireader_tasks_ = nullptr;          /// <--- указатель на интерфейс читателя списка задач
    _Iinstances_t_* iwriter_instances_ = nullptr;   /// <--- указатель на интерфейс писателя списка исполнителей
    _Iwtasks_t_* iwriter_tasks_ = nullptr;          /// <--- указатель на интерфейс писателя списка задач
};
}       /// <--- component
}   /// <-- td

#endif
