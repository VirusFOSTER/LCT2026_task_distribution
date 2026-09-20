#include "process_configuration/components_configuration/component_configuration.hpp"

using namespace mps;
using namespace config;


component_configuration::component_configuration(const json::object::JsonObject *obj_cfg_) {
    // Считываем конфигурацию компоненты процесса системы и запоминаем результат
    this->configuration_read_ = this->read_configuration(obj_cfg_);
}

bool component_configuration::read_configuration(const json::object::JsonObject *obj_cfg_) {
    //(?) Если конфигурация компоненты процесса системы валидна, то считываем ее
    // При этом компонента сама по себе может быть неактивна. В этом случае, все равно будет сформирована компонента,
    // т.к. ее запуск может быть произведен в процессе работы системы
    if (this->configuration_valid(obj_cfg_)) {
        this->component_uid_ = obj_cfg_->asInteger("component_uid");
        this->component_name_ = obj_cfg_->asString("component_name");
        this->component_active_ = obj_cfg_->asBoolean("component_active");
        this->component_respawn_ = obj_cfg_->asBoolean("component_respawn");
        this->component_count_ = obj_cfg_->asInteger("component_count");

        if (obj_cfg_->hasProperty("component_description")) {
            this->component_description_ = obj_cfg_->asString("component_description");
        }

        return true;
    }
    return false;
}

/* Требуемый тип конфигурации компоненты процесса системы
 *
    {
        "component_uid": ...,
        "component_name": ...,
        "component_active": ...,
        "component_respawn": ...,
        "component_count": ...,
        "component_description": ...  (является необязательным полем)
    }
} */
bool component_configuration::configuration_valid(const json::object::JsonObject *obj_cfg_) {
    return obj_cfg_ &&
            obj_cfg_->hasProperty("component_uid") &&
            obj_cfg_->hasProperty("component_name") &&
            obj_cfg_->hasProperty("component_active") &&
            obj_cfg_->hasProperty("component_respawn") &&
            obj_cfg_->hasProperty("component_count");
}
