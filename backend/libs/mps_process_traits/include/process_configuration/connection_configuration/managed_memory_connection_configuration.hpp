#ifndef SYSTEM_PROCESS_MANAGED_MEMORY_CONNECTION_CONFIGURATION_HPP
#define SYSTEM_PROCESS_MANAGED_MEMORY_CONNECTION_CONFIGURATION_HPP

#include "connection_configuration.hpp"

namespace mps {
namespace config {
/** ------------------------------------------------------------------------------------------------------------------------
 * @brief The managed_memory_connection_configuration class - конфигурация отдельно взятого внешнего соединения между
 * процессами системы. Данная конфигурация предназначена для соединения между процессами через общий сегмент памяти
 -------------------------------------------------------------------------------------------------------------------------*/
class managed_memory_connection_configuration : public connection_configuration {
public:
    /**
     * @brief managed_memory_connection_configuration - конструктор
     * @param obj_cfg_ - указатель на объект конфигурации
     */
    explicit managed_memory_connection_configuration(const json::object::JsonObject* obj_cfg_);

    explicit managed_memory_connection_configuration() = default;

    /**
     * деструктор
     */
    ~managed_memory_connection_configuration() = default;

    /**
     * @brief memory_name - получение наименования общего для процессов сегмента памяти
     * @return наименование общего для процессов сегмента памяти
     */
    inline std::string memory_name() const { return this->memory_name_; }

    /**
     * @brief messages_count - получение количества сообщений в буфере
     * @return количество сообщений в буфере
     */
    inline uint32_t messages_count() const { return this->messages_count_; }

private:
    /**
     * @brief read_configuration - чтение конфигурации внешнего соединения с другими процессами
     * @param obj_cfg_ - указатель на объект конфигурации
     * @return результат чтения конфигурации
     */
    bool read_configuration(const json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация конфигурации внешнего соединения на валидность
     * @param obj_cfg_ - указатель на объект конфигурации
     * @return результат верификации на валидность
     */
    bool configuration_valid(const json::object::JsonObject* obj_cfg_);

private:
    std::string memory_name_ = "";          /// <--- наименование сегмента общей для процессов памяти
    uint32_t messages_count_ = 0;           /// <--- количество сообщений в буфере
};
}       /// <--- config
}   /// <--- mps

#endif
