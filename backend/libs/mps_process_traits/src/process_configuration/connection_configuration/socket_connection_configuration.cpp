#include "process_configuration/connection_configuration/socket_connection_configuration.hpp"

using namespace mps;
using namespace config;

//-------------------------------------------------------------

socket_connection_configuration::socket_connection_configuration(const json::object::JsonObject* obj_cfg_) {
    // Считываем конфигурацию и запоминаем результат чтения
    this->configuration_read_ = this->read_configuration(obj_cfg_);
}

//-------------------------------------------------------------

bool socket_connection_configuration::read_configuration(const json::object::JsonObject* obj_cfg_) {
    //(?) Если конфигурация типа managed_memory внешнего взаимодействия валидна, считываем ее
    if (this->configuration_valid(obj_cfg_)) {
        // Считываем параметры соединения
        this->connection_type_ = connection_configuration::_tg_conn_::_tg_conn_managed_memory_;
        this->connection_uid_ = (uint16_t)obj_cfg_->asInteger("connection_uid");
        this->connection_name_ = obj_cfg_->asString("connection_name");
        this->connection_priority_ = (uint16_t)obj_cfg_->asInteger("connection_priority");
        this->buffer_size_ = (uint32_t)obj_cfg_->asInteger("connection_buffer_size");
        this->connection_host_ = obj_cfg_->asString("connection_host");
        this->connection_port_ = (uint16_t)obj_cfg_->asInteger("connection_port");

        //(?>) Считываем список модулей с доступом к соединению
        auto ar_access_ = obj_cfg_->asArray("connection_access");
        for (int16_t i = 0; i < ar_access_->size(); ++i) {
            auto module_name_ = ar_access_->asString(i);

            //(?) Все процессы и компоненты имеют доступ к данному соединению
            if (module_name_ == "*") {
                this->access_.clear();
                std::set<std::string> cs_ = { "*" };
                this->access_.insert(std::make_pair("*",cs_));
                break;
            }

            // Добавляем в словарь очередной процесс и компоненту с доступном к внешнему соединению
            auto p_ = module_name_.find("/");
            if (p_ != std::string::npos) {
                std::string process_name_ = module_name_.substr(0,p_);
                std::string component_name_ = module_name_.substr(p_+1,module_name_.length());

                auto itr_ = this->access_.find(process_name_);
                //(?) Доступ для процесса еще не организован
                if (itr_ == this->access_.end()) {
                    std::set<std::string> cs_ = { component_name_ };
                    this->access_.insert(std::make_pair(process_name_,cs_));
                }  else {
                    //(?) Для всех компонент процесса уже установлен доступ к соединению
                    if (*itr_->second.begin() == "*") {
                        continue;
                    } else {
                        //(?) Устанавливается доступ к соединению для всех компонент процесса
                        if (component_name_ == "*") {
                            itr_->second.clear();
                            itr_->second.insert(component_name_);
                        } else {    //(?) Добавляется очередная компонента с доступом к соединению
                            itr_->second.insert(component_name_);
                        }
                    }
                }
            }
        }

        return true;
    }

    return false;
}

//-------------------------------------------------------------

/* Требуемый формат конфигурации
{
    "connection_uid": ...,
    "connection_priority": ...,
    "connection_name": ...,
    "connection_type": ...,
    "connection_buffer_size": ...,
    "connection_host": ...,
    "connection_port": ...,
    "connection_access": [ "*" ]
}
 */
bool socket_connection_configuration::configuration_valid(const json::object::JsonObject* obj_cfg_) {
    return obj_cfg_ &&
           obj_cfg_->hasProperty("connection_uid") &&
           obj_cfg_->hasProperty("connection_priority") &&
           obj_cfg_->hasProperty("connection_name") &&
           obj_cfg_->hasProperty("connection_buffer_size") &&
           obj_cfg_->hasProperty("connection_host") &&
           obj_cfg_->hasProperty("connection_port") &&
           obj_cfg_->hasProperty("connection_access") &&
           obj_cfg_->asArray("connection_access");
}
