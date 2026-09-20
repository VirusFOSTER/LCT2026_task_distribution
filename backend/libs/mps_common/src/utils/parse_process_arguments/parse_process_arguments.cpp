#include "utils/parse_process_arguments/parse_process_arguments.hpp"


namespace po = boost::program_options;

namespace mps {
namespace parse {
std::pair<boost::program_options::variables_map, boost::program_options::options_description>
parse_args(int argc, char **argv){
    po::options_description desc_("Allowed options");
    desc_.add_options()("process, proc", po::value<std::string>(), "Process name");
    desc_.add_options()("configuration, cfg", po::value<std::string>(), "Path to configuration");
    auto parsed_ = po::command_line_parser(argc, argv).options(desc_).allow_unregistered().run();

    po::variables_map vm;
    po::store(parsed_, vm);
    po::notify(vm);

    return std::pair(vm, desc_);
}
}       /// <--- parse
}   /// <--- mps
