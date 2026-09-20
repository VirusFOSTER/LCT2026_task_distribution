#ifndef MPS_LIST_CONTAINER_HPP
#define MPS_LIST_CONTAINER_HPP

#include "tp_list_element.hpp"
#include "memory/list_memory_buffer.hpp"


namespace mps {
namespace container {
/** -----------------------------------------------------------------------------------------------------------------
 * @brief The list class - список системы (mps)
 * В основу данного контейнера заложена идея кольцевого буфера фиксированного размера. При этом каждый элемент списка -
 * это некоторая обертка над указателем на значение. Все элементы добавляются в список в порядке очереди.
 -------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class list_buffer {
public:
    using _value_t_ = T;
    using _element_t_ = tp_list_element<T>;

    /**
     * @brief list_buffer - конструктор
     * @param l_ - выделяемый размер списка (максимальное количество элементов в списке)
     */
    explicit list_buffer(uint32_t l_);

    /**
     * @brief ~list_buffer - деструктор
     */
    virtual ~list_buffer();

    /**
     * @brief append - добавление значения в список
     * @param value_ - указатель на добавляемое значение
     * @return указатель на элемент списка, в который записано значение
     */
    _element_t_* append(_value_t_* value_);

    /**
     * @brief first - получение первого элемента списка
     * @return указатель на первый элемент списка
     */
    inline _element_t_* first() const { return this->first_element_; }

    /**
     * @brief last - получение последнего элемента списка
     * @return указатель на последний элемент списка
     */
    inline _element_t_* last() const { return this->last_element_; }

    /**
     * @brief length - получение максимального количества элементов в списке
     * @return максимальное количество элементов в списке
     */
    inline uint32_t length() const { return this->length_; }

    /**
     * @brief count - получение общего количества записанных в список элементов
     * @return общее количество записанных элементов в список
     */
    inline uint32_t count() const { return this->count_; }

    /**
     * @brief reset - сброс всех значений колцевого буфера
     * @param l_ - новый размер кольцевого буфера
     */
    void reset(uint32_t l_ = 0);

    /**
     * @brief operator [] - получение значения по индексу массива
     * @param idx_ - индекс элемента в массиве
     * @return указатель на значение записанное в ячейку под индексом idx_
     */
    inline _value_t_* operator[](uint32_t idx_) {
        return (idx_ < this->length_) ? this->values_[idx_]->value_ : nullptr;
    }

private:
    /**
     * @brief init - инициализация списка
     */
    void init();

    /**
     * @brief clear - метод полной очистки списка
     */
    void clear();

    /**
     * @brief update - обновление списка при добавлении нового элемента в список
     * По сути обновляются именно первый (при не обходимости) и последний элементы списка
     * @param element_ - новый элемент списка
     */
    void update(_element_t_* element_);

protected:
    list_memory_buffer<_element_t_> values_;    /// <--- массив элементов списка
    _element_t_* first_element_ = nullptr;      /// <--- указатель на первый элемент списка
    _element_t_* last_element_ = nullptr;       /// <--- указатель на последний элемент списка

    uint32_t length_ = 0;                       /// <--- размер списка (максимальное количество элементов)
    uint32_t count_ = 0;                        /// <--- количество добавленных элементов в список
};


//-------------------------------------------------------------------------

template <typename T>
list_buffer<T>::list_buffer(uint32_t l_) : values_(l_), length_(l_) {
    // Инициализируем список
    this->init();
}

//-------------------------------------------------------------------------

template <typename T>
list_buffer<T>::~list_buffer() {
    // Очищаем список от всех элементов
    this->clear();
}

//-------------------------------------------------------------------------

template <typename T>
typename list_buffer<T>::_element_t_* list_buffer<T>::append(_value_t_* value_) {
    //(?) Добавляем элемент в список, если указатель на значение не пустой
    if (value_ && this->length_) {
        auto element_ = this->values_[this->count_ % this->length_];
        if (!element_) {
            return nullptr;
        }

        //(?) Если в элементе списка содержится другое значение, то заменяем последнее на новое (входящее)
        if (element_->value_ != value_) {
            delete element_->value_;
            element_->value_ = value_;
        }

        // Обновляем список (первый и последний элементы)
        this->update(element_);

        // Увеличиваем счетчик записанных в буфер значений
        this->count_++;

        // Возвращаем указатель на элемент списка, в который записано значение
        return element_;
    }

    // В противном случае возвращаем пустой указатель на элемент списка
    return nullptr;
}

//-------------------------------------------------------------------------

template <typename T>
void list_buffer<T>::update(_element_t_* element_) {
    //(?) Если список пуст, инициализируем его
    if (!this->last_element_) {
        this->first_element_ = element_;
        this->last_element_ = this->first_element_;

        return;
    }

    //(?) Если размер буфера не больше 1, ничего не делаем (оперируем ровно с одним элементом)
    if (!(this->length_ > 1)) {
        return;
    }

    //(?) Определяем указатель на первый элемент списка
    this->first_element_ = (element_->next_element_) ?
                               element_->next_element_ : this->first_element_;

    // Определяем указатель на последний элемент списка
    element_->prev_element_ = this->last_element_;
    this->last_element_->next_element_ = element_;
    this->last_element_ = element_;
    this->last_element_->next_element_ = nullptr;
}

//-------------------------------------------------------------------------

template <typename T>
void list_buffer<T>::reset(uint32_t l_) {
    // Выделяем новый блок памяти под элементы
    this->length_ = l_;
    this->values_.reset(this->length_);

    // Сбрасываем количество записанных элементов
    this->count_ = 0;

    this->first_element_ = nullptr;
    this->last_element_ = nullptr;

    // Инициализируем список
    this->init();
}

//-------------------------------------------------------------------------

template <typename T>
void list_buffer<T>::init() {
    //(?>) Заполняем список пустыми элементами (значений нет)
    for (uint32_t i = 0; i < this->length_; ++i) {
        this->values_[i]->value_ = new _value_t_;
    }
}

//-------------------------------------------------------------------------

template <typename T>
void list_buffer<T>::clear() {
    // Очищаем массив элементов списка
    this->values_.reset();

    // Устанавливаем указатель на первый и последний элементы списка как nullptr
    this->first_element_ = nullptr;
    this->last_element_ = nullptr;

    this->length_ = 0;
    this->count_ = 0;
}
}       /// <--- container
}   /// <--- mps

#endif
