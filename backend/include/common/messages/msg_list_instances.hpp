#ifndef TASK_DITRIBUTION_LIST_INSTANCE_MESSAGE_HPP
#define TASK_DITRIBUTION_LIST_INSTANCE_MESSAGE_HPP

#include <mps/mps_containers/list/list_buffer.hpp>
#include "common/types/tp_instance.hpp"

namespace td {
namespace msg {
/** -------------------------------------------------------------------------------------------------------
 * @brief The msg_list_instances class - список исполнителей (описание сообщения)
 * Данное сообщение описывает количество всех исполнителей.
 * Формируется модулем обращения к базе данных при поступлении запроса
 ---------------------------------------------------------------------------------------------------------*/
class msg_list_instances {
    using _list_t_ = mps::container::list_buffer<types::tp_instance>;

public:
    /**
     * @brief msg_list_instances - конструктор (по умолчанию)
     */
    explicit msg_list_instances() = default;

    /**
     * деструктор
     */
    ~msg_list_instances() {
        //(?) Освобождение памяти, если она была выделена под описание исоплнителей
        if (this->instances_) {
            delete this->instances_;
            this->instances_ = nullptr;
        }
    }

    /**
     * @brief init_list - инициализация списка исполнителей
     * @param size_ - размер буфера хранения исполнителя
     * @return результат инициализации
     */
    bool init_list(uint32_t size_);

    /**
     * @brief instance_count - получение количества исоплнителей
     * @return количество исполниетелей
     */
    inline uint32_t instance_count() const {
        return (this->instances_) ?
            (this->instances_->count() > this->instances_->length()) ?
                                        this->instances_->length() : this->instances_->count() :
                   0;
    }

    /**
     * @brief get_instance - получение описания отдельно взятого исполнителя
     * @param idx_ - идентификатор исполнителя в массиве
     * @return указатель на описание исполнителя
     */
    inline types::tp_instance* get_instance(uint32_t idx_) const {
        return (this->instances_) ? (*this->instances_)[idx_] : nullptr;
    }

    /**
     * @brief append_instance - добавление описания нового исполнителя
     * @param instance_ - указатель на описание исполнителя
     * @return результат добавления исоплнителя
     */
    inline bool append_instance(types::tp_instance* instance_) {
        return (this->instances_) ? this->instances_->append(instance_) != nullptr : false;
    }

private:
    _list_t_* instances_ = nullptr;         /// <--- список исполнителей
};
}       /// <--- msg
}   /// <--- td

#endif
