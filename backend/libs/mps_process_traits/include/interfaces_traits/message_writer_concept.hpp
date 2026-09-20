#ifndef SYSTEM_PROCESS_MESSAGE_WRITER_CONCEPT_HPP
#define SYSTEM_PROCESS_MESSAGE_WRITER_CONCEPT_HPP

#include <cstdint>

namespace mps {
namespace process {
namespace interface {
namespace buffer {
namespace message {
/** --------------------------------------------------------------------------------------------------------------------------
 * @brief The write_message class - концепция хранящегося в буфере сообщения (обертка для записи сообщения в буфер)
 * Именно в виде этой обертки будет выдан указатель на свободный элемент буфера хранения сообщений
 ---------------------------------------------------------------------------------------------------------------------------*/
template <typename T_message>
struct write_message {
    using _message_t_ = T_message;

    _message_t_* element_ = nullptr;        /// <--- указатель на добавляемое в буфер сообщение

protected:
    /**
     * @brief write_message - конструктор
     * @param msg_ - указатель на сообщение в буфере
     * @param idx_ - индекс сообщения в буфере (массиве)
     */
    explicit write_message(_message_t_* msg_, uint16_t idx_ = 0) : element_(msg_) {}

    /**
     * деструктор
     */
    virtual ~write_message() {}
};
}                   /// <--- message
}               /// <--- buffer
}           /// <--- interface
}       /// <--- process
}   /// <--- mps

#endif
