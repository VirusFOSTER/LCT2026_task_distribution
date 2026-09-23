#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

#include "structures/vroom/input/input.h"
#include "structures/vroom/job.h"
#include "structures/vroom/vehicle.h"
#include "structures/vroom/time_window.h"
#include "structures/vroom/location.h"
#include "utils/exception.h"

// ===============================================================
// Параметры задачи
// ===============================================================
constexpr int NUM_WORKERS = 20;
constexpr int NUM_TASKS   = 200;
constexpr int TOTAL_NODES = NUM_WORKERS + NUM_TASKS;   // 220

// Смена: 9:00–18:00 (в секундах от начала суток)
constexpr int SHIFT_START = 9  * 3600;   // 32400
constexpr int SHIFT_END   = 18 * 3600;   // 64800

// Загрузка матрицы времени из файла.
// Формат: N строк по N чисел (секунды), разделённых пробелами.
// Возвращает vroom::Matrix<vroom::UserDuration> размера N×N.
vroom::Matrix<vroom::UserDuration> load_time_matrix(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        throw std::runtime_error("Не могу открыть файл: " + filename);
    }

    // Считываем все строки в вектор векторов
    std::vector<std::vector<vroom::UserDuration>> data;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::vector<vroom::UserDuration> row;
        double v;
        while (iss >> v) {
            row.push_back(static_cast<vroom::UserDuration>(std::llround(v)));
        }
        if (!row.empty()) data.push_back(std::move(row));
    }

    // Проверка размерности
    const std::size_t n = data.size();
    if (n == 0) {
        throw std::runtime_error("Файл пуст: " + filename);
    }
    for (const auto& row : data) {
        if (row.size() != n) {
            throw std::runtime_error("Матрица не квадратная в файле: " + filename);
        }
    }

    // Создаём vroom::Matrix нужного размера и заполняем
    vroom::Matrix<vroom::UserDuration> matrix(n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            matrix[i][j] = data[i][j];
        }
    }

    return matrix;
}

// ===============================================================
// Вывод решения
// ===============================================================
void log_solution(const vroom::Solution& sol) {
    std::cout << "=== СВОДКА ===\n";
    std::cout << "Общая стоимость:       " << sol.summary.cost << "\n";
    std::cout << "Не выполнено задач:    " << sol.unassigned.size() << "\n";

    if (!sol.unassigned.empty()) {
        std::cout << "ID невыполненных задач: ";
        for (const auto& j : sol.unassigned) {
            std::cout << j.id << " ";
        }
        std::cout << "\n";
    }

    // Считаем, сколько машин реально использовано
    int used = 0;
    for (const auto& r : sol.routes) {
        for (const auto& s : r.steps) {
            if (s.step_type == vroom::STEP_TYPE::JOB) { ++used; break; }
        }
    }
    std::cout << "Использовано рабочих:  " << used << " из " << NUM_WORKERS << "\n";

    std::cout << "\n=== МАРШРУТЫ ===\n";
    for (const auto& route : sol.routes) {
        // Пропускаем пустые маршруты
        int job_count = 0;
        for (const auto& s : route.steps)
            if (s.step_type == vroom::STEP_TYPE::JOB) ++job_count;
        if (job_count == 0) continue;

        std::cout << "\nРабочий " << route.vehicle
                  << " | задач: " << job_count
                  << " | стоимость: " << route.cost
                  << " | длительность: " << route.duration / 60 << " мин"
                  << " | сервис: " << route.service / 60 << " мин\n";

        for (const auto& step : route.steps) {
            std::string type;
            switch (step.step_type) {
            case vroom::STEP_TYPE::START: type = "СТАРТ  "; break;
            case vroom::STEP_TYPE::END:   type = "КОНЕЦ  "; break;
            case vroom::STEP_TYPE::JOB:   type = "ЗАДАЧА "; break;
            case vroom::STEP_TYPE::BREAK:
                break;
            }

            int h = step.arrival / 3600;
            int m = (step.arrival % 3600) / 60;

            // std::cout << "  " << type;
            // if (step.step_type == vroom::STEP_TYPE::JOB) {
            //     std::cout << step.job_type.value();
            // } else {
            //     std::cout << "     ";
            // }
            // std::cout << " | узел " << step.location.index()
            //           << " | прибытие " << h << ":"
            //           << (m < 10 ? "0" : "") << m
            //           << " | ожидание " << step.waiting_time / 60 << " мин"
            //           << " | сервис "   << step.service      / 60 << " мин\n";
        }
    }
}

