#ifndef PROCESS_SYSTEM_TASK_RUNNER_HPP
#define PROCESS_SYSTEM_TASK_RUNNER_HPP

#include "base_task_runner.hpp"
#include <mps/mps_common/type_traits/types/properties/member_detector.hpp>
#include <mps/mps_common/type_traits/types/properties/type_detector.hpp>

GENERATE_REQUIRED_MEMBER_CLASS(init)
GENERATE_REQUIRED_MEMBER_CLASS(run)


namespace mps {
namespace process {
namespace task {
namespace runner {
/** --------------------------------------------------------------------------------------------------------
 * @brief The task_runner class - класс для определения задачи (компоненты)
 * Установлены следующие правила для запуска компоненты:
 *  - компонента должна иметь метод инициализации init с сигнатурой <bool()>
 *  - компонента должна иметь головную процедуру нити run с сигнатурой <void()>
 * Данный класс является оболочкой запуска компоненты процесса системы
 ----------------------------------------------------------------------------------------------------------*/
template <class T, class Enable = void>
class task_runner : public base::base_task_runner<T> {};

template <class T>
class task_runner <
    T,
    typename boost::enable_if<
        boost::mpl::and_<
            has_field_init<T>,
            has_field_run<T>
            >
        >::type
    > : public base::base_task_runner<T> {
public:
    explicit task_runner() {
        this->initter_ = [ & ](T* task_){ return task_->init(); };
        this->runner_ = [ & ](T* task_) { task_->run(); };
    }
};


template <class T>
class task_runner <
    T,
    typename boost::enable_if<
        boost::mpl::and_<
            boost::mpl::or_<
                boost::is_pointer<T>,
                mps::type_traits::type_detector::is_shared_ptr<T>
                >,
            has_field_init<typename boost::remove_reference<decltype(*T())>::type>,
            has_field_run<typename boost::remove_reference<decltype(*T())>::type>
            >
        >::type
    > {
protected:
    using f_init = boost::function<bool()>;
    using f_run = boost::function<void()>;

    task_runner<typename boost::remove_reference<decltype(*T())>::type> execer_;

    f_init initter_ = [ & ](T){ return false; };
    f_run runner_ = [ & ](T) {};
    bool task_inited_ = false;

    explicit task_runner() {
        this->initter_ = [ & ](T& task_) { return this->execer_.initter_(*task_); };
        this->runner_ = [ & ](const T& task_) { this->execer_.runner_(*task_); };
        this->task_inited_ = true;
    }
};
}               /// <--- runner
}           /// <--- task
}       /// <--- process
}   /// <--- mps

#endif
