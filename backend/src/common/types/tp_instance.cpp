#include "common/types/tp_instance.hpp"

using namespace td;
using namespace types;


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

        return true;
    }

    // В противном случае возвращаем соответствующий результат
    return false;
}

//-------------------------------------------------
/*
{
    "instance_uid": 0,
    "instance_name": "name_1",
    "intance_region": "yugotsentr",
    "instance_competence":
    [
        1, 1, 1, 0
    ]
}
 */
bool tp_instance::configuration_valid(const mps::json::object::JsonObject* obj_cfg_) {
    return obj_cfg_ &&
           obj_cfg_->hasProperty("instance_uid") &&
           obj_cfg_->hasProperty("instance_name") &&
           obj_cfg_->hasProperty("instance_region") &&
           obj_cfg_->hasProperty("instance_competence") &&
           obj_cfg_->asArray("instance_competence");
}
