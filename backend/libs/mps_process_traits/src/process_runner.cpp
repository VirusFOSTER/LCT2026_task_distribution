#include "process_runner.hpp"
#include "messages_traits/messages_container/register_messages_container.hpp"
#include "components_traits/components_container/components_container.hpp"
#include "components_traits/components_container/qcomponents_container.hpp"

boost::shared_ptr<mps::process::container::messages::register_messages_container>
    register_messages_container_(new mps::process::container::messages::register_messages_container);

boost::shared_ptr<mps::process::container::components::register_components_container>
    register_components_container_(new mps::process::container::components::register_components_container);

boost::shared_ptr<mps::process::container::components::register_qcomponents_container>
    register_qcomponents_container_(new mps::process::container::components::register_qcomponents_container);

boost::atomic<bool> threads_join_ = true;
boost::atomic<uint16_t> count_tasks_ = 0;

using namespace mps;
using namespace process;

//--------------------------------------------------------------------

process_runner::process_runner(const std::string &proc_name_, const std::string &path_cfg_) :
    mps::config::process_configuration(proc_name_,path_cfg_) { }

//--------------------------------------------------------------------

void process_runner::start_process() {
    //(?) Запускаем процесс, если инициализация успешно выполнена
    if (this->init_process()) {
        this->start();
    }
}

//--------------------------------------------------------------------

bool process_runner::init_process() {
    //(?) Если конфигурация процесса прочитана, инициализаируем процесс системы
    if (this->configuration_read()) {
        return this->init_process_components() &&
               this->init_process_messages();
    }

    // В противном случае возвращаем отрицательный результат инициализации
    // Процесс в этом случае не должен быть запущен
    return false;
}

//--------------------------------------------------------------------

bool process_runner::init_process_components() {
    //(?>) Формируем задачи на исполнение в соответствии с конфигурацией
    if (this->components_configuration_ && register_components_container_) {
        for (uint16_t i = 0; i < this->components_configuration_->components_count(); ++i) {
            auto component_cfg_ = (*this->components_configuration_)(i);
            if (!component_cfg_) {
                return false;
            }
            //(?) Если компонента обозначена активной,
            if (component_cfg_->component_active()) {
                auto component_name_ = component_cfg_->component_name();
                auto component_  = register_components_container_->get_component(component_name_);
                if (component_) {
                    this->push_task(component_.get());
                }

                auto qcomponent_ = register_qcomponents_container_->get_qcomponent(component_name_);
                if (qcomponent_) {
                    this->push_task(qcomponent_.get());
                }
            }
        }

        return true;
    }

    return false;
}

//--------------------------------------------------------------------

bool process_runner::init_process_messages() {
    //(?>) Формируем буферы хранения сообщений в соответствии с конфигурацией
    if (this->messages_configuration_ && register_messages_container_) {
        for (uint16_t i = 0; i < this->messages_configuration_->size(); ++i) {
            auto message_cfg_ = (*this->messages_configuration_)(i);
            if (!message_cfg_) {
                return false;
            }

            //(?) Если указанное сообщение зарегистрировано в буфере,...
            auto message_ = register_messages_container_->find_message(message_cfg_->message_name());
            if (message_) {
                // Инициализируем буфер хранения сообщений заданного типа
                message_->register_message_->initialize_buffer(message_cfg_);
            }
        }

        return true;
    }

    return false;
}
