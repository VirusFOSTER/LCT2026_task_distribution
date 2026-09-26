#ifndef CONVERTER_COMPONENT_HPP
#define CONVERTER_COMPONENT_HPP

#include <mps/mps_process_traits/components_traits/base_component/base_functional_component.hpp>
#include "common/messages/msg_request.hpp"
#include "common/messages/msg_tasks_list.hpp"
#include "common/messages/msg_times_table.hpp"

namespace td {
namespace component {
/**------------------------------------------------------------------------------------------------------------------
 * @brief The fc_converter class - [функциональная компонента] описание модуля конвертации данных
 * Данный модуль принимает входные запросы и конвертирует в нужный для обработки формат:
 *  - список задач на обработку;
 *  - временная матрица путей.
 ------------------------------------------------------------------------------------------------------------------*/
class fc_converter : public mps::process::component::base::base_functional_component {
    using _Irequest_t_ = mps::process::interface::Isequence_reader<msg::msg_request>;
    using _Itime_table_t_ = mps::process::interface::Isequence_writer<msg::msg_time_table>;
    using _Itasks_t_ = mps::process::interface::Isequence_writer<msg::msg_tasks_list>;

public:
    /**
     * @brief fc_converter -  конструктор
     * @param fc_name_ - уникальное наименование компоненты
     */
    explicit fc_converter(const std::string& fc_name_);

    /**
     * деструктор
     */
    ~fc_converter();

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
    _Irequest_t_* ireader_request_ = nullptr;           /// <--- указатель на интерфейс читателя запросов
    _Itime_table_t_* iwriter_time_table_ = nullptr;     /// <--- указатель на интерфейс писателя временной диаграммы путей
    _Itasks_t_* iwriter_tasks_ = nullptr;               /// <--- указатель на интерфейс писателя задач на исоплнение
};
}       /// <--- component
}   /// <--- td

#endif
