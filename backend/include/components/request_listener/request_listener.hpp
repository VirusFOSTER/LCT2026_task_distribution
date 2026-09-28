#ifndef REQUEST_LISTENER_COMPONENT_HPP
#define REQUEST_LISTENER_COMPONENT_HPP

#include <mps/mps_process_traits/components_traits/base_component/base_functional_component.hpp>
#include "common/messages/msg_request.hpp"
#include <zmq.hpp>

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
    /**
     * @brief make_request - метод формирования сообщения типа user_request
     * @param request_ - входящее сообщение от пользователя (выполняется через мост)
     */
    void make_request(const std::string& request_);

    /**
     * @brief define_type - метод определения типа запроса
     * @param type_ - тип запроса в строковом описании
     * @return метка типа запроса
     */
    msg::tg_request_type define_type(const std::string& type_);

    /**
     * @brief define_subtype - общий метод определения подтипа запроса и установления функции передачи запросов
     * @param type_ - тип запроса
     * @param subtype_ - подтип запроса в строковой переменной
     * @return результат определения функции передачи запроса
     */
    bool define_subtype(msg::tg_request_type& type_, const std::string& subtype_, const std::string& request_);

    /**
     * @brief define_update_subtype - метод определения подтипа запроса (обновление базы данных)
     * @param subtype_ - подтип запроса в строковом описании
     * @return метка подтипа запроса
     */
    msg::tg_update define_update_subtype(const std::string& subtype_, const std::string& request_);

    /**
     * @brief define_processing_subtype - метод определения подтипа запроса (обработка данных)
     * @param subtype_ - подтип запроса в строковом описании
     * @return метка подтипа запроса
     */
    msg::tg_processing define_processing_subtype(const std::string& subtype_, const std::string& request_);

    /**
     * @brief define_info_subtype - метод определения подтипа запроса (получение данных)
     * @param subtype_ - подтип запроса в строковом описании
     * @return метка подтипа запроса
     */
    msg::tg_get_info define_info_subtype(const std::string& subtype_, const std::string& request_);

private:
    _Irequest_t_* iwriter_request_ = nullptr;   /// <--- указатель на интерфейс писателя запросов от пользователя
    _Irequest_t_* iwriter_request_database_ = nullptr;  /// <--- указатель на интерфейс писателя запросов от пользователя

    zmq::context_t context_{1};
    zmq::socket_t socket_;
    std::string address_ = "tclp://*:5555";
};
}       /// <--- component
}   /// <--- td

#endif
