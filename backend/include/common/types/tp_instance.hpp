#ifndef TASK_DISTRIBUTION_INSTANCE_TYPE_HPP
#define TASK_DISTRIBUTION_INSTANCE_TYPE_HPP

#include <mps/mps_common/utils/json_io/json.hpp>

namespace td {
namespace types {
/** -----------------------------------------------------------------------------------------------------------------
 * @brief The tp_instance class - описание исполнителя задач
 * Каждому исполнителю назначается определенное количество задач. В результате формируется полноценный список задач
 * на исполнение.
 --------------------------------------------------------------------------------------------------------------------*/
class tp_instance {
public:
    /**
     * @brief tp_instance - конструктор
     * @param obj_cfg_ - конфигурация (описание) исполнителя задач
     */
    explicit tp_instance(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * деструктор
     */
    ~tp_instance() = default;

private:
    /**
     * @brief read_configuration - метод чтения конфигурации (описания) исполнителя задач
     * @param obj_cfg_ - указатель на описание исполнителя задач
     * @return результат чтения описания
     */
    bool read_configuration(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация описания исполнителя задач на валидность
     * @param obj_cfg_ - указатель на описание исполнителя задач
     * @return результат верификации
     */
    bool configuration_valid(const mps::json::object::JsonObject* obj_cfg_);

private:
    bool description_valid_ = false;        /// <--- Признак чтения описания исполнителя

    uint16_t instance_uid_ = 0;             /// <--- уникальный идентификатор исполнителя
    std::string instance_name_ = "";        /// <--- уникальное имя исполнителя
    std::string instance_region_ = "";      /// <--- регион обработки задач исполнителем
    uint8_t byte_competence_ = 0x00;        /// <--- байт компетентности
};
}       /// <--- types
}   /// <--- td

#endif
