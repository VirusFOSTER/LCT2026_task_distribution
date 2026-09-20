#include "process_configuration/messages_configuration/message_configuration.hpp"
#include <algorithm>

#define TAG_UNIQUE_MESSAGE_UNIQUE    (std::string)"unique"
#define TAG_UNIQUE_MESSAGE_SHARED    (std::string)"shared"

#define TAG_ACCES_MESSAGE_MUTABLE   (std::string)"mutable"
#define TAG_ACCES_MESSAGE_UNMUTABLE   (std::string)"unmutable"

using namespace mps;
using namespace config;


message_configuration::message_configuration(const json::object::JsonObject *obj_cfg_) {
    // Считываем конфигурацию сообщения процесса системы и запоминаем результат
    this->configuration_read_ = this->read_configuration(obj_cfg_);
}

bool message_configuration::read_configuration(const json::object::JsonObject* obj_cfg_) {
    //(?) Если указанная конфигурация валидна, считываем ее
    if (this->configuration_valid(obj_cfg_)) {
        this->message_uid_ = obj_cfg_->asInteger("message_uid");
        this->message_name_ = obj_cfg_->asString("message_name");
        this->alt_message_name_ = obj_cfg_->asString("alt_message_name");
        this->buffer_name_ = obj_cfg_->asString("buffer_name");
        this->buffer_size_ = obj_cfg_->asInteger("buffer_size");
        this->message_registration_ = obj_cfg_->asBoolean("registration");

        auto ar_components_ = obj_cfg_->asArray("sources");
        this->sources_.resize(ar_components_->size());
        for (int32_t i = 0; i < ar_components_->size(); ++i) {
            this->sources_[i] = ar_components_->asString(i);
        }

        ar_components_ = obj_cfg_->asArray("targets");
        this->targets_.resize(ar_components_->size());
        for (int32_t i = 0; i < ar_components_->size(); ++i) {
            this->targets_[i] = ar_components_->asString(i);
        }

        // Унифицируем читателей и писателей сообщения
        this->unified_components(this->sources_);
        this->unified_components(this->targets_);

        return this->define_acces_tag(obj_cfg_->asString("message_acces_tag")) &&
               this->define_unique_tag(obj_cfg_->asString("message_unique_tag"));
    }
    return false;
}

/* Требуемый формат конфигурации
 * {
        "message_uid": ...,
        "message_name": ...,
        "alt_message_name": ...,
        "message_acces_tag": ...,
        "message_unique_tag":...,
        "buffer_name": ...,
        "buffer_size": ...,
        "registration": ...,
        "sources":
        [
            ...
        ],
        "targets":
        [
            ...
        ],
        "message_description": ...
} */
bool message_configuration::configuration_valid(const json::object::JsonObject* obj_cfg_) {
    return obj_cfg_ &&
           obj_cfg_->hasProperty("message_uid") &&
           obj_cfg_->hasProperty("message_name") &&
           obj_cfg_->hasProperty("alt_message_name") &&
           obj_cfg_->hasProperty("message_acces_tag") &&
           obj_cfg_->hasProperty("message_unique_tag") &&
           obj_cfg_->hasProperty("buffer_name") &&
           obj_cfg_->hasProperty("buffer_size") &&
           obj_cfg_->hasProperty("registration") &&
           obj_cfg_->hasProperty("sources") &&
           obj_cfg_->asArray("sources") &&
           obj_cfg_->hasProperty("targets") &&
           obj_cfg_->asArray("targets");
}

void message_configuration::unified_components(std::vector<std::string>& components_) {
    // Удаляем дубликаты сообщений
    std::sort(components_.begin(),components_.end());
    auto last_ = std::unique(components_.begin(),components_.end());
    components_.erase(last_,components_.end());
}

bool message_configuration::define_acces_tag(const std::string& tg_) {
    //(?) Если для сообщения доступ установлен только на чтение, поднимаем соответствующий флаг доступа
    if (tg_ == TAG_ACCES_MESSAGE_MUTABLE) {
        this->message_tag_ |= message_tag::_tg_mutable_message_;
        return true;
    }

    // В противном случае проверяем валидность указанных данных
    return (tg_ == TAG_ACCES_MESSAGE_UNMUTABLE);
}

bool message_configuration::define_unique_tag(const std::string& tg_) {
    //(?) Если для сообщения установлена уникальность (читает только одна компонента),
    // поднимаем соответствующий флаг уникальности
    if (tg_ == TAG_UNIQUE_MESSAGE_UNIQUE) {
        this->message_tag_ |= message_tag::_tg_unique_message_;
        return true;
    }

    // В противном случае проверяем валидность указанных данных
    return (tg_ == TAG_UNIQUE_MESSAGE_SHARED);
}

bool message_configuration::component_is_writer(const std::string& component_name_) {
    //(?>) Ищем в массиве компоненту с указанным именем, и если такая имеется, возвращаем полложительный ответ
    auto itr_ = std::find_if(this->sources_.begin(),this->sources_.end(),
                             [ &component_name_ ](auto& el_){ return component_name_ == el_; });
    return itr_ != this->sources_.end();
}


bool message_configuration::component_is_reader(const std::string& component_name_) {
    //(?>) Ищем в массиве компоненту с указанным именем, и если такая имеется, возвращаем полложительный ответ
    auto itr_ = std::find_if(this->targets_.begin(),this->targets_.end(),
                             [ &component_name_ ](auto& el_){ return component_name_ == el_; });
    return itr_ != this->targets_.end();
}
