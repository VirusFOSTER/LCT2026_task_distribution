#ifndef SYSTEM_BUFFER_DESCRIPTION_HPP
#define SYSTEM_BUFFER_DESCRIPTION_HPP

#include <iostream>
#include <cstring>
#include <cstdint>


namespace mps {
namespace process {
namespace interface {
namespace buffer {
/** ------------------------------------------------------------------------------------------------------------------
 * @brief The buffer_description class - описание буфера сообщений
 * Данное описание формируется на базе конфигурации системы (а именно при чтении конфигурации сообщений)
 --------------------------------------------------------------------------------------------------------------------*/
struct buffer_description {
    /**
     * @brief The sign_element enum - сигнатура элементов буфера
     */
    enum sign_element {
        tp_default_         = 0b00000000,   /// <--- сигнатура выставлена по default
        tp_read_all_        = 0b00000001,   /// <--- сообщение должно быть прочитано каждой функциональной компонентой
        tp_registration_    = 0b00000010    /// <--- сообщение регистрируется
    };

    std::string message_name_ = "";             /// <--- уникальное наименование сообщения
    std::string buffer_name_ = "";              /// <--- уникальное наименование буфера хранения сообщения
    uint16_t message_uid_ = 0;                  /// <--- уникальный идентификатор сообщения
    uint16_t buffer_size_ = 0;                  /// <--- размер буфера сообщений
    uint16_t readers_count_ = 0;                /// <--- количество читателей сообщения
    uint16_t writers_count_ = 0;                /// <--- количество писателей сообщения
    uint16_t register_reader_count_ = 0;        /// <--- количество зарегистрированных читателей сообщения
    uint64_t write_messages_count_ = 0;         /// <--- общее количество записанных сообщений в буфер
    uint64_t read_messages_count_ = 0;          /// <--- общее количество прочитанных сообщений из буфера
    uint8_t sign_ = sign_element::tp_default_;  /// <--- сигнатура сообщения

    /**
     * Конструктор
     */
    explicit buffer_description() = default;

    /**
     * @brief buffer_description - конструктор копирования
     * @param dsc_ - описание буфера сообщений
     */
    buffer_description(const buffer_description& dsc_) :
        message_name_(dsc_.message_name_),
        message_uid_(dsc_.message_uid_),
        buffer_size_(dsc_.buffer_size_),
        readers_count_(dsc_.readers_count_),
        writers_count_(dsc_.writers_count_),
        write_messages_count_(dsc_.write_messages_count_),
        read_messages_count_(dsc_.read_messages_count_),
        sign_(dsc_.sign_) {}
};  /// <--- buffer_description
}               /// <--- buffer
}           /// <--- interface
}       /// <--- process
}   /// <--- mps


#endif
