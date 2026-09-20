#ifndef SYSTEM_PROCESS_MESSAGES_CONFIGURATION_HPP
#define SYSTEM_PROCESS_MESSAGES_CONFIGURATION_HPP

#include "message_configuration.hpp"
#include "process_configuration/sglobal_config.hpp"

namespace mps {
namespace config {
/** -----------------------------------------------------------------------------------------------------------------------
 * @brief The messages_configuration class - конфигурация всех сообщений, фигурирующих между компонентами процесса системы
 ------------------------------------------------------------------------------------------------------------------------*/
class messages_configuration {
    SCONFIG_GLOBAL

public:
    /**
     * @brief messages_configuration - конструктор
     * @param type_cfg_ - тип конфигурации сообщений процесса системы (ожидаемый тип)
     * @param path_cfg_ - полный путь к файлу конфигурации сообщений процесса системы
     */
    explicit messages_configuration(const std::string& type_cfg_, const std::string& path_cfg_);

    /**
     * Деструктор
     */
    ~messages_configuration() = default;

    /**
     * @brief size - получение количества сообщений процесса системы (согласно конфигурации)
     * @return количество сообщений процесса системы (согласно конфигурации)
     */
    inline uint16_t size() const { return this->messages_.size(); }

    /**
     * @brief operator () - получение указателя на конфигурацию сообщения
     * @param i - индекс сообщения в массиве конфигураций сообщений процеса системы
     * @return указатель на конфигурацию сообщения
     */
    inline message_configuration* operator()(uint16_t& i) {
        return (i < this->messages_.size()) ? &this->messages_[i] : nullptr;
    }

private:
    /**
     * @brief read_configuration - метод чтения конфигурации сообщений, фигурирующих между компонентами процесса системы
     * @param obj_cfg_ - указатель на головной объект конфигурации сообщений процесса системы
     * @return результат чтения конфигурации сообщений процесса системы
     */
    bool read_configuration(const json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация конфигурации сообщений процесса системы на валидность
     * @param obj_cfg_ - указатель на головной объект конфигурации сообщений процесса системы
     * @return результат верификации конфигурации сообщений процесса системы на валидность
     */
    bool configuration_valid(const json::object::JsonObject* obj_cfg_);

private:
    std::vector<message_configuration> messages_ = {};  /// <--- конфигурация сообщений процесса системы
};
}       /// <--- config
}   /// <--- mps

#endif
