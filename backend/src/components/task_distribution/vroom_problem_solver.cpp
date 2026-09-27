#include "components/task_distribution/vroom_problem_solver.hpp"

using namespace td;
using namespace algorithms;

//---------------------------------------------------------------------

vroom_problem_solver::vroom_problem_solver(vroom::TimeWindow day_wind_) : global_time_work_(day_wind_) {}

//---------------------------------------------------------------------

void vroom_problem_solver::set_instances(_vehicles_t_& vehicles_) {
    //(?) Очищаем текущий массив исполнителей
    if (this->instances_.size()) {
        this->instances_.clear();
    }

    // Устанавливаем новый массив
    this->instances_ = std::move(vehicles_);
}

//---------------------------------------------------------------------

void vroom_problem_solver::set_tasks(_tasks_t_& tasks_) {
    if (this->tasks_.size()) {
        this->tasks_.clear();
    }
}

//---------------------------------------------------------------------

void vroom_problem_solver::set_moving_maxtricies(std::vector<_mtx_moving_t_> &mtx_) {
    //(?) Очищаем текущий массив временных матриц движения между задачами
    if (this->moving_matricies_.size()) {
        this->moving_matricies_.clear();
    }

    // Устанавливаем новые значения
    this->moving_matricies_ = std::move(mtx_);
}

//---------------------------------------------------------------------

vroom::Solution vroom_problem_solver::solve_problem(const unsigned exploration_level_,
                                                    const unsigned nb_threads_) {
    //(?) Инициализируем проблему, если этого еще не сделали
    if (!this->problem_) {
        this->init_problem();
    }

    //(?>) Устанавливаем матрицы времени движения по профилям
    for (auto& duration_mtx_ : this->moving_matricies_) {
        this->problem_->set_durations_matrix(duration_mtx_.first,std::move(duration_mtx_.second));
    }

    //(?>) Устанавливаем исполниетелей задач
    for (auto& instance_ : this->instances_) {
        this->problem_->add_vehicle(instance_);
    }

    for (auto& task_ : this->tasks_) {
        this->problem_->add_job(task_);
    }

    return this->problem_->solve(exploration_level_, nb_threads_);
}

//---------------------------------------------------------------------

void vroom_problem_solver::reset() {
    //(?) Если указатель инициализирован, сбрасываем его
    if (this->problem_) {
        delete this->problem_;
        this->problem_ = nullptr;
    }

    this->tasks_.clear();
    this->instances_.clear();
    this->moving_matricies_.clear();
}
