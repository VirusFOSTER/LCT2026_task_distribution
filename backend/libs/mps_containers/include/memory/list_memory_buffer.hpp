#ifndef MPS_LIST_MEMORY_BUFFER_HPP
#define MPS_LIST_MEMORY_BUFFER_HPP

#include "tp_list_element.hpp"
#include <iostream>

namespace mps {
namespace container {
template <typename T>
class list_memory_buffer {
public:
    using _value_t_ = T;
    using _element_t_ = tp_list_memory_element<T>;

    /**
     * @brief list_memory_buffer - конструктор
     * @param l_ - размер буфера хранения элементов
     */
    explicit list_memory_buffer(uint32_t l_);

    /**
     * деструктор
     */
    ~list_memory_buffer();

    /**
     * @brief get_element - получение указателя на свободный элемент буфера
     * @return указатель на свободный элемент буфера
     */
    _element_t_* get_element();

    /**
     * @brief remove_element - удаление элемента из числа используемых (освобождение элемента по указателю на элемент)
     * По сути это метод возвращения используемого элемента в спсиок свободных для записи элементов
     * @param element_ - указатель на указатель на элемент буфера
     */
    void remove_element(_element_t_** element_);

    /**
     * @brief reset - переинициализация буфера хранения элементов (выделение блока памяти нового размера)
     * @param l_ - новый размер буфера хранения элементов
     */
    void reset(uint32_t l_ = 0);

    /**
     * @brief length - получение размера буфера
     * @return размер буфера элементов
     */
    inline uint32_t length() const { return this->length_; }

    /**
     * @brief operator [] - получение указателя на значениее элемента по индексу
     * @param idx_ - индекс запрашиваемого элемента
     * @return указатель на элемент
     */
    inline _value_t_* operator[](uint32_t idx_) {
        return (idx_ < this->length_) ? &this->elements_[idx_].value_ : nullptr;
    }

private:
    /**
     * @brief init - инициализация буфера хранения элементов
     */
    void init();

    /**
     * @brief clear - метод полной очистки выделенной памяти
     */
    void clear();

private:
    uint32_t length_ = 0;                       /// <--- количество выделенной памяти
    _element_t_* elements_ = nullptr;           /// <--- массив элементов буфера
    _element_t_* first_element_ = nullptr;      /// <--- первый элемент списка
    _element_t_* last_element_ = nullptr;       /// <--- последний элемент списка
};


//-----------------------------------------------------------------------------------------------------

template <typename T>
list_memory_buffer<T>::list_memory_buffer(uint32_t l_) : length_(l_) {
    // Инициализируем массив элементов и двусвязный список
    this->init();
}

//-----------------------------------------------------------------------------------------------------

template <typename T>
list_memory_buffer<T>::~list_memory_buffer() {
    //(?) Если память была выделена, освобождаем ее
    if (this->length_) {
        this->clear();
    }
}

//-----------------------------------------------------------------------------------------------------

template <typename T>
typename list_memory_buffer<T>::_element_t_* list_memory_buffer<T>::get_element() {
    //(?) Если в буфере имеются свободные элементы (свободные ячейки памяти для записи),...
    if (this->first_element_) {
        //... получаем указатель на очередной элемент
        auto element_ = this->first_element_;

        //(?) Переопределяем соответствующие указатели, если имеются еще свободные элементы
        if (this->first_element_->next_element_) {
            this->first_element_->next_element_->prev_element_ = nullptr;
            this->first_element_ = this->first_element_->next_element_;
        } else {    // В противном случае устанавливаем указатель на последний элемент в nullptr
            this->last_element_ = nullptr;
            this->first_element_ = nullptr;
        }

        // Сбрасываем для возвращаемого элемента указатели на следующий и предыдущий элементы
        element_->next_element_ = nullptr;
        element_->prev_element_ = nullptr;

        // Возвращаем свободный для записи элемент буфера
        return element_;
    }

    // В противном случае возвращаем пустой указатель
    return nullptr;
}

//-----------------------------------------------------------------------------------------------------

template <typename T>
void list_memory_buffer<T>::remove_element(_element_t_** element_) {
    //(?) Если передан непустой указатель на освобождаемый элемент, ...
    if (element_) {
        // Получаем индекс возвращаемого элемента
        uint32_t index_ = (*element_)->index_;

        //(?) Проверяем, что возвращаемый элемент соответствует элементу из общего массива
        // Если это так, ...
        auto current_element_ = &this->elements_[index_];
        if ((this->length_ > index_) && (current_element_ = *element_)) {
            //(?) Если свободных элементов в буфере нет, инициализируем список свободных для записи элементов
            if (!this->last_element_) {
                this->first_element_ = *element_;
                this->last_element_ = this->first_element_;
            } else { // ... в противном случае добавляем освобождаемый элемент в конец списка
                this->last_element_->next_element_ = *element_;
                (*element_)->prev_element_ = this->last_element_;
                this->last_element_ = *element_;
            }

            // Сбрасываем переданный указатель на элемент списка
            element_ = nullptr;
        }
    }
}

//-----------------------------------------------------------------------------------------------------

template <typename T>
void list_memory_buffer<T>::init() {
    //(?) Если количество выделяемой памяти не равно 0,...
    if (this->length_) {
        //... выделяем память...
        uint32_t idx_ = 0;
        this->elements_ = new _element_t_[this->length_]();

        //...формируем двусвязный список элементов
        _element_t_* current_element_ = &this->elements_[0];

        for (uint32_t i = 1; i < this->length_; ++i, ++idx_) {
            current_element_->next_element_ = &this->elements_[i];
            this->elements_[i].prev_element_ = current_element_;
            current_element_->index_ = idx_;
            current_element_ = &this->elements_[i];
            //std::cout << "element id = " << idx_ << "\n";
        }
        current_element_->index_ = idx_;

        // Определяем указатели на первый и последний элементы списка
        this->first_element_ = &this->elements_[0];
        this->last_element_ = &this->elements_[this->length_ - 1];
    }
}

//-----------------------------------------------------------------------------------------------------

template <typename T>
void list_memory_buffer<T>::reset(uint32_t l_) {
    // Освобождаем выделенную память в буфере
    this->clear();

    // Фиксируем новый размер буфера хранения элементов
    this->length_ = l_;

    // Инициализируем буфера хранения элементов
    this->init();
}

//-----------------------------------------------------------------------------------------------------

template <typename T>
void list_memory_buffer<T>::clear() {
    //(?) освобождаем память, если она была выделена
    if (this->length_) {
        delete [] this->elements_;
        this->elements_ = nullptr;
    }

    // Сбрасываем указатели на первый и последний элементы
    this->first_element_ = nullptr;
    this->last_element_ = nullptr;

    this->length_ = 0;
}
}       /// <--- container
}   /// <--- mps

#endif
