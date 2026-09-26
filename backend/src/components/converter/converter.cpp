#include "components/converter/converter.hpp"

using namespace td;
using namespace component;

//-----------------------------------------------------------------------------------

fc_converter::fc_converter(const std::string& fc_name_) :
    mps::process::component::base::base_functional_component(fc_name_) {}

//-----------------------------------------------------------------------------------

fc_converter::~fc_converter() { this->reset(); }

//-----------------------------------------------------------------------------------

bool fc_converter::init() {
    // Получаем указатель на интерфейс читателя сообщений типа user_request
    this->ireader_request_ = this->interface_reader<msg::msg_request>("user_request");
    if (!this->ireader_request_) {
        std::cout << "Fail! Pointer to reader_interface \'user_request\' is not defined!";
        return false;
    }

    // Получаем указатель на интерфейс писателя сообщений типа time_table
    this->iwriter_time_table_ = this->interface_writer<msg::msg_time_table>("time_table");
    if (!this->iwriter_time_table_) {
        std::cout << "Fail! Pointer to writer_interface \'time_table\' is not defined!";
        this->reset();
        return false;
    }

    // Получаем указатель на интерфейс писателя сообщений типа list_tasks
    this->iwriter_tasks_ = this->interface_writer<msg::msg_tasks_list>("list_tasks");
    if (!this->iwriter_tasks_) {
        std::cout << "Fail! Pointer to writer_interface \'list_tasks\' is not defined!";
        this->reset();
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------

void fc_converter::run() {
    std::cout << "Component \'" + this->component_name_ + "\' is started!";

    //(?>) Работаем в бесконечном цикле
    while (1) {
        // TODO
    }
}

//-----------------------------------------------------------------------------------

void fc_converter::reset() {
    // Освобождение выделеной под интерфейсы памяти
    if (this->ireader_request_) {
        delete this->ireader_request_;
        this->ireader_request_ = nullptr;
    }

    if (this->iwriter_time_table_) {
        delete this->iwriter_time_table_;
        this->iwriter_time_table_ = nullptr;
    }

    if (this->iwriter_tasks_) {
        delete this->iwriter_tasks_;
        this->iwriter_tasks_ = nullptr;
    }
}
