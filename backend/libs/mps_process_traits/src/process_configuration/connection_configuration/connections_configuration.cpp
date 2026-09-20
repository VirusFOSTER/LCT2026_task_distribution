#include "process_configuration/connection_configuration/connections_configuration.hpp"
#include "process_configuration/connection_configuration/socket_connection_configuration.hpp"
#include "process_configuration/connection_configuration/managed_memory_connection_configuration.hpp"

#define TCP_IP_CONNECTION_TYPE (std::string)"tcp/ip"
#define MANAGED_MEMORY_CONNECTION_TYPE (std::string)"managed_memory"

using namespace mps;
using namespace config;

connections_configuration::connections_configuration(const std::string& type_cfg_, const std::string& path_cfg_) {
    // Запоминаем путь к файлу конфигурации и тип конфигурации
    this->configuration_path_ = path_cfg_;
    this->configuration_type_ = type_cfg_;

    // Загружаем файл конфигурации для чтения
    json::loader::JsonLoader loader_(this->configuration_path_);
    auto obj_cfg_ = loader_.rootObject();

    // Читаем конфигурацию о внешних соединениях процесса и запоминаем результат чтения
    this->configuration_read_ = this->read_configuration(obj_cfg_);
}

bool connections_configuration::read_configuration(const json::object::JsonObject* obj_cfg_) {
    //(?) Если конфигурация внешних соединений процесса валидна, считываем ее
    if (this->configuration_valid(obj_cfg_)) {
        this->configuration_version_ = obj_cfg_->asString("version");

        //(?>) Считываем конфигурацию всех внешних соединений процесса
        auto ar_conns_ = obj_cfg_->asArray("connection_configuration");
        this->connections_.reserve(ar_conns_->size());
        for (int16_t i = 0; i < ar_conns_->size(); ++i) {
            auto conn_cfg_ = ar_conns_->asObject(i);
            if (!conn_cfg_ || !conn_cfg_->hasProperty("connection_type")) {
                this->connections_.clear();
                return false;
            }
            this->read_connection(conn_cfg_);
            if (!this->connections_.back()->configuration_read()) {
                this->connections_.clear();
                return false;
            }
        }

        return true;
    }

    return false;
}

uint8_t connections_configuration::read_connection(const json::object::JsonObject* obj_cfg_) {
    //(?) Читаем конфигурацию внешнего соединения в зависимости от указанного типа
    if (obj_cfg_->asString("connection_type") == TCP_IP_CONNECTION_TYPE) {
        this->connections_.emplace_back(new socket_connection_configuration(obj_cfg_));
        return (!this->connections_.back()->configuration_read()) ? 0x02 : 0x00;
    } else if (obj_cfg_->asString("connection_type") == MANAGED_MEMORY_CONNECTION_TYPE) {
        this->connections_.emplace_back(new managed_memory_connection_configuration(obj_cfg_));
        return (!this->connections_.back()->configuration_read()) ? 0x02 : 0x00;
    }

    return 0x01;
}

/* Требуемый формат конфигурации
{
    "type": ...,
    "version": ...,
    "connection_configuration":
    [
        ...
    ]
}
*/
bool connections_configuration::configuration_valid(const json::object::JsonObject* obj_cfg_) {
    return obj_cfg_ &&
           obj_cfg_->hasProperty("type") &&
           obj_cfg_->asString("type") == this->configuration_type_ &&
           obj_cfg_->hasProperty("version") &&
           obj_cfg_->hasProperty("connection_configuration") &&
           obj_cfg_->asArray("connection_configuration");
}
