#ifndef SYSTEM_PROCESS_MESSAGE_READER_CONCEPT_HPP
#define SYSTEM_PROCESS_MESSAGE_READER_CONCEPT_HPP

#include "messages_traits/message_head.hpp"

namespace mps {
namespace process {
namespace interface {
namespace buffer {
namespace message {
/** --------------------------------------------------------------------------------------------------------------------------
 * @brief The read_message class - концепция хранящегося в буфере сообщения (обертка для чтения сообщения из буфера)
 * Сообщения, хранящиеся в буфере, выдаются в виде этой оболочки.
 * При этом читаемое сообщение не может быть изменено
 ---------------------------------------------------------------------------------------------------------------------------*/
template <typename T_message>
struct read_message {
    using _message_t_ = T_message;
    using _header_t_ = process::message::message_header;

    const _message_t_* const message_ = nullptr;    /// <--- указатель на сообщение, полученное из буфера
    const _header_t_* const header_ = nullptr;      /// <--- указатель на заголовок сообщения

    /**
     * @brief message - конструктор
     * @param msg_ - указатель на возвращаемое из буфера сообщение
     * @param head_ - указатель на заголовок сообщения
     */
    explicit read_message(_message_t_* msg_, _header_t_* head_) : message_(msg_), header_(head_) {}

    /**
     * @brief ~read_message - деструктор
     */
    virtual ~read_message(){}
};
}                   /// <--- message
}               /// <--- interface
}           /// <--- buffer
}       /// <--- process
}   /// <--- mps

#endif
