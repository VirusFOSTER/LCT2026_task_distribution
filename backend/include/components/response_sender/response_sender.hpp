#ifndef RESPONSE_SENDER_COMPONENT_HPP
#define RESPONSE_SENDER_COMPONENT_HPP

#include <mps/mps_process_traits/components_traits/base_component/base_functional_component.hpp>
#include <zmq.hpp>

#include "common/messages/msg_list_instances.hpp"
#include "common/messages/msg_tasks_list.hpp"

namespace td {
namespace component {
class fc_response_sender : public mps::process::component::base::base_functional_component {
public:
    /**
     * @brief fc_request_listener - конструктор
     * @param fc_name_ - уникальное наименование компоненты
     */
    explicit fc_response_sender(const std::string& fc_name_);

    /**
     * деструктор
     */
    ~fc_response_sender();

    /**
     * @brief init метод инициализации компоненты
     * @return результат инициализации
     */
    bool init();

    /**
     * @brief run - головная процедура нити
     */
    void run();
};
}       /// <--- component
}   /// <--- td

#endif
