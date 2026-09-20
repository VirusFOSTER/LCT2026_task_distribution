#ifndef SYSTEM_CONTAINER_MESSAGE_CONCEPT_HPP
#define SYSTEM_CONTAINER_MESSAGE_CONCEPT_HPP

#include <iostream>
#include <cstring>

#include "interfaces_traits/buffer_concept.hpp"
#include "interfaces_traits/buffer_description.hpp"
#include "process_configuration/messages_configuration/message_configuration.hpp"

namespace mps {
namespace process {
namespace container {
namespace messages {
/** -----------------------------------------------------------------------------------------------------------------------
 * @brief The register_message_concept class - концепция зарегистрированного в контейнере сообщения
 ------------------------------------------------------------------------------------------------------------------------*/
struct register_message_concept {
    /**
     * @brief register_message_concept - конструктор (по умолчанию)
     */
    explicit register_message_concept() = default;

    /**
     * @brief ~register_message_concept - деструктор
     */
    virtual ~register_message_concept() {}

    /**
     * @brief message_name - получение уникального наименования сообщения
     * @return уникальное наименование сообщения
     */
    virtual inline std::string message_name() const = 0;

    /**
     * @brief component_is_reader - верификация компоненты на чтение данного сообщения
     * @param component_name_ - уникальное имя компоненты
     * @return результат верификации
     */
    virtual inline bool component_is_reader(const std::string& component_name_) = 0;

    /**
     * @brief component_is_writer - верификация компоненты на запись данного сообщения
     * @param component_name_ - уникальное наименование компоненты
     * @return результат верификации
     */
    virtual inline bool component_is_writer(const std::string& component_name_) = 0;

    /**
     * @brief initialize_buffer - инициализация буфера хранения сообщений
     * @param message_cfg_ - указатель на конфигурацию сообщения
     * @return результат инициализации
     */
    virtual bool initialize_buffer(mps::config::message_configuration* message_cfg_) = 0;

    /**
     * @brief initialize_buffer - инициализация буфера хранения сообщений
     * @param dsc_ - описание буфера хранения сообщений
     * @return результат инициализации
     */
    virtual bool initialize_buffer(interface::buffer::buffer_description& dsc_) = 0;
};
}               /// <--- message
}           /// <--- container
}       /// <--- process
}   /// <--- mps

#endif
