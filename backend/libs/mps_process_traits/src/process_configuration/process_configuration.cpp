#include "process_configuration/process_configuration.hpp"

using namespace mps;
using namespace config;


process_configuration::process_configuration(const std::string &proc_name_, const std::string &path_cfg_) {
    // Запоминаем путь к файлу конфигурации и наименование запускаемого процесса системы
    this->configuration_path_ = path_cfg_;
    this->process_name_ = proc_name_;

    // Загружаем файл конфигурации
    json::loader::JsonLoader loader_(path_cfg_);
    auto obj_cfg_ = loader_.rootObject();

    // Считываем конфигурацию процесса системы и запоминаем результат чтения
    this->configuration_read_ = this->read_configuration(obj_cfg_);
}

bool process_configuration::read_configuration(const json::object::JsonObject *obj_cfg_) {
    if (this->configuration_valid(obj_cfg_)) {
        this->configuration_type_ = obj_cfg_->asString("type");
        this->configuration_version_ = obj_cfg_->asString("version");
        this->process_uid_ = obj_cfg_->asInteger("process_uid");

        auto proc_config_ = obj_cfg_->asObject("process_configuration");

        //(?) Определяем локальную конфигурацию процесса системы
        if (proc_config_->hasProperty("local_configuration")) {
            this->local_configuration_ = proc_config_->asString("local_configuration");
            this->local_configuration_ = this->make_full_path(this->local_configuration_);
        }

        return
            this->read_local_configuration(
                this->components_configuration_,proc_config_->asObject("components_configuration")) &&
            this->read_local_configuration(
                this->messages_configuration_,proc_config_->asObject("messages_configuration")) &&
            this->read_local_configuration(
                this->connections_configuration_,proc_config_->asObject("connection_configuration"));
    }
    return false;
}

/* Требуемый формат конфигурации процесса системы
 * {
    "type": ...,
    "version": ...,
    "process_uid": ...,
    "process_name": ...,
    "process_configuration":
    {
        "components_configuration":
        {
            "type": ...,
            "path": ...,
        },
        "messages_configuration":
        {
            "type": ...,
            "path": ...
        },
        "connection_configuration":
        {
            "type": ...,
            "path": ...
        },
        "local_configuration": ...
    }
} */
bool process_configuration::configuration_valid(const json::object::JsonObject *obj_cfg_) {
    return obj_cfg_ &&
           obj_cfg_->hasProperty("type") &&
           obj_cfg_->hasProperty("version") &&
           obj_cfg_->hasProperty("process_uid") &&
           obj_cfg_->hasProperty("process_name") &&
           obj_cfg_->asString("process_name") == this->process_name_ &&
           obj_cfg_->hasProperty("process_configuration") &&
           obj_cfg_->asObject("process_configuration") &&
           obj_cfg_->asObject("process_configuration")->hasProperty("components_configuration") &&
           obj_cfg_->asObject("process_configuration")->hasProperty("messages_configuration") &&
           obj_cfg_->asObject("process_configuration")->hasProperty("connection_configuration") &&
           obj_cfg_->asObject("process_configuration")->hasProperty("local_configuration") &&
           this->local_configuration_valid(obj_cfg_->asObject("process_configuration")->asObject("components_configuration")) &&
           this->local_configuration_valid(obj_cfg_->asObject("process_configuration")->asObject("messages_configuration")) &&
           this->local_configuration_valid(obj_cfg_->asObject("process_configuration")->asObject("connection_configuration"));
}

bool process_configuration::local_configuration_valid(const json::object::JsonObject* obj_cfg_) {
    return obj_cfg_ &&
           obj_cfg_->hasProperty("type") &&
           obj_cfg_->hasProperty("path");
}
