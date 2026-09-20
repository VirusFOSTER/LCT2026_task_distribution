#ifndef SYSTEM_PROCESS_QBASE_FUNCTIONAL_COMPONENT_HPP
#define SYSTEM_PROCESS_QBASE_FUNCTIONAL_COMPONENT_HPP

#include "components_traits/base_component/base_functional_component.hpp"
#include <QThread>

namespace mps {
namespace process {
namespace component {
namespace base {
/** ----------------------------------------------------------------------------------------------------
 * @brief The base_functional_component class - базовая функциональная компонента
 * По сути от этого класса должны наследоваться все основные компоненты системы
 * Здесь имеется возможность получения указателя на интерфейсы взаимодействия
 * Отличительной чертой данной компоненты является наличие свойств класса QThread, что позволяет
 * использовать данный класс для графический процессов
 ------------------------------------------------------------------------------------------------------*/
class qbase_functional_component :
        public QThread {
    Q_OBJECT

public:
    explicit qbase_functional_component(const std::string fc_name_);
    virtual ~qbase_functional_component() {}

    /**
     * @brief init - инициализация компоненты
     * @return результат инициализации компоненты
     */
    virtual bool init() = 0;

    /**
     * @brief run - головная процедура нити
     */
    virtual void run() = 0;

    /**
     * @brief component_name - получение уникального наименования компоненты
     * @return уникальное наименование компоненты
     */
    inline std::string component_name() const { return this->component_name_; }

    /**
     * @brief component_uid - получение уникального идентификатора компоненты
     * @return уникальный идентификатор компоненты
     */
    inline uint16_t component_uid() const { return this->component_uid_; }

protected:
    /**
     * @brief interface_reader - получение указателя на интерфейс читателя сообщений данного типа
     * @param message_name_ - уникальное наименование сообщения
     * @return указатель на интерфейс читателя сообщений данного типа
     */
    template <typename T_message>
    interface::Isequence_reader<T_message>* interface_reader(const std::string& message_name_) {
        return (register_messages_container_) ?
                   register_messages_container_->interface_reader<T_message>(this->component_name_,message_name_) : nullptr;
    }

    /**
     * @brief interface_writer - получение указателя на интерфейс писателя сообщений данного типа
     * @param message_name_ - уникальное наименование сообщения
     * @return указатель на интерфейс писателя сообщений данного типа
     */
    template <typename T_message>
    interface::Isequence_writer<T_message>* interface_writer(const std::string& message_name_) {
        return (register_messages_container_) ?
                   register_messages_container_->interface_writer<T_message>(this->component_name_,message_name_) : nullptr;
    }

private:
    std::string component_name_ = "";   /// <--- уникальное наименование компоненты
    uint16_t component_uid_ = 0;        /// <--- уникальный идентификатор компонент

    /// (!) Указываем дружественный класс - конфигурация компонент процесса системы
    friend class config::components_configuration;
};
}               /// <--- base
}           /// <--- component
}       /// <--- process
}   /// <--- mps

#endif
