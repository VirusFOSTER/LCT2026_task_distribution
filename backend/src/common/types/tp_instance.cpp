#include "common/types/tp_instance.hpp"

using namespace td;
using namespace types;

static constexpr std::string PROFILE_CAR        = "car";
static constexpr std::string PROFILE_MOTORBIKE  = "motorbike";
static constexpr std::string PROFILE_BICYCLE    = "bicycle";
static constexpr std::string PROFILE_PEDESTRIAN = "pedestrian";
static constexpr std::string PROFILE_TRANSIT    = "transit";

//-------------------------------------------------

tp_instance::tp_instance(const mps::json::object::JsonObject* obj_cfg_) {
    // ПОлучаем описание исполнителя задачи и фиксируем результат чтения
    this->description_valid_ = this->read_configuration(obj_cfg_);
}

//-------------------------------------------------
/*
    uint16_t instance_uid_ = 0;             /// <--- уникальный идентификатор исполнителя
    std::string instance_name_ = "";        /// <--- уникальное имя исполнителя
    std::string instance_region_ = "";      /// <--- регион обработки задач исполнителем
    uint8_t byte_competence_ = 0x00;        /// <--- байт компетентности
 */
bool tp_instance::read_configuration(const mps::json::object::JsonObject* obj_cfg_) {
    //(?) Если описание исполнителя задач валидно, считываем его и возвращаем результат
    if (this->configuration_valid(obj_cfg_)) {
        this->instance_uid_ = obj_cfg_->asInteger("instance_uid");
        this->instance_name_ = obj_cfg_->asString("instance_name");
        this->instance_region_ = obj_cfg_->asString("instance_region");

        auto ar_competence_ = obj_cfg_->asArray("instance_competence");
        for (int8_t i = 0; i < ar_competence_->size(); ++i) {
            if (ar_competence_->asInteger(i)) {
                this->byte_competence_ |= static_cast<uint8_t>(1u << i);
            }
        }
        // (!) Заметка: количество типов задач ограничено. Имеется допущение, что этих типов не больше чем 8

        // Определяем тип движения исполнителя задач
        this->moving_tag_ = this->define_moving_tag(obj_cfg_->asString("instance_moving_type"));
        if (this->moving_tag_ == tg_moving::_tg_unknown_) {
            return false;
        }

        // Определяем стартовую позицию исполнителя задач
        this->position_.set_latitude(obj_cfg_->asObject("instance_start_position")->asReal("latitude"));
        this->position_.set_longitude(obj_cfg_->asObject("instance_start_position")->asReal("longitude"));

        return true;
    }

    // В противном случае возвращаем соответствующий результат
    return false;
}

//-------------------------------------------------

tg_moving tp_instance::define_moving_tag(const std::string& str_tg_) {
    if (str_tg_ == PROFILE_CAR) { return tg_moving::_tg_car_; }
    else if (str_tg_ == PROFILE_MOTORBIKE) { return tg_moving::_tg_motorbike_; }
    else if (str_tg_ == PROFILE_BICYCLE) { return tg_moving::_tg_bicycle_; }
    else if (str_tg_ == PROFILE_TRANSIT) { return tg_moving::_tg_public_transport_; }
    else if (str_tg_ == PROFILE_PEDESTRIAN) { return tg_moving::_tg_pedestrian_; }

    return tg_moving::_tg_unknown_;
}

//-------------------------------------------------
/*
{
    "instance_uid": ...,
    "instance_name": ...,
    "intance_region": ...,
    "instance_moving_type": ...,
    "instance_start_position":
    {
        "longitude": ...,
        "latitude": ...
    },
    "instance_competence":
    [
        ...
    ]
}
 */
bool tp_instance::configuration_valid(const mps::json::object::JsonObject* obj_cfg_) {
    return obj_cfg_ &&
           obj_cfg_->hasProperty("instance_uid") &&
           obj_cfg_->hasProperty("instance_name") &&
           obj_cfg_->hasProperty("instance_region") &&
           obj_cfg_->hasProperty("instance_moving_type") &&
           obj_cfg_->hasProperty("instance_start_position") &&
           obj_cfg_->asObject("instance_start_position") &&
           obj_cfg_->asObject("instance_start_position")->hasProperty("longitude") &&
           obj_cfg_->asObject("instance_start_position")->hasProperty("latitude") &&
           obj_cfg_->hasProperty("instance_competence") &&
           obj_cfg_->asArray("instance_competence");
}
