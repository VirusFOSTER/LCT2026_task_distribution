#ifndef SYSTEM_PROCESS_MESSAGE_CONFIGURATION_HPP
#define SYSTEM_PROCESS_MESSAGE_CONFIGURATION_HPP

#include <mps/mps_common/utils/json_io/json.hpp>

namespace mps {
namespace config {
/** -------------------------------------------------------------------------------------------------------------------------
 * @brief The message_configuration class - конфигурация сообщения, фигурирующего в процесссе системы между компонентами
 ---------------------------------------------------------------------------------------------------------------------------*/
class message_configuration {
public:
    /**
     * @brief The message_tag enum - метки сообщения
     *  - тип доступа;
     *  - тип чтения.
     */
    enum message_tag {
        _tg_default_            = 0b00000000,   /// <--- метка по умолчанию
        _tg_mutable_message_    = 0b00000001,   /// <--- признак отсутствия доступа к изменению данных в сообщении
        _tg_unique_message_     = 0b00000010    /// <--- признак уникальности сообщения (читается ровно 1 компонентой)
    };

    /**
     * @brief message_configuration - конструктор
     * @param obj_cfg_ - указатель на головной объект конфигурации сообщения процесса системы
     */
    explicit message_configuration(const json::object::JsonObject* obj_cfg_);

    /**
     * Деструктор
     */
    ~message_configuration() = default;

    /**
     * @brief configuration_read - получение признака чтения конфигурации сообщения процесса системы
     * @return результат чтения конфигурации сообщения процесса системы
     */
    inline bool configuration_read() const { return this->configuration_read_; }

    /**
     * @brief message_registration - получение признака регистрации сообщения данного типа
     * @return признак регистрации сообщения данного типа
     */
    inline bool message_registration() const { return this->message_registration_; }

    /**
     * @brief message_uid - получение уникального идентификатора сообщения
     * @return уникальный идентификатор сообщения
     */
    inline int16_t message_uid() const { return this->message_uid_; }

    /**
     * @brief buffer_size - получение размера буфера хранения сообщения
     * @return размер буфера хранения сообщения
     */
    inline uint16_t buffer_size() const { return this->buffer_size_; }

    /**
     * @brief message_name - получение уникального наименование сообщения
     * @return уникальное наименование сообщения
     */
    inline std::string message_name() const { return this->message_name_; }

    /**
     * @brief alt_message_name - получение альтернативного наименования сообщения процесса системы
     * @return альтернативное наименование сообщения процесса системы
     */
    inline std::string alt_message_name() const { return this->alt_message_name_; }

    /**
     * @brief buffer_name - получение уникального наименования буфера хранения сообщения данного типа
     * @return уникальное наименование буфера хранения сообщения данного типа
     */
    inline std::string buffer_name() const { return this->buffer_name_; }

    /**
     * @brief readers_count - получение количества читателей сообщения процесса системы
     * @return количество читателей сообщения процесса системы
     */
    inline uint16_t readers_count() const { return this->targets_.size(); }

    /**
     * @brief writers_count - получение количества читателей сообщения процесса системы
     * @return количество читателей сообщения процесса системы
     */
    inline uint16_t writers_count() const { return this->sources_.size(); }

    /**
     * @brief component_is_writer - проверка, является ли компонента с указанным именем писателем сообщения данного типа
     * @param component_name_ - уникальное наименование компоненты
     * @return результат проверки
     */
    bool component_is_writer(const std::string& component_name_);

    /**
     * @brief component_is_reader - проверка, является ли компонента с указанным именем читателем сообщения данного типа
     * @param component_name_ - уникальное наименование компоненты
     * @return результат проверки
     */
    bool component_is_reader(const std::string& component_name_);

    /**
     * @brief message_is_unique - получение признака уникальности сообщения
     * Признак показывает, как буфер распоряжается последовательностью сообщений. Если флаг признака поднят, то
     * сообщение будет прочитано один раз одной компонентой несмотря на то, что читателей может быть несколько
     * @return признак уникальности сообщения
     */
    inline bool message_is_unique() const { return this->message_tag_ & message_tag::_tg_unique_message_; }

    /**
     * @brief message_is_mutable - признак доступа к данным сообщения
     * Данный признак показывает, может ли сообщение быть изменено другой компонентой
     * @return признак доступа к данным сообщения
     */
    inline bool message_is_mutable() const { return this->message_tag_ & message_tag::_tg_mutable_message_; }

private:
    /**
     * @brief read_configuration - чтение конифгурации сообщения процесса системы
     * @param obj_cfg_ - указатель на головной объект конфигурации сообщения процесса системы
     * @return результат чтения конфигурации сообщения процесса системы
     */
    bool read_configuration(const json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация конфигурации собщения процесса системы на валидность
     * @param obj_cfg_ - указатель на головной объект конфигурации сообщения процесса системы
     * @return результат верификации конфигурации сообщения процесса системы на валидность
     */
    bool configuration_valid(const json::object::JsonObject* obj_cfg_);

    /**
     * @brief unified_components - унификация компонент, взаимодействующих данным сообщением (удаление дубликатов)
     * @param components_ - фильтруемый массив
     */
    void unified_components(std::vector<std::string>& components_);

    /**
     * @brief define_acces_tag - определение метки доступа к данным сообщения
     * @param tg_ - метка доступа к данным, указанная в конфигурации
     * @return результат определения метки доступа к данным
     */
    bool define_acces_tag(const std::string& tg_);

    /**
     * @brief define_unique_tag - определение метки уникальности сообщения
     * @param tg_ - метка уникальности сообщения, указанная в конфигурации
     * @return результат определения метки уникальности сообщения
     */
    bool define_unique_tag(const std::string& tg_);

private:
    bool configuration_read_ = false;       /// <--- признак чтения конфигурации сообщения процесса системы
    bool message_registration_ = false;     /// <--- признак регистрации сообщения процесса системы
    int16_t message_uid_ = -1;              /// <--- уникальный идентификатор сообщения процесса системы
    uint16_t buffer_size_ = 0;              /// <--- размер буфера хранения сообщения данного типа (максимальное количество)
    std::string message_name_ = "";         /// <--- уникальное наименование сообщения процесса системы
    std::string alt_message_name_ = "";     /// <--- альтеранативное наименование сообщения процесса системы
    std::string buffer_name_ = "";          /// <--- наименование буфера хранения сообщений данного типа
    std::vector<std::string> sources_ = {}; /// <--- уникальные имена писателей данного типа сообщения
    std::vector<std::string> targets_ = {}; /// <--- уникальные именна читателей данного типа сообщения
    uint8_t message_tag_ = message_tag::_tg_default_;   /// <--- набор меток сообщения процесса системы
};
}       /// <--- config
}   /// <--- mps

#endif
