#include <mps/mps_process_traits/process_runner.hpp>

#define CONFIGURATIN_PATH   \
    (std::string)PROJECT_PATH + "/configuration/task_distribution_configuration.json"

int main() {
    mps::process::process_runner runner_("task_distribution",CONFIGURATIN_PATH);
    runner_.start_process();

    return 0;
}
