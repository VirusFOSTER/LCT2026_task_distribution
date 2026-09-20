#ifndef PROCESS_SYSTEM_TASK_WRAPPER_HPP
#define PROCESS_SYSTEM_TASK_WRAPPER_HPP

#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/atomic.hpp>

#include <iostream>
#include <chrono>
#include <thread>

#include "task_runner.hpp"

extern boost::atomic<bool> threads_join_;               /// <--- признак ожидания завершения работы всех потоков
extern boost::atomic<uint16_t> count_tasks_;            /// <--- количество успешно зарегистрированных задач

namespace mps {
namespace process {
namespace task {
namespace wrap {
/**
 * @brief The task_wrapped class - класс-обертка регистрации задачи
 * Данный класс регистрирует задачу (компоненту) для дальнейшего выполнения работ
 * Этап регистрации по сути происходит на этапе чтения конфигурации системы
 */
template <class T>
class task_wrapped : private runner::task_runner<T> {
public:
    /** Конструктор */
    explicit task_wrapped(T *f) :
        task_unwrapped(f) {
        count_tasks_++;
    }

    /** Оператор для обработки исключений зарегистрированных задач */
    void operator()() const {
        /// Сброс прерывания
        try {
            boost::this_thread::interruption_point();
        } catch (const boost::thread_interrupted&) {}

        try {
            if (this->initter_(this->task_unwrapped)) {
                this->runner_(this->task_unwrapped);
            }
        }  catch (const std::exception &e) {
            std::cerr << "\nError! Exception: " << e.what();
        } catch (const boost::thread_interrupted&) {
            std::cerr << "\nError! Thread interrupted!";
        } catch (...) { std::cerr << "\nUnknown error!"; }

        count_tasks_--;
    }

    /**
     * @brief is_init - получение признака регистрации задачи (компоненты)
     * @return признак регистрации
     */
    bool is_init() const { return this->task_inited_; }

private:
    T* task_unwrapped;   /// <--- Указатель на зарегистрированную задачу
};
}               /// <--- wrap
}           /// <--- task
}       /// <--- process
}   /// <--- mps

#endif
