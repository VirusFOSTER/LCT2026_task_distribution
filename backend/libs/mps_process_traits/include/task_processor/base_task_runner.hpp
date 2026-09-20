#ifndef PROCESS_SYSTEM_BASE_TASK_RUNNER_HPP
#define PROCESS_SYSTEM_BASE_TASK_RUNNER_HPP

#include <boost/function.hpp>

namespace mps {
namespace process {
namespace task {
namespace base {
/** ------------------------------------------------------------------------------------------------------------------------
 * @brief The base_task_runner class - базовый класс запуска задачи (компоненты) процесса
 * Главное предназначение данного класса заключается в хранении функции инициализации и запуска компоненты процесса
 * При этом не исключено, что в данном классе может быть какая-то дополнительная информация
 --------------------------------------------------------------------------------------------------------------------------*/
template <class T>
class base_task_runner {
protected:
    using f_init = boost::function<bool(T*)>;
    using f_run = boost::function<void(T*)>;

    f_init initter_ = [ ](T*)->bool{ return false; };         /// <--- инициализатор компоненты
    f_run runner_ = [](T*)->void {};                          /// <--- исполнитель компоненты

    bool task_inited_ = false;      /// <--- признак того, что компонента зарегистрирована

    explicit base_task_runner() {  }
};
}               /// <--- base
}           /// <--- task
}       /// <--- process
}   /// <--- mps

#endif
