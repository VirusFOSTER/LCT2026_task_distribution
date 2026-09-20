#ifndef SYSTEM_PROCESS_COMPONENT_CONFIGURATION_HPP
#define SYSTEM_PROCESS_COMPONENT_CONFIGURATION_HPP

#include <mps/mps_common/utils/json_io/json.hpp>

namespace mps {
namespace config {
/** --------------------------------------------------------------------------------------------------------------------------
 * @brief The component_configuration class - конфигурация отдельно взятой компоненты процесса системы
 ----------------------------------------------------------------------------------------------------------------------------*/
class component_configuration {
public:
    /**
     * @brief component_configuration - конструктор
     * @param obj_cfg_ - указатель на головной объект конфигурации отдельно взятой компоненты процесса системы
     */
    explicit component_configuration(const json::object::JsonObject* obj_cfg_);

    /**
     * Деструктор
     */
    ~component_configuration() = default;

    inline bool configuration_read() const { return this->configuration_read_; }
    inline bool component_active() const { return this->component_active_; }
    inline bool component_respawn() const { return this->component_respawn_; }
    inline uint8_t component_count() const { return this->component_count_; }
    inline uint32_t component_uid() const { return this->component_uid_; }
    inline std::string component_name() const { return this->component_name_; }
    inline std::string component_description() const { return this->component_description_; }

private:
    /**
     * @brief read_configuration - чтение конфигурации отдельно взятой компоненты процесса системы
     * @param obj_cfg_ - указатель на головной объект конфигурации отдельно взятой компоненты процесса системы
     * @return результат чтения конфигурации
     */
    bool read_configuration(const json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация конфигурации отдельно взятой компоненты процесса системы на валидность
     * @param obj_cfg_ - указатель на головной объект конфигурации отдельно взятой компоненты процесса системы
     * @return результат верификации на валидность
     */
    bool configuration_valid(const json::object::JsonObject* obj_cfg_);

private:
    bool configuration_read_ = false;           /// <--- признак чтения конфигурации компоненты процесса системы
    bool component_active_ = false;             /// <--- признак активности компонента процесса системы
    bool component_respawn_ = false;            /// <--- признак перезапуска компоненты в случае падения
    uint8_t component_count_ = 0;               /// <--- количество запускаемых компонент данного типа в процессе
    uint32_t component_uid_ = 0;                /// <--- уникальный идентификатор компоненты процесса системы
    std::string component_name_ = "";           /// <--- уникальное наименование компоненты процесса системы
    std::string component_description_ = "";    /// <--- описание компоненты процесса системы (путь к файлу описания)
};
}
}

#endif
