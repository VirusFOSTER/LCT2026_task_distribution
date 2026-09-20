#ifndef SYSTEM_PROCESS_BUFFER_CONCEPT_HPP
#define SYSTEM_PROCESS_BUFFER_CONCEPT_HPP

#include "message_reader_concept.hpp"
#include "message_writer_concept.hpp"

#include <iostream>
#include <cstring>

namespace mps {
namespace process {
namespace interface {
namespace buffer {
/** ---------------------------------------------------------------------------------------------------------------
 * @brief The buffer_concept class - концепция построения буфера хранения сообщений
 * Описывает головные методы, которые должны входить в модель буфера
 -----------------------------------------------------------------------------------------------------------------*/
template <typename T_message>
struct buffer_concept {
    using _message_t_ = T_message;
    using _rd_message_t_ = message::read_message<_message_t_>;
    using _wr_message_t_ = message::write_message<_message_t_>;

    /**
     * @brief buffer_concept - коструктор
     */
    explicit buffer_concept() = default;

    /**
     * @brief ~buffer_concept - деструктор
     */
    virtual ~buffer_concept() {}

    /**
     * @brief register_interface - регистрация интерфейса читателя
     */
    virtual void register_interface_reader() = 0;

    /**
     * @brief get_free_element - получение указателя на свободный элемент буфера сообщений
     * @return указатель на свободный элемент буфера сообщений
     */
    virtual _wr_message_t_* get_free_element() = 0;

    /**
     * @brief add_new_element - добавление нового элемента в буфер
     * @param ptr_message_ - указатель на новое (добавляемое сообщение)
     * @return результат добавления сообщения
     */
    virtual bool add_new_element(_wr_message_t_* ptr_message_) = 0;

    /**
     * @brief read_next_element - получение указателя на следующий элемент сообщения
     * @param ptr_message_ - указатель на последнее прочитанное сообщение
     * @return указатель на сообщение (если такового нет возвращается nullptr)
     */
    virtual _rd_message_t_* read_next_element(_rd_message_t_* ptr_message_) = 0;

    /**
     * @brief delete_element - удаление элемента из буфера
     * @param ptr_message_ - указатель на удаляемый элемент буфера
     * @return результат удаления
     */
    virtual bool delete_element(_rd_message_t_** ptr_message_) = 0;

    /**
     * @brief message_name - получение уникального наименования сообщения
     * @return уникальное наименование сообщения
     */
    virtual std::string message_name() const = 0;

    /**
     * @brief message_uid - получение уникального уидентификатора сообщения
     * @return уникальный идентификатор сообщения
     */
    virtual uint16_t message_uid() const = 0;

    /**
     * @brief total_write_messages_count - получение общего количетсва записанных в буфер сообщений
     * @return общее количество записанных в буфер сообщений
     */
    virtual uint64_t total_write_messages_count() const = 0;

    /**
     * @brief total_read_messages_count - получение общего количества прочитанных сообщений из буфера
     * @return общее количество записанных из буфера сообщений
     */
    virtual uint64_t total_read_messages_count() const = 0;

    /**
     * @brief buffer_size - получение размера буфера
     * @return размер буфера
     */
    virtual uint16_t buffer_size() const = 0;

    /**
     * @brief readers_count - получение количества читателей сообщений из буфера
     * @return количество читателей сообщений из буфера
     */
    virtual uint16_t readers_count() const = 0;

    /**
     * @brief writers_count - получение количества писателей сообщений из буфера
     * @return количество писателей сообщений из буфера
     */
    virtual uint16_t writers_count() const = 0;
};
}               /// <--- buffer
}           /// <--- interface
}       /// <--- process
}   /// <--- mps

#endif
