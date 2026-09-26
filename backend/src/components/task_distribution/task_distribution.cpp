#include "components/task_distribution/task_distribution.hpp"

using namespace td;
using namespace component;

//-----------------------------------------------------------------------------------

fc_task_distribution::fc_task_distribution(const std::string& fc_name_) :
    mps::process::component::base::base_functional_component(fc_name_) { }

//-----------------------------------------------------------------------------------

fc_task_distribution::~fc_task_distribution() { }

//-----------------------------------------------------------------------------------

bool fc_task_distribution::init() {
    // Получаем указатель на интерфейс читателя сообщений типа list_instances
    this->ireader_instance_ = this->interface_reader<td::msg::msg_list_instances>("list_instances");
    if (!this->ireader_instance_) {
        std::cout << "Fail! Pointer to reader_interface \'list_instances\' is not defined!";
        return false;
    }

    // Получаем указатель на интерфейс читателя сообщений типа time_table
    this->ireader_time_ = this->interface_reader<td::msg::msg_time_table>("time_table");
    if (!this->ireader_time_) {
        std::cout << "Fail! Pointer to reader_interface \'time_table\' is not defined!";
        return false;
    }

    std::cout << "Component \'" + this->component_name_ + "\' is init!";

    return true;
}

//-----------------------------------------------------------------------------------

void fc_task_distribution::run() {
    std::cout << "Component \'" + this->component_name_ + "\' is started!";

    //(?>) Работаем в бесконечном цикле
    while (1) {
        // TODO
    }
}
