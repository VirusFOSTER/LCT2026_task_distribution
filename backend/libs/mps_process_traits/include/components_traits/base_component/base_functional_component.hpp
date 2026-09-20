#ifndef SYSTEM_BASE_FUNCTIONAL_COMPONENT_HPP
#define SYSTEM_BASE_FUNCTIONAL_COMPONENT_HPP

#include "messages_traits/messages_container/register_messages_container.hpp"
#include <boost/shared_ptr.hpp>

#include <thread>
#include <chrono>

namespace mps {
namespace process {
namespace config {
/**
 * Предварительное объявление класса конфигурации компонент
 */
class components_configuration;
}   /// <--- config

namespace component {
namespace base {
/** ----------------------------------------------------------------------------------------------------
 * @brief The base_functional_component class - базовая функциональная компонента
 * По сути от этого класса должны наследоваться все основные компоненты системы
 * Здесь имеется возможность получения указателя на интерфейсы взаимодействия
 ------------------------------------------------------------------------------------------------------*/
class base_functional_component {
public:
    /** Конструктор */
    explicit base_functional_component(const std::string fc_name_);

    /** Деструктор */
    virtual ~base_functional_component() {}

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

protected:
    uint16_t component_uid_ = 0;                    /// <--- уникальный идентификатор компоненты
    std::string component_name_ = "";               /// <--- уникальное имя компоненты

    /// (!) Указываем дружественный класс - конфигурация компонент процесса системы
    friend class config::components_configuration;
};
}               /// <--- sys
}           /// <--- component
}       /// <--- process
}   /// <--- mps

#endif
