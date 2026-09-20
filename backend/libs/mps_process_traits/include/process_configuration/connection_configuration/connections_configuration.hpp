#ifndef SYSTEM_PROCESS_OUT_CONNECTION_CONFIGURATION_HPP
#define SYSTEM_PROCESS_OUT_CONNECTION_CONFIGURATION_HPP

#include "process_configuration/sglobal_config.hpp"
#include "connection_configuration.hpp"


namespace mps {
namespace config {
/** -------------------------------------------------------------------------------------------------------------------------
 * @brief The connections_configuration class - конфигурация установления соединения между различными процессами
 * Соединение между процессами может быть абсолютно разным:
 *  - соединение через сокеты;
 *  - соединение через файлы;
 *  - соединение через общую выделенную память;
 *  и т.д.
 --------------------------------------------------------------------------------------------------------------------------*/
class connections_configuration {
    SCONFIG_GLOBAL

public:
    /**
     * @brief connection_configuration - конструктор
     * @param type_cfg_ - тип конфигурации (ожидаемый)
     * @param path_cfg_ - полный путь к файлу конфигурации
     */
    explicit connections_configuration(const std::string& type_cfg_, const std::string& path_cfg_);

    /**
     * Деструктор
     */
    ~connections_configuration() = default;

    /**
     * @brief size - получение размера массива внешних соединений
     * @return размер массива внешних соединений
     */
    inline uint16_t size() const { return this->connections_.size(); }

    /**
     * @brief operator [] - получение указателя на конфигурацию внешнего соединения
     * @param i - индекс соединения в массиве
     * @return указатель на конфигурацию соединения
     */
    inline connection_configuration* operator[](uint16_t i) {
        return (i < this->connections_.size()) ? this->connections_[i] : nullptr;
    }

private:
    /**
     * @brief read_configuration - чтение конфигурации внешних соединений с другими процессами
     * @param obj_cfg_ - указатель на головной объект конфигурации
     * @return результат чтения конфигурации внешних соединений с другими процессами
     */
    bool read_configuration(const json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация конфигурации внешний соединений на валидность
     * @param obj_cfg_ - указатель на головной объект конфигурации
     * @return реультат верификации конфигурации
     */
    bool configuration_valid(const json::object::JsonObject* obj_cfg_);

    /**
     * @brief read_connection - чтение конфигурации отдельно взятого внешнего соединения процесса
     * @param obj_cfg_ - указатель на конфигурацию отдельно взятого внешнего соединения процесса
     * @return результат чтения конфигурации (0x00 - успешно, 0x01 - тип соединения не определен, 0x02 - ошибка чтения
     * конфигурации)
     */
    uint8_t read_connection(const json::object::JsonObject* obj_cfg_);

private:
    std::vector<connection_configuration*> connections_ = {};    /// <--- массив внешних соединений
};
}       /// <--- config
}   /// <--- mps

#endif
