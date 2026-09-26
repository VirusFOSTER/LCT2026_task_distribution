#ifndef TASK_DISTRIBUTION_INSTANCE_TYPE_HPP
#define TASK_DISTRIBUTION_INSTANCE_TYPE_HPP

#include <mps/mps_common/utils/json_io/json.hpp>
#include "tg_instances.hpp"
#include "utils/tp_position.hpp"

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
     * @brief tp_instance - конструктор (по умолчанию)
     */
    explicit tp_instance() = default;

    /**
     * @brief tp_instance - конструктор
     * @param obj_cfg_ - конфигурация (описание) исполнителя задач
     */
    explicit tp_instance(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * @brief tp_instance - конструктор копирования
     * @param inst_ - описание копируемого исоплнителя
     */
    tp_instance(const tp_instance& inst_);

    /**
     * @brief tp_instance - конструктор перемещения
     * @param inst_ - описание перемещаемого исполнителя
     */
    tp_instance(tp_instance&& inst_) noexcept;

    /**
     * @brief operator = - оператор считывания описания исполнителя из формата json
     * @param obj_cfg_ - указатель на описание исполнителя
     * @return сформированный исполнитель
     */
    tp_instance& operator=(const mps::json::object::JsonObject* obj_cfg_);

    /**
     * @brief operator = - оператор копирования исоплнителя
     * @param inst_ - копируемый исполнитель
     * @return копия исоплнителя
     */
    tp_instance& operator=(const tp_instance& inst_);

    /**
     * @brief operator = - оператор перемещения исполнителя
     * @param inst_ - пермещаемый исполнитель
     * @return перемещенный исполнитель
     */
    tp_instance& operator=(tp_instance&& inst_) noexcept;

    /**
     * деструктор
     */
    ~tp_instance() = default;

    inline bool description_valid() const { return this->description_valid_; }
    inline uint16_t instance_uid() const { return this->instance_uid_; }
    inline std::string instance_name() const { return this->instance_name_; }
    inline std::string instance_region() const { return this->instance_region_; }
    inline uint8_t compoetence() const { return this->byte_competence_; }
    inline tg_instance tag() const { return this->instance_tag_; }
    inline tg_moving moving_tag() const { return this->moving_tag_; }
    inline tp_position position() const { return this->position_; }

    inline void set_instance_uid(uint16_t uid_) { this->instance_uid_ = uid_; }
    inline void set_instance_name(const std::string& name_) { this->instance_name_ = name_; }
    inline void set_instance_region(const std::string& region_) { this->instance_region_ = region_; }
    inline void set_competence(uint8_t b_) { this->byte_competence_ = b_; }
    inline void set_tag(tg_instance tg_) { this->instance_tag_ = tg_; }
    inline void set_moving_tag(tg_moving tg_) { this->moving_tag_ = tg_; }
    inline void set_position(const tp_position& pose_) { this->position_ = pose_; }

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
    bool description_valid_ = false;                                /// <--- Признак чтения описания исполнителя

    uint16_t instance_uid_ = 0;                                     /// <--- уникальный идентификатор исполнителя
    std::string instance_name_ = "";                                /// <--- уникальное имя исполнителя
    std::string instance_region_ = "";                              /// <--- регион обработки задач исполнителем
    uint8_t byte_competence_ = 0x00;                                /// <--- байт компетентности
    tg_instance instance_tag_ = tg_instance::_tg_status_unknown_;   /// <--- метка исполнителя по статусу работ
    tg_moving moving_tag_ = tg_moving::_tg_unknown_;                /// <--- тип перемещения исполнителя задач
    tp_position position_;                                          /// <--- текущее положение исполнителя
};
}       /// <--- types
}   /// <--- td

#endif
