#include "components/database_query/database_query.hpp"

using namespace td;
using namespace component;

//-----------------------------------------------------------------------------------

fc_database_query::fc_database_query(const std::string& fc_name_) :
    mps::process::component::base::base_functional_component(fc_name_) { }

//-----------------------------------------------------------------------------------

fc_database_query::~fc_database_query() { this->reset(); }

//-----------------------------------------------------------------------------------

bool fc_database_query::init() {
    // Получаем указатель на интерфейс читателя сообщений типа user_request
    this->ireader_request_ = this->interface_reader<msg::msg_request>("user_request");
    if (!this->ireader_request_) {
        std::cout << "Fail! Pointer to reader_interface \'user_request\' is not defined!";
        this->reset();
        return false;
    }

    // Получаем указатель на интерфейс писателя сообщений типа list_instances
    this->iwriter_instances_ = this->interface_writer<msg::msg_list_instances>("list_instances");
    if (!this->iwriter_instances_) {
        std::cout << "Fail! Pointer to writer_interface \'list_instances\' is not defined!";
        this->reset();
        return false;
    }

    // Получаем указатель на интерфейс читателя сообщений типа list_tasks
    this->ireader_tasks_ = this->interface_reader<msg::msg_tasks_list>("list_tasks");
    if (!this->ireader_tasks_) {
        std::cout << "Fail! Pointer to reader_interface \'list_tasks\' is not defined!";
        this->reset();
        return false;
    }

    std::cout << "Component \'" + this->component_name_ + "\' is init!";

    return true;
}

//-----------------------------------------------------------------------------------

void fc_database_query::run() {
    std::cout << "Component \'" + this->component_name_ + "\' is started!";

    //(?>) Работаем в бесконечном цикле
    while (1) {
        // TODO
    }
}

//-----------------------------------------------------------------------------------

void fc_database_query::reset() {
    // Освобождение выделеной под интерфейсы памяти
    if (this->ireader_tasks_) {
        delete this->iwriter_tasks_;
        this->iwriter_tasks_ = nullptr;
    }

    if (this->ireader_request_) {
        delete this->ireader_request_;
        this->ireader_request_ = nullptr;
    }

    if (this->iwriter_instances_) {
        delete this->iwriter_instances_;
        this->iwriter_instances_ = nullptr;
    }

    if (this->iwriter_tasks_) {
        delete this->iwriter_tasks_;
        this->iwriter_tasks_ = nullptr;
    }
}
