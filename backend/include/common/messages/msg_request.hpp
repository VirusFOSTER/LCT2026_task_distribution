#ifndef TASK_DISTRIBTUITION_REQUEST_MESSAGE_HPP
#define TASK_DISTRIBTUITION_REQUEST_MESSAGE_HPP

#include <iostream>
#include <cstring>

namespace td {
namespace msg {
/** -----------------------------------------------
     * @brief The tg_request_type enum - метки запросов
     * Указывают, что хочет получить пользователь
     -------------------------------------------------*/
enum class tg_request_type {
    _tg_unknown_            = 0,        /// <--- тип запроса не известен
    _tg_update_database_    = 1,        /// <--- тип запроса: обновить базу данных
    _tg_processing_data_    = 2,        /// <--- тип запроса: обработать данные (сформировать расписание)
    _tg_get_info_           = 3         /// <--- тип запроса: получить информацию из базы данных
};

/** ----------------------------------------------------------------
     * @brief The tg_update enum - метки обновления данных в базе
     * Обозначает, какую информацию нужно изменить
     ------------------------------------------------------------------*/
enum class tg_update {
    _tg_unknown_            = 0,        /// <--- тип неизвестен
    _tg_append_instances_   = 1,        /// <--- добавить новых исполнителей задач
    _tg_remove_instances_   = 2,        /// <--- удалить исполнительных задач
    _tg_block_instances_    = 3,        /// <--- заблокировать исполнителей задач
    _tg_update_position_    = 4         /// <--- обновление текущего положения исполнителей задач
};

/** ------------------------------------------------------------------
     * @brief The tg_processing enum - метки типов обработки данных
     ---------------------------------------------------------------------*/
enum class tg_processing {
    _tg_unknown_            = 0,        /// <--- тип неизвестен
    _tg_task_distribution_  = 1,        /// <--- распределить задачи между исполнителями
    _tg_process_emergency_  = 2,        /// <--- перераспределить задачи, срочная заявка
    _tg_process_cancel_     = 3         /// <--- перераспределить задачи, заявка отменена
};

/** ----------------------------------------------------------------------
     * @brief The tg_get_info enum - метки получения данных (какая нужна информация пользователю)
     -------------------------------------------------------------------------*/
enum class tg_get_info {
    _tg_unknown_            = 0,        /// <--- тип неизвестен
    _tg_free_instances_     = 1,        /// <--- получение всех свободных исполнителей задач
    _tg_job_instances_      = 2,        /// <--- получение всех занятых исполнителей задач
    _tg_free_tasks_         = 3,        /// <--- получение всех неназнченных задач
    _tg_job_tasks_          = 4,        /// <--- получение всех задач в обработке
    _tg_completed_tasks_    = 5,        /// <--- получение всех выполненных задач
    _tg_current_positions_  = 6         /// <--- текущее положение всех исполнителей
};

/** ------------------------------------------------------------------------------------------------------------
 * @brief The msg_request class - описание запроса от пользователя (описание сообщения)
 * Данный класс является некоторым представлением запросов, которые отправляет пользователь
 * К таким запросам относятся:
 *  - регистрация новых исполнителей задач;
 *  - новые заявки (массив заявок);
 *  - срочная заявка (например, авария);
 *  - отмена заявки;
 *  - получение состояния текущего списка исполнителей задач;
 *  - получение данных о текущих выполняемых задачах;
 *  - получение данных о неназначенных задачах;
 *  и тд
 --------------------------------------------------------------------------------------------------------------*/
class msg_request {
public:
    /**
     * @brief msg_request - конструктор (по умолчанию)
     */
    explicit msg_request() = default;

    /**
     * деструктор
     */
    ~msg_request() = default;

    inline tg_request_type request_type() const { return this->request_type_; }
    inline tg_update update_type() const { return this->update_type_; }
    inline tg_processing processing_type() const { return this->processing_type_; }
    inline tg_get_info info_type() const { return this->info_type_; }

    inline void set_request_type(tg_request_type t_) { this->request_type_ = t_; }
    inline void set_update_type(tg_update t_) { this->update_type_ = t_; }
    inline void set_processing_type(tg_processing t_) { this->processing_type_ = t_; }
    inline void set_info_type(tg_get_info t_) { this->info_type_ = t_; }

    inline std::string data() const { return this->message_; }
    inline void set_data(const std::string& data_) { this->message_ = data_; }

private:
    tg_request_type request_type_ = tg_request_type::_tg_unknown_;      /// <--- тип запроса
    tg_update update_type_ = tg_update::_tg_unknown_;                   /// <--- тип обновления данных
    tg_processing processing_type_ = tg_processing::_tg_unknown_;       /// <--- тип обработки данных
    tg_get_info info_type_ = tg_get_info::_tg_unknown_;                 /// <--- тип запрашиваемых данных

    std::string message_ = "";              /// <--- Входные данные для обработки (должны быть в формате json)
};
}       /// <--- msg
}   /// <--- td

#endif
