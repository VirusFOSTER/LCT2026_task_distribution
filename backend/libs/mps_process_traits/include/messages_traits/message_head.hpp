#ifndef SYSTEM_PROCESS_MESSAGE_HEADER_HPP
#define SYSTEM_PROCESS_MESSAGE_HEADER_HPP

#include <cstdint>

namespace mps {
namespace process {
namespace message {
/** --------------------------------------------------------------------------------------------------------------------------
 * @brief The message_header class - заголовок сообщения
 * Каждое сообщение в буфере в обязательном порядке должно содержать в себе заголовок с дополнительной информацией
 * Сам заголовок формируется для сообщения в момент записи в буфер и при чтении не должен быть изменен
 --------------------------------------------------------------------------------------------------------------------------- */
class message_header {
public:
    /**
     * @brief message_header - конструктор
     */
    explicit message_header(uint16_t& uid_) : message_uid_(uid_) {}

    /**
     * деструктор
     */
    ~message_header() = default;

    /**
     * @brief set_message_id - установление порядкового номер сообщения
     * @param msg_id_ - порядковый номер сообщения
     */
    inline void set_message_id(uint64_t& msg_id_) { this->message_id_ = msg_id_; }

    /**
     * @brief set_message_source - установление источника сообщения (идентификатор компоненты, добавляющей сообщение в буфер)
     * @param msg_source_ - уникальный идентификатор компоненты, записывающей сообщение в буфер
     */
    inline void set_message_source(uint16_t& msg_source_) { this->message_source_ = msg_source_; }

    /**
     * @brief set_message_time - установление времени формирования сообщения
     * @param t_ - время, когда сообщение было добавлено в буфер
     */
    inline void set_message_time(uint64_t& t_) { this->message_time_ = t_; }

    /**
     * @brief message_id - получение порядкового номера сообщения
     * @return порядковый номер сообщения
     */
    inline uint64_t message_id() const { return this->message_id_; }

    /**
     * @brief message_time - получение времени формирования сообщения
     * @return время формирования сообщения
     */
    inline uint64_t message_time() const { return this->message_time_; }

    /**
     * @brief message_uid - получение уникального идентификатора сообщения
     * @return уникальный идентификатор сообщения
     */
    inline uint16_t message_uid() const { return this->message_uid_; }

    /**
     * @brief message_source - получение источника сообщения
     * @return источник сообщения
     */
    inline uint16_t message_source() const { return this->message_source_; }

private:
    uint64_t message_time_ = 0;     /// <--- время записи сообщения (в нс)
    uint64_t message_id_ = 0;       /// <--- идентификатор сообщения (порядковый номер)
    uint16_t message_uid_ = 0;      /// <--- уникальный идентификатор сообщения
    uint16_t message_source_ = 0;   /// <--- источник сообщения (уникальный идентификатор компоненты)
};
}           /// <--- message
}       /// <--- process
}   /// <--- mps

#endif
