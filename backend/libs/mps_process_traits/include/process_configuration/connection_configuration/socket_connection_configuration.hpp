#ifndef SYSTEM_PROCESS_SOCKET_CONNECTION_CONFIGURATION_HPP
#define SYSTEM_PROCESS_SOCKET_CONNECTION_CONFIGURATION_HPP

#include "connection_configuration.hpp"

namespace mps {
namespace config {
/** ---------------------------------------------------------------------------------------------------------------------------
 * @brief The connection_configuration class - конфигурация отдельно взятого внешнего соединения с другим  процессами системы
 * Конкретно эта конфигурация предназначена для соединения между процессами через сокет (tcp/ip - connection)
 ----------------------------------------------------------------------------------------------------------------------------*/
class socket_connection_configuration : public connection_configuration {
public:
    /**
     * @brief connection_configuration - конструктор
     * @param obj_cfg_ - указатель на головной объект конфигурации внешнего соединения с другими процессами
     */
    explicit socket_connection_configuration(const json::object::JsonObject* obj_cfg_);

    /**
     * Деструктор
     */
    ~socket_connection_configuration() = default;

private:
    /**
     * @brief read_configuration - чтение конфигурации отдельного взятого внешнего соединения с другими процессами
     * @param obj_cfg_ - указатель на головной объект конфигурации внешнего соединения с другими процессами
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
    std::string connection_host_ = "";          /// <--- хост внешнего соединения
    uint16_t connection_port_ = 0;              /// <--- порт внешнего соединения
};
}       /// <--- config
}   /// <--- mps

#endif
