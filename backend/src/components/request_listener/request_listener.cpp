#include "components/request_listener/request_listener.hpp"

using namespace td;
using namespace component;

//-----------------------------------------------------------------------------------

fc_request_listener::fc_request_listener(const std::string& fc_name_) :
    mps::process::component::base::base_functional_component(fc_name_) {}

//-----------------------------------------------------------------------------------

fc_request_listener::~fc_request_listener() {
    // Освобождение выделеной под интерфейсы памяти
    if (this->iwriter_request_) {
        delete this->iwriter_request_;
        this->iwriter_request_ = nullptr;
    }
}

//-----------------------------------------------------------------------------------

bool fc_request_listener::init() {
    // Получаем указатель на интерфейс писателя сообщений типа user_request
    this->iwriter_request_ = this->interface_writer<msg::msg_request>("user_request");
    if (!this->iwriter_request_) {
        std::cout << "Fail! Pointer to writer_interface \'user_request\' is not defined!";
        return false;
    }

    std::cout << "Component \'" + this->component_name_ + "\' is init!";

    return true;
}

//-----------------------------------------------------------------------------------

void fc_request_listener::run() {
    std::cout << "Component \'" + this->component_name_ + "\' is started!";

    //(?>) Работаем в бесконечном цикле
    while (1) {
        // TODO
    }
}
