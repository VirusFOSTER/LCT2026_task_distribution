#ifndef PROCESS_SYSTEM_TASK_PROCESSOR_HPP
#define PROCESS_SYSTEM_TASK_PROCESSOR_HPP

#include "task_wrapper.hpp"

namespace mps {
namespace process {
namespace task {
/** -------------------------------------------------------------------------------------------------------------------------
 * @brief The task_processor class - класс-регистратор задач процесса
 * В качестве задач для процесса выступают компоненты, логика которых носит универсальный характер
 * Все компоненты в случае успешной регистрации будут выполняться каждый в своем потоке
 * В случае, если одна из компонент аварийно завершает работу, дальнейшее поведение определяется исходя из конфигурации
 * текущего процесса. При этом возможны следующие случаи поведения:
 *  - завершается полностью работа процесса;
 *  - процесс продолжает работу не смотря на отвалившуюся компоненту;
 *  - процесс пытается перезапустить компоненту.
 ---------------------------------------------------------------------------------------------------------------------------*/
class task_processor : private boost::asio::noncopyable {
protected:
    /**
     * @brief task_processor - конструктор
     */
    explicit task_processor(){}

    /**
     * Деструктор
     */
    ~task_processor() = default;

    inline boost::asio::io_service& get_ios() {
        static boost::asio::io_service ios_;
        static boost::asio::io_service::work work_(ios_);

        return ios_;
    }

    /** Функция создания task_wrapped из функтора пользователя */
    template <class T>
    wrap::task_wrapped<T> make_task_wrapped(T *task_unwrapped) {
        return wrap::task_wrapped<T>(task_unwrapped);
    }

    /**
     * @brief push_task - регистрация задачи на выполнение
     * @param task_unwrap_ - указатель на регистрируемую задачу
     * @return результат регистрации задачи
     */
    template <typename T>
    bool push_task(T* task_unwrap_) {
        auto task_wrapped_ = this->make_task_wrapped(task_unwrap_);
        if (!task_wrapped_.is_init()) {
            this->get_ios().post(task_wrapped_);
            return true;
        }

        this->count_errors_++;
        return false;
    }

    /** Запуск всех компонент */
    void start() {
        if (count_tasks_) {
            boost::asio::io_service &ios_ = this->get_ios();
            for (std::size_t i = 0; i < count_tasks_; i++) {
                this->task_threads_.create_thread([ &ios_ ](){
                    ios_.run();
                });
            }

            if (threads_join_) {
                while (count_tasks_) {
                    std::this_thread::sleep_for(std::chrono::microseconds(10));
                }

                ios_.stop();
                this->task_threads_.join_all();
            }
        }
    }

protected:
    uint16_t count_errors_ = 0;                         /// <--- количество ошибок регистрации задач
    boost::thread_group task_threads_;                  /// <--- группа потоков выполнения нитей процесса
};
}           /// <--- task
}       /// <--- process
}   /// <--- mps

#endif
