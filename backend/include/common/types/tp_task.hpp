#ifndef TASK_DISTRIBUTION_TASK_TYPE_HPP
#define TASK_DISTRIBUTION_TASK_TYPE_HPP

#include <mps/mps_common/utils/json_io/json.hpp>
#include "utils/tp_position.hpp"
#include "utils/tp_time_window.hpp"
#include "tg_task.hpp"

namespace td {
namespace types {
/** ------------------------------------------------------------------------------------------------------
 * @brief The tp_task class - описание выполняемой задачи
 * Самое описание считывается из json-файла
 --------------------------------------------------------------------------------------------------------*/
class tp_task {
public:
    /**
     * @brief tp_task - конструктор
     * @param obj_cfg_ - описание задачи
     */
    explicit tp_task(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * деструктор
     */
    ~tp_task();

private:
    /**
     * @brief read_configuration - чтение конфигурации (описания исполняемой задачи)
     * @param obj_cfg_ - указатель на описание задачи
     * @return результат чтения описания задачи
     */
    bool read_configuration(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация описания задачи на валидность
     * @param obj_cfg_ - указатель на описание задачи
     * @return результат верификации на валидность
     */
    bool configuration_valid(const mps::json::object::JsonObject* obj_cfg_);

private:
    bool description_valid_ = false;                /// <--- признак чтения описания задачи на исполнение

    uint32_t task_uid_ = 0;                         /// <--- уникальный идентификатор задачи на исоплнение
    tg_task task_type_ = tg_task::_tg_unknown_;     /// <--- тип задачи на выполнение
    tp_position position_;                          /// <--- положение задачи на карте
    tp_time_window time_window_;                    /// <--- временное окно выполнения задачи
    std::string task_region_ = "";                  /// <--- регион исполняемой задачи
};
}       /// <--- types
}   /// <--- td

#endif