// ===============================================================
// Сборка задачи VROOM на основе готовой матрицы времени
// ===============================================================
// ===============================================================
// Сборка задачи VROOM на основе готовой матрицы времени
// ===============================================================
vroom::Input build_problem(
    vroom::Matrix<vroom::UserDuration>& time_matrix,
    const std::vector<vroom::Skills>& worker_skills,
    const std::vector<vroom::Skills>& task_skills,
    const std::vector<std::pair<vroom::UserDuration, vroom::UserDuration>>& task_windows,
    const std::vector<vroom::UserDuration>& task_service
    ) {
    vroom::Input problem;

    // ---- 1. Матрица времени ----
    // В C++ API VROOM матрица подаётся с указанием профиля.
    // Тип: vroom::Matrix<vroom::UserDuration> (синоним uint32_t).
    problem.set_durations_matrix("car", std::move(time_matrix));

    // ---- 2. Рабочие (транспортные средства) ----
    const vroom::TimeWindow shift(SHIFT_START, SHIFT_END);

    for (int i = 0; i < NUM_WORKERS; ++i) {
        // Дом рабочего — индекс в матрице (0..NUM_WORKERS-1)
        vroom::Location home(i);

        // Стоимость: большая фиксированная цена за использование машины
        vroom::VehicleCosts costs(
            1'000'000,   // fixed
            3600,        // per_hour
            0,           // per_km
            0            // per_task_hour
            );

        std::optional<vroom::Location> start_loc(home);
        std::optional<vroom::Location> end_loc;

        // Открытый маршрут: start задан, end = std::nullopt.
        // Конструктор Vehicle принимает std::optional<Location> для start и end.
        vroom::Vehicle v(
            i + 1,                      // id
            start_loc,
            end_loc,
            "car",                      // profile
            vroom::Amount(0),           // capacity
            worker_skills[i],           // skills
            shift,                      // time_window (смена)
            {},                         // breaks
            "",                         // description
            costs                       // costs
            );

        problem.add_vehicle(v);
    }

    // ---- 3. Задачи ----
    for (int j = 0; j < NUM_TASKS; ++j) {
        vroom::Location loc(NUM_WORKERS + j);

        vroom::Job job(
            1000 + j,
            loc,
            0,
            task_service[j],
            vroom::Amount(0),
            vroom::Amount(0),
            task_skills[j],
            0,
            {
                vroom::TimeWindow(task_windows[j].first, task_windows[j].second)
            }
            );

        problem.add_job(job);
    }

    return problem;
}
// ===============================================================
// main
// ===============================================================
int main() {
    std::cout << "=== VROOM C++: VRPTW на готовой матрице времени ===\n\n";

    try {
        // ---- 1. Загрузка матрицы ----
        // Файл time_matrix.txt должен содержать 220 строк по 220 чисел.
        auto time_matrix = load_time_matrix("time_matrix.txt");
        std::cout << "Матрица загружена: "
                  << time_matrix.size() << " x " << time_matrix.size() << "\n";

        // ---- 2. Данные о навыках и задачах ----
        // (в реальности — из файла/БД; здесь — синтетика для демонстрации)

        // Навыки рабочих: каждый рабочий — это unordered_set из 1-4 навыков
        std::vector<vroom::Skills> worker_skills(NUM_WORKERS);
        for (int i = 0; i < NUM_WORKERS; ++i) {
            worker_skills[i] = { static_cast<vroom::Skill>((i % 4) + 1),
                                static_cast<vroom::Skill>(((i + 1) % 4) + 1) };
        }

        // Требуемые навыки задач: каждая задача — набор из одного навыка
        std::vector<vroom::Skills> task_skills(NUM_TASKS);
        for (int j = 0; j < NUM_TASKS; ++j) {
            task_skills[j] = { static_cast<vroom::Skill>((j % 4) + 1) };
        }

        // Временные окна: пара UserDuration (start, end)
        std::vector<std::pair<vroom::UserDuration, vroom::UserDuration>> task_windows(NUM_TASKS);
        for (int j = 0; j < NUM_TASKS; ++j) {
            vroom::UserDuration e = SHIFT_START + (j % 8) * 1800;
            vroom::UserDuration l = e + 3 * 3600;
            if (l > SHIFT_END) l = SHIFT_END;
            task_windows[j] = {e, l};
        }

        // Длительности задач: правильный тип — vroom::UserDuration (uint32_t)
        std::vector<vroom::UserDuration> task_service(NUM_TASKS);
        for (int j = 0; j < NUM_TASKS; ++j) {
            task_service[j] = (30 + (j % 4) * 20) * 60; // 30/50/70/90 мин в секундах
        }

        // ---- 3. Сборка задачи ----
        vroom::Input problem = build_problem(
            time_matrix,
            worker_skills,
            task_skills,
            task_windows,
            task_service
            );

        // ---- 4. Решение ----
        // exploration_level: 0 (быстро) … 5 (качественно)
        // nb_threads: число потоков
        const unsigned exploration_level = 5;
        const unsigned nb_threads        = 4;

        vroom::Solution sol = problem.solve(exploration_level, nb_threads);

        // ---- 5. Вывод ----
        log_solution(sol);

    } catch (const vroom::Exception& e) {
        std::cerr << "Ошибка VROOM: " << e.message << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}


/*#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <tuple>

int main() {
    const int NUM_WORKERS = 20;
    const int NUM_TASKS   = 200;
    const int TOTAL       = NUM_WORKERS + NUM_TASKS;  // 220

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> uni(0.0, 1.0);

    // Центры кластеров задач
    std::vector<std::tuple<double,double,double>> clusters = {
                                                                {3,  3,  2.5},
                                                                {3,  15, 2.0},
                                                                {10, 8,  3.0},
                                                                {17, 4,  2.5},
                                                                {17, 16, 2.5},
                                                                };

    // Координаты
    std::vector<std::pair<double,double>> coords;
    coords.reserve(TOTAL);

    // Дома рабочих — случайно по городу 20x20 км
    for (int i = 0; i < NUM_WORKERS; ++i) {
        coords.emplace_back(1 + uni(rng) * 18, 1 + uni(rng) * 18);
    }

    // Задачи — по кластерам
    for (int j = 0; j < NUM_TASKS; ++j) {
        auto [cx, cy, r] = clusters[j % clusters.size()];
        double angle = uni(rng) * 2 * M_PI;
        double rad   = uni(rng) * r;
        coords.emplace_back(cx + rad * std::cos(angle),
                            cy + rad * std::sin(angle));
    }

    // Параметры времени
    const double SPEED_KMH   = 30.0;
    const double ROAD_FACTOR = 1.4;

    std::uniform_real_distribution<double> noise(0.85, 1.15);

    auto travel_time = [&](int i, int j) -> int {
        if (i == j) return 0;
        double dx = coords[i].first  - coords[j].first;
        double dy = coords[i].second - coords[j].second;
        double dist_km = std::sqrt(dx*dx + dy*dy) * ROAD_FACTOR;
        double sec = dist_km / SPEED_KMH * 3600.0;
        sec *= noise(rng);
        return static_cast<int>(std::lround(sec));
    };

    std::ofstream fout("time_matrix.txt");
    if (!fout) { std::cerr << "Не могу открыть файл\n"; return 1; }

    for (int i = 0; i < TOTAL; ++i) {
        for (int j = 0; j < TOTAL; ++j) {
            fout << travel_time(i, j);
            if (j + 1 < TOTAL) fout << ' ';
        }
        fout << '\n';
    }

    std::cout << "Готово: " << TOTAL << " x " << TOTAL
              << " записано в time_matrix.txt\n";
    return 0;
}*/
