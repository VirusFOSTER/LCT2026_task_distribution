#ifndef SYSTEM_PROCESS_CONFIGURATION_HPP
#define SYSTEM_PROCESS_CONFIGURATION_HPP

#include "components_configuration/components_configuration.hpp"
#include "messages_configuration/messages_configuration.hpp"
#include "connection_configuration/connections_configuration.hpp"

#include <boost/shared_ptr.hpp>

namespace mps {
namespace config {
/** --------------------------------------------------------------------------------------------------------------------------
 * @brief The process_configuration class - конфигурация процесса системы
 * В основе конфигруации каждого процесса должна быть в обязательном порядке заложена информация о:
 *  - компонентах процесса системы (каждая компонента будет выполняться в своем потоке);
 *  - сообщениях, фигурирующих внутри процесса и передаваемых между потоками;
 *  - внешнем соединении (для установления связи между процессами по сокету или общей памяти);
 *  - другой конфигурации, которая может быть использована при выполнении задач, возлагаемых на процесс
 *  (не обязательно должна присутствовать)
 ---------------------------------------------------------------------------------------------------------------------------*/
class process_configuration {
    SCONFIG_GLOBAL

    using _components_cfg_t_ = boost::shared_ptr<components_configuration>;
    using _messages_cfg_t_ = boost::shared_ptr<messages_configuration>;
    using _connections_cfg_t_ = boost::shared_ptr<connections_configuration>;


public:
        /**
         * @brief process_configuration - конструктор
         * @param proc_name_ - уникальное имя процесса
         * @param path_cfg_ - полный путь к файлу конфигурации
         */
    explicit process_configuration(const std::string& proc_name_, const std::string& path_cfg_);

    /**
     * Деструктор
     */
    ~process_configuration() = default;

    /**
     * @brief components_config - получение конфигурации компонент процесса системы
     * @return конфигурация компонент процесса системы
     */
    inline _components_cfg_t_ components_config() const { return this->components_configuration_; }

    /**
     * @brief process_name - получение уникального наименования процесса системы
     * @return уникальное наименование процесса системы
     */
    inline std::string process_name() const { return this->process_name_; }

    /**
     * @brief process_uid - получение уникального идентификатора процесса системы
     * @return уникальный идентификатор процесса системы
     */
    inline uint32_t process_uid() const { return this->process_uid_; }

    /**
     * @brief other_configuration - получение полного пути к локальной конфигурации процесса системы.
     * Данная конфигурация предназначена уже больше для компонент системы, нежели для процесса в целом.
     * Явным примером конфигурации служит массив настраиваемых параметров того или иного алгоритма, используемого
     * в процессе работы компоненты или процесса в целом.
     * Чтение этой конфигурации возлагается исключительно на компоненты, которые используют данную конфигурацию
     * @return полный путь к файлу конфигурации
     */
    inline std::string local_configuration() const { return this->local_configuration_; }

private:
    /**
     * @brief read_configuration - чтение конфигурации процесса системы
     * @param obj_cfg_ - указатель на головной объект конфигурации процесса системы
     * @return результат чтения конфигурации процесса системы
     */
    bool read_configuration(const json::object::JsonObject* obj_cfg_);

    /**
     * @brief configuration_valid - верификация конфигурации процесса системы на валидность
     * @param obj_cfg_ - указатель на головной объект конфигурации процесса системы
     * @return результат верификации конфигурации процесса системы на валидность
     */
    bool configuration_valid(const json::object::JsonObject* obj_cfg_);

    /**
     * @brief local_configuration_valid - верификация локальной конфигурации процесса системы на валидность
     * К локальной конфигурации процесса системы относятся:
     *  - конфигурация компонент процесса системы;
     *  - конфигурация сообщений, фигурирующих между компонентами процесса системы;
     *  - конфигурация внешнего соединения, устанавливаемого процессом для взаимодействия с другими процессами системы.
     * Все эти типы конфигурации в составе общей конфигураии имеют одну структуру
     * @param obj_cfg_ - указатель на локальную конфигурацию процесса системы
     * @return результат верификации на валидность
     */
    bool local_configuration_valid(const json::object::JsonObject* obj_cfg_);

private:
    /**
     * @brief read_local_configuration - метод чтения локальной конфигурации процесса системы
     * @param ptr_config_ - указатель (shared_ptr) на читаемую локальную конфигурацию процесса системы
     * @param obj_cfg_ - указатель на объект локальной конфигурации процесса системы
     * @return результат чтения конфигурации
     */
    template <typename T_config>
    bool read_local_configuration(T_config& ptr_config_, const json::object::JsonObject* obj_cfg_) {
        using cfg_type_t_ = typename T_config::element_type;

        // Определяем ожидаемый тип конфигурации и полный путь к файлу конфигурации
        std::string type_cfg_ = obj_cfg_->asString("type");
        std::string path_cfg_ = obj_cfg_->asString("path");

        path_cfg_ = this->make_full_path(path_cfg_);

        //(?) Сбрасываем конфигурацию, если таковая имеется
        if (ptr_config_) {
            ptr_config_.reset();
        }

        // Считываем конфигурацию
        ptr_config_ = boost::shared_ptr<cfg_type_t_>(new cfg_type_t_(type_cfg_,path_cfg_));
        if (!ptr_config_->configuration_read()) {
            ptr_config_.reset();
            return false;
        }

        return true;
    }

protected:
    std::string process_name_ = "";                             /// <--- наименование процесса системы
    uint32_t process_uid_ = 0;                                  /// <--- уникальный идентификатор процесса системы

    _components_cfg_t_ components_configuration_ = nullptr;     /// <--- конфигурация компонента процесса системы
    _messages_cfg_t_ messages_configuration_ = nullptr;         /// <--- конфигурация сообщений процесса системы
    _connections_cfg_t_ connections_configuration_ = nullptr;   /// <--- конфигурация внешних соединений процесса системы

    std::string local_configuration_ = "";                      /// <--- локальная конфигурация процесса системы
};
}       /// <--- config
}   /// <--- mps

#endif
