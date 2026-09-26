#ifndef TASK_DISTRIBUTION_TASKS_LISTS_MESSAGE_HPP
#define TASK_DISTRIBUTION_TASKS_LISTS_MESSAGE_HPP

#include <mps/mps_containers/list/list_buffer.hpp>
#include "common/types/tp_task.hpp"

namespace td {
namespace msg {
/** ---------------------------------------------------------------------------------------------------------------------
 * @brief The msg_tasks_list class - список задач на исполнение (описание сообщения)
 * Данное сообщение формируется либо модулем обращения к базе данных при поступлении запросов на получение соответствующей
 * информации либо модулем конвертации данных в случае поступления новых заявок на обработку.
 * Содержит информацию о всех задачах, которые:
 *  - выполнены (из базы данных);
 *  - не выполнены (из базы данных);
 *  - которые требуют выполнения (конвертация, поступает из frontend);
 *  - назначены (из базы данных)
 *  и т.д.
 ---------------------------------------------------------------------------------------------------------------------*/
class msg_tasks_list {
    using _list_t_ = mps::container::list_buffer<types::tp_task>;

public:
    /**
     * @brief msg_tasks_list - конструктор (по умолчанию)
     */
    explicit msg_tasks_list() = default;

    /**
     * деструктор
     */
    ~msg_tasks_list() {
        //(?) Освобождение памяти, если она была выделена под описание исоплнителей
        if (this->tasks_) {
            delete this->tasks_;
            this->tasks_ = nullptr;
        }
    }

    /**
     * @brief init_list - инициализация списка задач
     * @param size_ - размер буфера хранения задач
     * @return результат инициализации
     */
    bool init_list(uint32_t size_);

    /**
     * @brief tasks_count - получение количества задач
     * @return количество задач
     */
    inline uint32_t tasks_count() const {
        return (this->tasks_) ?
                   (this->tasks_->count() > this->tasks_->length()) ?
                       this->tasks_->length() : this->tasks_->count() :
                   0;
    }

    /**
     * @brief get_task - получение описания отдельно взятой задачи
     * @param idx_ - идентификатор задачи в массиве
     * @return указатель на описание задачи
     */
    inline types::tp_task* get_task(uint32_t idx_) const {
        return (this->tasks_) ? (*this->tasks_)[idx_] : nullptr;
    }

    /**
     * @brief append_task - добавление описания новой задачи
     * @param task_ - указатель на описание задачи
     * @return результат добавления исоплнителя
     */
    inline bool append_task(types::tp_task* task_) {
        return (this->tasks_) ? this->tasks_->append(task_) != nullptr : false;
    }

private:
    _list_t_* tasks_ = nullptr;             /// <--- Списо задач на исоплнение
};
}       /// <--- msg
}   /// <--- td

#endif
