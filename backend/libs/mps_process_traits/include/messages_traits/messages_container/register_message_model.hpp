#ifndef SYSTEM_CONTAINER_MESSAGE_MODEL_HPP
#define SYSTEM_CONTAINER_MESSAGE_MODEL_HPP

#include <boost/shared_ptr.hpp>

#include "register_message_concept.hpp"
#include "interfaces_traits/buffer_models/list_buffer_model.hpp"

#define LIST_BUFFER_MODEL_TYPE (std::string)"list_buffer_model"

namespace mps {
namespace process {
namespace container {
namespace messages {
/** ------------------------------------------------------------------------------------------------------------------------
 * @brief The register_message_model class - модель регистрируемого сообщения для хранения в контейнере системы
 -------------------------------------------------------------------------------------------------------------------------*/
template <typename T_message>
struct register_message_model : public register_message_concept {
    using _message_t_ = T_message;
    using _buffer_t_ = boost::shared_ptr<interface::buffer::buffer_concept<_message_t_>>;
    using _message_cfg_t_ = mps::config::message_configuration;

    /**
     * @brief register_message_model - конструктор
     * @param msg_name_ - уникальное имя сообщения
     */
    explicit register_message_model(const std::string& msg_name_) : message_name_(msg_name_) {}

    /**
     * @brief message_name - получение уникального имени сообщения
     * @return уникальное имя сообщения
     */
    inline std::string message_name() const { return this->message_name_; }

    /**
     * @brief component_is_reader - верификация компоненты на чтение данного сообщения
     * @param component_name_ - уникальное имя компоненты
     * @return результат верификации
     */
    inline bool component_is_reader(const std::string& component_name_) {
        return (this->configuration_) ? this->configuration_->component_is_reader(component_name_) : false;
    }

    /**
     * @brief component_is_writer - верификация компоненты на запись данного сообщения
     * @param component_name_ - уникальное наименование компоненты
     * @return результат верификации
     */
    inline bool component_is_writer(const std::string& component_name_) {
        return (this->configuration_) ? this->configuration_->component_is_writer(component_name_) : false;
    }

    /**
     * @brief initialize_buffer - инициализация буфера хранения сообщений
     * @param message_cfg_ - указатель на конфигурацию сообщения
     * @return результат инициализации
     */
    bool initialize_buffer(_message_cfg_t_* message_cfg_) {
        this->configuration_ = message_cfg_;
        if (this->configuration_) {
            // Формируем описание буфера хранения сообщений
            interface::buffer::buffer_description* buffer_description_ = new interface::buffer::buffer_description;
            buffer_description_->message_name_ = message_cfg_->message_name();
            buffer_description_->buffer_name_ = message_cfg_->buffer_name();
            buffer_description_->message_uid_ = message_cfg_->message_uid();
            buffer_description_->buffer_size_ = message_cfg_->buffer_size();
            buffer_description_->readers_count_ = message_cfg_->readers_count();
            buffer_description_->writers_count_ = message_cfg_->writers_count();
            buffer_description_->sign_ |= (((message_cfg_->message_is_unique() & 1) << 1) |
                                          ((message_cfg_->message_registration() & 1) << 2));

            return this->initialize_buffer(*buffer_description_);
        }

        return false;
    }

    /**
     * @brief initialize_buffer - инициализация буфера хранения сообщений данного типа
     * @param dsc_ - указатель на описание буфера хранения сообщений данного типа
     * @return результат инициализации
     */
    bool initialize_buffer(interface::buffer::buffer_description& dsc_) {
        if (dsc_.message_name_ == this->message_name_) {
            if (dsc_.buffer_name_ == LIST_BUFFER_MODEL_TYPE) {
                this->buffer_ = _buffer_t_(new interface::buffer::list_buffer_model<_message_t_>(&dsc_));
            }

            return true;
        }

        return false;
    }

    /**
     * @brief buffer - получение указателя на буфер хранения сообщений
     * @return указатель на буфер хранения сообщений
     */
    inline _buffer_t_ buffer() const { return this->buffer_; }

private:
    std::string message_name_ = "";                 /// <--- уникальное наименование сообщения
    _message_cfg_t_* configuration_ = nullptr;      /// <--- указатель на конфигурацию сообщения
    _buffer_t_ buffer_ = nullptr;                   /// <--- указатель на буфер хранения сообщений
};
}               /// <--- message
}           /// <--- container
}       /// <--- process
}   /// <--- mps

#endif
