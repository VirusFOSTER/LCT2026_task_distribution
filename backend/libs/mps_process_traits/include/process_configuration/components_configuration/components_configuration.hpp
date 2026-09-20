#ifndef SYSTEM_PROCESS_COMPONENTS_CONFIGURATION_HPP
#define SYSTEM_PROCESS_COMPONENTS_CONFIGURATION_HPP

#include "process_configuration/sglobal_config.hpp"
#include "component_configuration.hpp"

namespace mps {
namespace config {
/** ---------------------------------------------------------------------------------------------------------------------------
 * @brief The components_configuration class - конфигурация компонент процесса системы
 * Каждая компонента процесса системы, если она активна, запускается в отдельном потоке. Причем запускаемая компонента
 * должна быть в обязательном порядке зарезервирована (на программном уровне) в самом исполнителе процесса (process_runner)
 ----------------------------------------------------------------------------------------------------------------------------*/
class components_configuration {
    SCONFIG_GLOBAL

public:
    /**
     * @brief components_configuration - конструктор
     * @param type_cfg_ - тип конфигурации компонент процесса системы (ожидаемый тип)
     * @param path_cfg_ - полный путь к файлу конфигурации компонент процесса системы
     */
    explicit components_configuration(const std::string& type_cfg_, const std::string& path_cfg_);

    /**
     * Деструктор
     */
    ~components_configuration() = default;

    /**
     * @brief components_count - получение количества компонент (согласно конфигурации)
     * @return количество компонент
     */
    inline uint16_t components_count() const { return this->components_.size(); }

    /**
     * @brief active_components_count - получение количества активных компонент
     * @return количество активных компонент
     */
    inline uint16_t active_components_count() const { return this->active_components_count_; }

    /**
     * @brief operator () - оператор получения указателя на конфигурацию отдельно взятой компоненты
     * @param i - идентификатор компоненты в массиве
     * @return указатель на конфигурацию отдельно взятой компоненты
     */
    inline const component_configuration* const operator()(uint16_t& i) {
        return (i < this->components_.size()) ? &this->components_[i] : nullptr;
    }

private:
    /**
     * @brief read_configuration - чтение конфигурации компонент процесса системы
     * @param obj_cfg_ - головной указатель на конфигурацию компонент процесса системы
     * @return результат чтения конфигурации компонент процесса системы
     */
    bool read_configuration(const json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация конфигурации компонент процесса системы на валидность
     * @param obj_cfg_ - указатель на головной объект конфигурации компонент процесса системы
     * @return результат верификации на валидность
     */
    bool configuration_valid(const json::object::JsonObject* obj_cfg_);

private:
    std::vector<component_configuration> components_ = {};  /// <--- массив конфигурации компонент процесса системы
    uint16_t active_components_count_ = 0;      /// <--- количество активных компонент процесса системы
};
}
}

#endif
