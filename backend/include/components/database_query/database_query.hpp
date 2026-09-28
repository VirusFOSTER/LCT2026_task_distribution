#ifndef DATABASE_QUERY_COMPONENT_HPP
#define DATABASE_QUERY_COMPONENT_HPP

#include <mps/mps_process_traits/components_traits/base_component/base_functional_component.hpp>
#include "common/messages/msg_request.hpp"
#include "common/messages/msg_list_instances.hpp"
#include "common/messages/msg_tasks_list.hpp"

#include <mps/mps_common/utils/json_io/json.hpp>
#include <sqlite3.h>

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
     * @brief exec - метод выполнения запросоа
     * @param request_message_ - сообщение, содержащее запрос
     */
    void exec(const msg::msg_request* const request_message_);

    //=======================================
    /**
     * @brief exec_update_data - метод выполнения запроса по обновлению данных
     * @param request_message_ - сообщение, содержащее запрос
     */
    void exec_update_data(const msg::msg_request* const request_message_);

    /**
     * @brief append_instances - метод добавления новых исполнителей
     * @param request_ - описание запроса
     */
    void append_instances(const msg::msg_request* const request_);

    /**
     * @brief block_instances - метод блокировки исполнителей задач (исполнители недоступны)
     * @param request_ - описание запроса
     */
    void block_instances(const msg::msg_request* const request_);

    /**
     * @brief remove_instances - метод удаления исполнителей задач
     * @param request_ - описание запроса
     */
    void remove_instances(const msg::msg_request* const request_);

    /**
     * @brief description_instances_valid - верификация описания исполнителей задач на валидность
     * @param dsc_insts_ - описание задач в формате json
     * @return результат верификации задач на валидность
     */
    bool description_instances_valid(const mps::json::object::JsonObject* dsc_insts_);

    //=======================================
    /**
     * @brief exec_processing_data - метод выполнения запроса по обработке данных
     * По сути собирается информация по текущему положению исполнителей задач
     * @param request_message_ - сообщение, содержащее запрос
     */
    void exec_processing_data(const msg::msg_request* const request_message_);

    void exec_task_distribution(const msg::msg_request* const request_);

    void exec_process_emergency(const msg::msg_request* const request_);

    void exec_process_cancel(const msg::msg_request* const request_);

    //=======================================
    /**
     * @brief exec_get_info - метод выполнения запроса на выдачу данных
     * @param request_message_ - сообщение, содержащее запрос
     */
    void exec_get_info(const msg::msg_request* const request_message_);

    void exec_get_free_instances(const msg::msg_request* const request_);
    void exec_get_job_instances(const msg::msg_request* const request_);
    void exec_get_free_tasks(const msg::msg_request* const request_);
    void exec_get_job_tasks(const msg::msg_request* const request_);
    void exec_completed_tasks(const msg::msg_request* const request_);
    void exec_current_position(const msg::msg_request* const request_);

    /**
     * @brief reset - сброс компоненты
     */
    void reset();

private:
    _Irequest_t_* ireader_request_ = nullptr;       /// <--- указатель на интерфейс читателя запросов пользователя
    _Irtasks_t_* ireader_tasks_ = nullptr;          /// <--- указатель на интерфейс читателя списка задач
    _Iinstances_t_* iwriter_instances_ = nullptr;   /// <--- указатель на интерфейс писателя списка исполнителей
    _Iwtasks_t_* iwriter_tasks_ = nullptr;          /// <--- указатель на интерфейс писателя списка задач

    sqlite3* database_ = nullptr;                               /// <--- указатель на базу данных
    const std::string database_name_ = "task_distribution.db";  /// <--- наименование базы данных
};
}       /// <--- component
}   /// <-- td

#endif
