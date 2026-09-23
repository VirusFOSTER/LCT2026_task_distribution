#include "common/types/tp_task.hpp"


#define CONNECTION_TASK_TYPE        (std::string)"connection"
#define ADDITIONAL_ORDER_TASK_TYPE  (std::string)"additional_order"
#define LOCAL_REQUEST_TASK_TYPE     (std::string)"local_request"
#define GLOBAL_PROBLEM_TASK_TYPE    (std::string)"global_problem"

using namespace td;
using namespace types;


//-------------------------------------------------

tp_task::tp_task(const mps::json::object::JsonObject* obj_cfg_) {
    // Читаем описание задачи и запоминаем результат чтения
    this->description_valid_ = this->read_configuration(obj_cfg_);
}

//-------------------------------------------------

bool tp_task::read_configuration(const mps::json::object::JsonObject* obj_cfg_) {
    //(?) Если описание задачи валидно, считываем его и возвращаем результат
    if (this->configuration_valid(obj_cfg_)) {
        this->task_uid_ = obj_cfg_->asInteger("task_uid");
        this->task_region_ = obj_cfg_->asString("task_region");

        //(?) Тип задачи должен быть валидным,
        // в противном случае, не знаем, что это такое
        this->define_task(obj_cfg_);
        if (this->task_type_ == tg_task::_tg_unknown_) {
            return false;
        }

        // Указываем местоположение выполнения задачи (в координатах)
        this->position_.set_longitude(obj_cfg_->asObject("position")->asReal("longitude"));
        this->position_.set_latitude(obj_cfg_->asObject("position")->asReal("latitude"));

        // Указываем временное окно
        this->time_window_.set_time_begin(obj_cfg_->asObject("time_window")->asString("time_begin"));
        this->time_window_.set_time_end(obj_cfg_->asObject("time_window")->asString("time_end"));

        return true;
    }

    // В противном случае возвращаем соответствующий результат
    return false;
}

//-------------------------------------------------

void tp_task::define_task(const mps::json::object::JsonObject* obj_cfg_) {
    auto tp_ = obj_cfg_->asString("task_type");

    if (tp_ == CONNECTION_TASK_TYPE) {
        this->task_type_ = tg_task::_tg_connection_;
    } else if ((tp_ == ADDITIONAL_ORDER_TASK_TYPE) || (tp_ == LOCAL_REQUEST_TASK_TYPE)) {
        this->task_type_ = tg_task::_tg_local_task_;
    } else if (tp_ == GLOBAL_PROBLEM_TASK_TYPE) {
        this->task_type_ = tg_task::_tg_emergency_;
    }
}

//-------------------------------------------------
/* Требуемый формат описания задачи
{
    "task_uid": ...,
    "task_type": ...,
    "task_region": ...,
    "position":
    {
        "latitude": ...,
        "longitude": ...
    },
    "time_window":
    {
        "time_begin": ...,
        "time_end": ...
    }
}
 */
bool tp_task::configuration_valid(const mps::json::object::JsonObject* obj_cfg_) {
    return obj_cfg_ &&
           obj_cfg_->hasProperty("task_uid") &&
           obj_cfg_->hasProperty("task_type") &&
           obj_cfg_->hasProperty("task_region") &&
           obj_cfg_->hasProperty("position") &&
           obj_cfg_->asObject("position") &&
           obj_cfg_->asObject("position")->hasProperty("latitude") &&
           obj_cfg_->asObject("position")->hasProperty("longitude") &&
           obj_cfg_->hasProperty("time_window") &&
           obj_cfg_->asObject("time_window") &&
           obj_cfg_->asObject("time_window")->hasProperty("time_begin") &&
           obj_cfg_->asObject("time_window")->hasProperty("time_end");
}
