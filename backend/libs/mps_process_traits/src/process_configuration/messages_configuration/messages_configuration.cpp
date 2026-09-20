#include "process_configuration/messages_configuration/messages_configuration.hpp"

using namespace mps;
using namespace config;


messages_configuration::messages_configuration(const std::string &type_cfg_, const std::string &path_cfg_) {
    // Запоминаем полный путь к файлу конфигурации и ожидаемый тип конфигурации
    this->configuration_path_ = path_cfg_;
    this->configuration_type_ = type_cfg_;

    // Загружаем файл конфигурации
    json::loader::JsonLoader loader_(this->configuration_path_);
    auto obj_cfg_ = loader_.rootObject();

    this->configuration_read_ = this->read_configuration(obj_cfg_);
}

bool messages_configuration::read_configuration(const json::object::JsonObject* obj_cfg_) {
    //(?) Если конфигурация валидна, считываем ее
    if (this->configuration_valid(obj_cfg_)) {
        this->configuration_version_ = obj_cfg_->asString("version");

        auto ar_messages_ = obj_cfg_->asArray("messages");
        this->messages_.reserve(ar_messages_->size());
        for (int32_t i = 0; i < ar_messages_->size(); ++i) {
            auto msg_cfg_ = ar_messages_->asObject(i);
            this->messages_.emplace_back(message_configuration(msg_cfg_));
            if (!this->messages_.back().configuration_read()) {
                this->messages_.pop_back();
            }
        }

        return true;
    }

    // В противном случае возвращаем отрицательный результат чтения конфигурации
    return false;
}

/** Требуемый формат конфигурации
 * {
    "type": ...,
    "version": ...,
    "messages":
    [
        ...
    ]
    }
 */
bool messages_configuration::configuration_valid(const json::object::JsonObject* obj_cfg_) {
    return obj_cfg_ && obj_cfg_->hasProperty("type") &&
           obj_cfg_->asString("type") == this->configuration_type_ &&
           obj_cfg_->hasProperty("version") &&
           obj_cfg_->hasProperty("messages") &&
           obj_cfg_->asArray("messages");
}
