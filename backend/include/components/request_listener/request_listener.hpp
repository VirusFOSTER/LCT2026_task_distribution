#ifndef REQUEST_LISTENER_COMPONENT_HPP
#define REQUEST_LISTENER_COMPONENT_HPP

#include <mps/mps_process_traits/components_traits/base_component/base_functional_component.hpp>
#include "common/messages/msg_request.hpp"

namespace td {
namespace component {
/** ---------------------------------------------------------------------------------------------------------------------
 * @brief The fc_request_listener class - [функциоональная компонента] модуль получения запросов от пользователя
 * Данный модуль только принимает запросы и формирует соответствующее сообщение для остальных модулей системы.
 * По сути это входная точка для обработки данных
 ---------------------------------------------------------------------------------------------------------------------*/
class fc_request_listener : public mps::process::component::base::base_functional_component {
    using _Irequest_t_ = mps::process::interface::Isequence_writer<msg::msg_request>;

public:
    /**
     * @brief fc_request_listener - конструктор
     * @param fc_name_ - уникальное наименование компоненты
     */
    explicit fc_request_listener(const std::string& fc_name_);

    /**
     * деструктор
     */
    ~fc_request_listener();

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
    _Irequest_t_* iwriter_request_ = nullptr;   /// <--- указатель на интерфейс писателя запросов от пользователя
};
}       /// <--- component
}   /// <--- td

#endif
