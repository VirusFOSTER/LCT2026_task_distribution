#ifndef SYSTEM_PROCESS_LIST_BUFFER_MODEL_HPP
#define SYSTEM_PROCESS_LIST_BUFFER_MODEL_HPP

#include "list_buffer_element.hpp"
#include "interfaces_traits/buffer_description.hpp"
#include "interfaces_traits/buffer_concept.hpp"

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace mps {
namespace process {
namespace interface {
namespace buffer {
/** -------------------------------------------------------------------------------------------------------------------------
 * @brief The list_buffer_model class - модель буфера хранения сообщений для взаимодействия между компонентами системы.
 * Структура хранения элементов данного буфера представлена в виде двусвязного списка.
 --------------------------------------------------------------------------------------------------------------------------*/
template <typename T_message>
class list_buffer_model : public buffer_concept<T_message> {
public:
    using _message_t_ = T_message;
    using _header_t_ = process::message::message_header;
    using _element_t_ = list_buffer_element<_message_t_>;
    using _rd_message_t_ = message::read_message<_message_t_>;
    using _wr_message_t_ = message::write_message<_message_t_>;
    using _owr_message_t_ = optional_list_element<_message_t_>;

    /**
     * @brief list_buffer_model - конструктор
     * @param dsc_ - описание буфера (задается конфигурацией)
     */
    explicit list_buffer_model(buffer_description* dsc_);

    /**
     * деструктор
     */
    ~list_buffer_model();

    /**
     * @brief register_interface_reader - регистрация интерфейса читателя
     */
    void register_interface_reader();

    /**
     * @brief get_free_element - получение указателя на свободный элемент буфера сообщений
     * @return указатель на свободный элемент буфера сообщений
     */
    _wr_message_t_* get_free_element();

    /**
     * @brief add_new_element - добавление нового элемента в буфер
     * @param ptr_message_ - указатель на новое (добавляемое сообщение)
     * @return результат добавления сообщения
     */
    bool add_new_element(_wr_message_t_* ptr_message_);

    /**
     * @brief read_next_element - получение указателя на следующий элемент сообщения
     * @param ptr_message_ - указатель на последнее прочитанное сообщение
     * @return указатель на сообщение (если такового нет возвращается nullptr)
     */
    _rd_message_t_* read_next_element(_rd_message_t_* ptr_message_);

    /**
     * @brief delete_element - удаление элемента из буфера
     * @param ptr_message_ - указатель на удаляемый элемент буфера
     * @return результат удаления
     */
    bool delete_element(_rd_message_t_ **ptr_message_);

    /**
     * @brief message_name - получение уникального наименования сообщения
     * @return уникальное наименование сообщения
     */
    std::string message_name() const { return this->description_->message_name_; }

    /**
     * @brief message_id - получение уникального уидентификатора сообщения
     * @return уникальный идентификатор сообщения
     */
    uint16_t message_uid() const { return this->description_->message_uid_; }

    /**
     * @brief total_write_messages_count - получение общего количетсва записанных в буфер сообщений
     * @return общее количество записанных в буфер сообщений
     */
    uint64_t total_write_messages_count() const { return this->description_->write_messages_count_; }

    /**
     * @brief total_read_messages_count - получение общего количества прочитанных сообщений из буфера
     * @return общее количество записанных из буфера сообщений
     */
    uint64_t total_read_messages_count() const { return this->description_->read_messages_count_; }

    /**
     * @brief buffer_size - получение размера буфера
     * @return размер буфера
     */
    uint16_t buffer_size() const { return this->description_->buffer_size_; }

    /**
     * @brief readers_count - получение количества читателей сообщений из буфера
     * @return количество читателей сообщений из буфера
     */
    uint16_t readers_count() const { return this->description_->readers_count_; }

    /**
     * @brief writers_count - получение количества писателей сообщений из буфера
     * @return количество писателей сообщений из буфера
     */
    uint16_t writers_count() const { return this->description_->writers_count_; }

private:
    buffer_description* description_ = nullptr;         /// <--- указатель на описание буфера хранения сообщений

    std::vector<_message_t_> messages_array_ = {};      /// <--- массив сообщений заданного типа
    std::vector<_header_t_> headers_array_ = {};        /// <--- массив заголовков сообщений заданного типа
    std::vector<_element_t_> elements_array_ = {};      /// <--- массив элементов буфера

    _element_t_* free_first_element_ = nullptr;    /// <--- указатель на первый свободный для записи элемент буфера
    _element_t_* free_last_element_ = nullptr;     /// <--- указатель на последний свободный для записи элемент буфера

    _element_t_* read_first_element_ = nullptr;         /// <--- указатель на первый доступный для чтения элемент буфера
    _element_t_* read_last_element_ = nullptr;          /// <--- указатель на последний доступный для чтения элемент буфера

    boost::mutex mutex_;                                /// <--- мьютекс доступа к общему ресурсу

    //(!) Полный доступ к параметрам для списка элементов буфера
    friend class list_buffer_element<_message_t_>;
};


//-----------------------------------------------------------------------

template <typename T>
list_buffer_element<T>::list_buffer_element(list_buffer_model<T>* buffer_, uint16_t& index_) :
    message::read_message<T>(&buffer_->messages_array_[index_],&buffer_->headers_array_[index_]),
    optional_list_element<T>(&buffer_->messages_array_[index_],index_),
    header_(&buffer_->headers_array_[index_])
{}

//-----------------------------------------------------------------------

template <typename T>
list_buffer_model<T>::~list_buffer_model() {
    delete this->description_;

    this->elements_array_.clear();
    this->messages_array_.clear();
    this->headers_array_.clear();
}

//-----------------------------------------------------------------------

template <typename T>
list_buffer_model<T>::list_buffer_model(buffer_description* dsc_) : description_(dsc_) {
    //(?) Если конфигурация определена, инициализируем буфер хранения сообщений
    if (this->description_) {
        //(?) Проверяем, что выделяемый размер буфера не равен 0
        if (this->description_->buffer_size_) {
            // Резервируем память под необходимые элементы буфера
            this->messages_array_.reserve(this->description_->buffer_size_);
            this->headers_array_.reserve(this->description_->buffer_size_);
            this->elements_array_.reserve(this->description_->buffer_size_);

            for (uint16_t i = 0; i < this->description_->buffer_size_; ++i) {
                this->messages_array_.emplace_back(_message_t_());
                this->headers_array_.emplace_back(_header_t_(this->description_->message_uid_));
                this->elements_array_.emplace_back(_element_t_(this,i));
            }

            //(?) Если удалось выделить память, инициализируем список
            if (this->elements_array_.size()) {
                this->free_first_element_ = &this->elements_array_[0];
                this->free_last_element_ = this->free_first_element_;

                for (uint16_t i = 1; i < this->description_->buffer_size_; ++i) {
                    _element_t_* current_element_ = &this->elements_array_[i];
                    current_element_->prev_element_ = this->free_last_element_;
                    this->free_last_element_->next_element_ = current_element_;
                    this->free_last_element_ = current_element_;
                }
            }
        }
    }
}

//-----------------------------------------------------------------------

template <typename T>
void list_buffer_model<T>::register_interface_reader() {
    // Увеличиваем счетчик зарегистрированных интерфейсов
    boost::lock_guard<boost::mutex> lock_(this->mutex_);
    this->description_->register_reader_count_++;
}

//-----------------------------------------------------------------------

template <typename T>
typename list_buffer_model<T>::_wr_message_t_* list_buffer_model<T>::get_free_element() {
    _element_t_* free_element_ = nullptr;

    //(?) Если имеются свободные для записи элементы буфера, возвращаем первый по списку из них
    boost::lock_guard<boost::mutex> lock_(this->mutex_);
    if (this->free_first_element_) {
        free_element_ = this->free_first_element_;
        free_element_->readers_count_ = 0;

        this->free_first_element_ = this->free_first_element_->next_element_;

        // обозначаем свободный элемент как зарезервированный
        free_element_->prev_element_ = nullptr;
        free_element_->next_element_ = nullptr;

        if (!this->free_first_element_) {
            this->free_last_element_ = nullptr;
        } else {
            this->free_first_element_->prev_element_ = nullptr;
        }
    }

    return free_element_;
}

//-----------------------------------------------------------------------

template <typename T>
bool list_buffer_model<T>::add_new_element(_wr_message_t_* ptr_message_) {
    //(?) Если передан не пустой указатель,...
    if (ptr_message_) {
        auto opt_element_ = dynamic_cast<optional_list_element<_message_t_>*>(ptr_message_);

        //(?) Получаем элемент списка и проверяем, является ли он зарезервированным.
        // Если да, то добавляем в список для чтения
        if (opt_element_ && opt_element_->array_index_ < this->elements_array_.size()) {
            auto current_element_ = &this->elements_array_[opt_element_->array_index_];
            auto current_header_ = &this->headers_array_[opt_element_->array_index_];
            if (!current_element_->readers_count_ && !current_element_->prev_element_ && !current_element_->next_element_) {
                if (this->read_last_element_) {
                    boost::lock_guard<boost::mutex> lock_(this->mutex_);
                    current_header_->set_message_id(this->description_->write_messages_count_);
                    this->description_->write_messages_count_++;
                    current_element_->readers_count_ = this->description_->register_reader_count_;

                    this->read_last_element_->next_element_ = current_element_;
                } else {
                    boost::lock_guard<boost::mutex> lock_(this->mutex_);
                    current_header_->set_message_id(this->description_->write_messages_count_);
                    this->description_->write_messages_count_++;
                    current_element_->readers_count_ = this->description_->register_reader_count_;

                    this->read_first_element_ = current_element_;
                    this->read_last_element_ = current_element_;
                }

                return true;
            }
        }
    }

    return false;
}

//-----------------------------------------------------------------------

template <typename T>
typename list_buffer_model<T>::_rd_message_t_*
list_buffer_model<T>::read_next_element(_rd_message_t_* ptr_message_) {
    //(?) Если ниодного сообщения прочитано не было, возвращаем указатель первый элемент
    if (!ptr_message_) {
        ptr_message_ = this->read_first_element_;
        return ptr_message_;
    }

    // Получаем указатель на следующий элемент чтения
    auto list_element_ = dynamic_cast<list_buffer_element<_message_t_>*>(ptr_message_);
    if (list_element_) {
        ptr_message_ = list_element_->next_element_;
    }

    return ptr_message_;
}

//-----------------------------------------------------------------------

template <typename T>
bool list_buffer_model<T>::delete_element(_rd_message_t_ **ptr_message_) {
    //(?) Если переданный указатель пустой, то возвращаем отрицательный результат удаления элемента из буфера
    if (!ptr_message_) {
        return false;
    }

    //boost::lock_guard<boost::mutex> lock_(this->mutex_);
    auto list_element_ = dynamic_cast<list_buffer_element<_message_t_>*>(*ptr_message_);

    boost::lock_guard<boost::mutex> lock_(this->mutex_);
    if (list_element_ && list_element_->readers_count_) {
        ptr_message_ = nullptr;
        // Уменьшаем счетчик читателей
        {
            list_element_->readers_count_--;
            this->description_->read_messages_count_++;
        }

        //(?) Возвращаем указатель в список свободных для записи элементов, если все компоненты прочитали
        // данное сообщение
        if (!list_element_->readers_count_) {
            list_element_->readers_count_ = this->description_->read_messages_count_;

            if (list_element_->prev_element_) {
                list_element_->prev_element_->next_element_ = list_element_->next_element_;
            } else {
                this->read_first_element_ = list_element_->next_element_;
            }

            if (list_element_->next_element_) {
                list_element_->next_element_->prev_element_ = list_element_->prev_element_;
            } else {
                this->read_last_element_ = list_element_->prev_element_;
            }

            list_element_->next_element_ = nullptr;
            list_element_->prev_element_ = nullptr;

            if (this->free_last_element_) {
                this->free_last_element_->next_element_ = list_element_;
                list_element_->prev_element_ = this->free_last_element_;
                this->free_last_element_ = list_element_;
            } else {
                this->free_first_element_ = list_element_;
                this->free_last_element_ = this->free_first_element_;
            }
        }

        return true;
    }

    return false;
}
}               /// <--- buffer
}           /// <--- interface
}       /// <--- process
}   /// <--- mps

#endif
