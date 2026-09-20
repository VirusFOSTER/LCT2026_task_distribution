#include "process_configuration/components_configuration/components_configuration.hpp"

using namespace mps;
using namespace config;


components_configuration::components_configuration(const std::string &type_cfg_, const std::string &path_cfg_) {
    // Запоминаем путь к файл конфигурации компонент процесса системы и ожидаемый тип конфигурации
    this->configuration_path_ = path_cfg_;
    this->configuration_type_ = type_cfg_;

    // Загружаем указанный файл конфигурации
    json::loader::JsonLoader jsl_(this->configuration_path_);
    auto root_ = jsl_.rootObject();

    // Считываем конфигурацию компонент процесса системы и запоминаем результат
    this->configuration_read_ = this->read_configuration(root_);
}

bool components_configuration::read_configuration(const json::object::JsonObject *obj_cfg_) {
    //(?) Если конфигурация компонент процесса системы валидна, считываем ее
    if (this->configuration_valid(obj_cfg_)) {
        this->configuration_version_ = obj_cfg_->asString("version");

        auto ar_components_ = obj_cfg_->asArray("components_configuration");
        this->components_.reserve(ar_components_->size());

        //(?>) Пытаемся прочитать конфигурацию всех компонент процесса системы
        // Если конфигурация очередной компоненты не валидна, то в массив компонент не добавляем
        for (int32_t i = 0; i < ar_components_->size(); ++i) {
            auto obj_component_cfg_ = ar_components_->asObject(i);
            this->components_.emplace_back(component_configuration(obj_component_cfg_));
            if (!this->components_.back().configuration_read()) {
                this->components_.pop_back();
            }
            // Увеличиваем количество активных компонент
            if (this->components_.back().component_active()) {
                this->active_components_count_++;
            }
        }

        return true;
    }

    return false;
}

/* Требуемый тип конфигурации компонент системы
 * {
    "type": ...,
    "version": ...,
    "components_configuration":
    [
        {
            "component_uid": ...,
            "component_name": ...,
            "component_active": ...,
            "component_respawn": ...,
            "component_count": ...,
            "component_description": ...
    ]
}*/
bool components_configuration::configuration_valid(const json::object::JsonObject *obj_cfg_) {
    return obj_cfg_ &&
            obj_cfg_->hasProperty("type") && obj_cfg_->asString("type") == this->configuration_type_ &&
            obj_cfg_->hasProperty("version") &&
            obj_cfg_->hasProperty("components_configuration") &&
            obj_cfg_->asArray("components_configuration");
}
