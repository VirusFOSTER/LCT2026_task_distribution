#ifndef SYSTEM_PROCESS_CONNECTION_CONFIGURATION_HPP
#define SYSTEM_PROCESS_CONNECTION_CONFIGURATION_HPP

#include <mps/mps_common/utils/json_io/json.hpp>
#include <set>

namespace mps {
namespace config {
/** ---------------------------------------------------------------------------------------------------------------------------
 * @brief The connection_configuration class - общая структура конфигурации внешнего соединения процессов
 ---------------------------------------------------------------------------------------------------------------------------*/
struct connection_configuration {
    /** ---------------------------------------------------------------------------------------------
     * @brief The _tg_conn_ enum - метки возможных типов внешних соединений
     ------------------------------------------------------------------------------------------------*/
    enum class _tg_conn_ {
        _tg_conn_unknown_ = 0,          /// <--- тип соединения неизвестен
        _tg_conn_managed_memory_ = 1,   /// <--- тип соединения - общий сегмент памяти
        _tg_conn_tcpip_ = 2             /// <--- тип соединения tcp/ip
    };

    /**
     * @brief connection_configuration - конструктор
     */
    explicit connection_configuration() = default;

    /**
     * @brief ~connection_configuration - деструктор
     */
    virtual ~connection_configuration() {}

    /**
     * @brief connection_uid - получение уникального идентификатор внешнего соединения
     * @return уникальный идентификатор внешнего соединения
     */
    inline uint16_t connection_uid() const { return this->connection_uid_; }

    /**
     * @brief connection_priority - получение приоритета внешнего соединения
     * @return приоритет внешнего соединения
     */
    inline uint16_t connection_priority() const { return this->connection_priority_; }

    /**
     * @brief connection_type - получение типа соединения
     * @return тип соединения
     */
    inline _tg_conn_ connection_type() const { return this->connection_type_; }

    /**
     * @brief connection_name - получение уникального наименования соединения
     * @return уникальное наименование соединения
     */
    inline std::string connection_name() const { return this->connection_name_; }

    /**
     * @brief buffer_size - получение размера передаваемых через соединение данных
     * @return размер передаваемых через соединениек данных
     */
    inline uint32_t buffer_size() const { return this->buffer_size_; }

    /**
     * @brief configuration_read - получение признака чтения конфигурации внешнего соединения процесса
     * @return признак чтения конфигурации внешнего соединения процесса
     */
    inline bool configuration_read() const { return this->configuration_read_; }

protected:
    std::map<std::string,std::set<std::string>> access_ = {};   /// <--- массив процессов/компонент с доступом к соединению
    std::string connection_name_ = "";                          /// <--- уникальное имя соединения
    _tg_conn_ connection_type_ = _tg_conn_::_tg_conn_unknown_;  /// <--- тип внешнего соединения
    uint16_t connection_uid_ = 0;                               /// <--- уникальный идентификатор соединения
    uint16_t connection_priority_ = 0;                          /// <--- приоритет соединения
    uint32_t buffer_size_ = 0;                                  /// <--- размер буфера для передачи данных

    bool configuration_read_ = false;                   /// <--- признак чтения конфигурации внешнего соединения
};
}       /// <--- config
}   /// <--- mps

#endif
