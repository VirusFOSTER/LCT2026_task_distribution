#ifndef TASK_DISTRIBUTION_METRICS_HPP
#define TASK_DISTRIBUTION_METRICS_HPP

#include <ctdint>

namespace td {
    namespace metrics {
        struct tp_metrics {
            uint32_t tasks_total_ = 0;
            uint32_t tasks_completed_ = 0;
            uint32_t tasks_unassigned_ = 0;
            uint32_t instances_used_ = 0;

            uint32_t total_duration_ = 0;
            uint32_t total_service_ = 0;
            uint32_t total_waiting_ = 0;
            uint32_t total_travel_ = 0;

            float avg_load_sec_ = 0.0f;
            uint32_t max_load_sec_ = 0;
            uint32_t min_load_sec_ = 0;
            float std_load_sec_ = 0.0f;

            uint32_t num_late_ = 0;
            uint32_t late_seconds_ = 0;
        };
    }
}

#endif
