#ifndef SYSTEM_PROCESS_REGISTER_MESSAGES_CONTAINER_HPP
#define SYSTEM_PROCESS_REGISTER_MESSAGES_CONTAINER_HPP

#include "register_message_model.hpp"
#include "interfaces_traits/sequence_reader.hpp"
#include "interfaces_traits/sequence_writer.hpp"

#include <cstdint>
#include <boost/shared_ptr.hpp>

namespace mps {
namespace process {
namespace container {
namespace messages {
/** -----------------------------------------------------------------------------------------------------------------------
 * @brief The register_messages_container class - класс для регистрации сообщений, пересылаемых между компонентами
 * процессов системы
 * Данный класс используется для установления соединения между компонентами процесса системы. При этом:
 *  1) для каждого сообщения формируется буфер хранения сообщений в соответствии с конфигурацией процесса системы
 *  2) для каждой компоненты формируется интерфейс для чтения или записи сообщений в буфер
 ------------------------------------------------------------------------------------------------------------------------*/
class register_messages_container {
    /** ----------------------------------------------------------------------------
     * @brief The messages_list class - список зарегистрированных сообщений
     -----------------------------------------------------------------------------*/
    struct el_messages_list {
        using _reg_message_t_ = boost::shared_ptr<register_message_concept>;

        _reg_message_t_ register_message_ = nullptr;        /// <--- зарегистрированное сообщение

        el_messages_list* next_element_ = nullptr;          /// <--- указатель на следующий элемент списка
        el_messages_list* prev_element_ = nullptr;          /// <--- указатель на предыдущий элемент списка

        /**
         * @brief el_messages_list - конструктор
         * @param msg_name_ - уникальное имя сообщения
         */
        template <typename T_message>
        explicit el_messages_list(register_message_model<T_message> *model_) :
            register_message_(model_){}
    };

public:
    /**
     * @brief register_messages_container - конструктор
     */
    explicit register_messages_container() = default;

    /**
     * деструктор
     */
    ~register_messages_container() = default;

    /**
     * @brief register_message - регистрация очередного типа сообщения
     * @return
     */
    template <typename T_message>
    bool register_message(const std::string& message_name_) {
        //(?) Если сообщение с указанным именем не было найдено в списке зарегистрированных, регистрируем новое сообщение
        // в списке
        if (!this->find_message(message_name_)) {
            register_message_model<T_message>* reg_msg_model_ = new register_message_model<T_message>(message_name_);
            el_messages_list* new_message_ = new el_messages_list(reg_msg_model_);
            if (this->last_message_) {
                this->last_message_->next_element_ = new_message_;
                new_message_->prev_element_ = this->last_message_;
                this->last_message_ = new_message_;
            } else {
                this->first_message_ = new_message_;
                this->last_message_ = this->first_message_;
            }

            return true;
        }

        // В противном случае возвращаем отрицательный ответ регистрации сообщения
        return false;
    }

    /**
     * @brief find_message - поиск сообщения по уникальному имени
     * @param message_name_ - уникальное имя сообщения
     * @return указатель на элемент списка сообщений
     */
    el_messages_list* find_message(const std::string& message_name_);

    /**
     * @brief interface_reader - получение указателя на интерфейс читателя сообщений заданного типа
     * @param component_name_ - уникальное наименование компоненты
     * @param message_name_ - уникальное наименование сообщения
     * @return указатель на интерфейс читателя сообщений заданного типа
     */
    template <typename T_message>
    interface::Isequence_reader<T_message>* interface_reader(const std::string& component_name_,
                                                             const std::string& message_name_) {
        boost::lock_guard<boost::mutex> lock_(this->mutex_);

        //(?>) Ищем в списке сообщение с указанным типом.
        // Если такое сообщение существует,...
        auto current_message_ = this->find_message(message_name_);
        if (current_message_) {
            //(?) Проверяем, что компонента является читателем сообщения (согласно конфигурации)
            if (current_message_->register_message_->component_is_reader(component_name_)) {
                //(?) Проверяем правильность типа указанного сообщения
                auto message_model_ = dynamic_cast<register_message_model<T_message>*>(current_message_->register_message_.get());
                if (message_model_) {
                    return new interface::Isequence_reader<T_message>(message_model_->buffer().get());
                }
            }
        }

        return nullptr;
    }

    /**
     * @brief interface_writer - получение указателя на интерфейс писателя сообщений заданного типа
     * @param component_name_ - уникальное наименование компоненты
     * @param message_name_ - уникальное наименование сообщения
     * @return указатель на интерфейс писателя сообщений заданного типа
     */
    template <typename T_message>
    interface::Isequence_writer<T_message>* interface_writer(const std::string& component_name_,
                                                             const std::string& message_name_) {
        boost::lock_guard<boost::mutex> lock_(this->mutex_);

        //(?>) Ищем в списке сообщение с указанным типом.
        // Если такое сообщение существует,...
        auto current_message_ = this->find_message(message_name_);
        if (current_message_) {
            //(?) Проверяем, что компонента является писателем сообщения (согласно конфигурации)
            if (current_message_->register_message_->component_is_writer(component_name_)) {
                //(?) Проверяем правильность типа указанного сообщения
                auto message_model_ = dynamic_cast<register_message_model<T_message>*>(current_message_->register_message_.get());
                if (message_model_) {
                    return new interface::Isequence_writer<T_message>(message_model_->buffer().get());
                }
            }
        }

        return nullptr;
    }

private:
    el_messages_list* first_message_ = nullptr;         /// <--- указатель на первое зарегистрированное сообщение
    el_messages_list* last_message_ = nullptr;          /// <--- указатель на последнее зарегистрированное сообщение

    boost::mutex mutex_;                                /// <--- мьютекс общего доступа к ресурсам контейнера

    uint16_t register_messages_count_ = 0;              /// <--- количество зарегистрированных сообщений процесса системы
};
}               /// <--- message
}           /// <--- container
}       /// <--- process
}   /// <--- mps


extern boost::shared_ptr<mps::process::container::messages::register_messages_container>
    register_messages_container_;

#endif
