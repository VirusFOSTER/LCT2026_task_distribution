#ifndef PROCESS_ARGUMENTS_PARSER_HPP
#define PROCESS_ARGUMENTS_PARSER_HPP

#include <boost/program_options.hpp>

namespace mps {
namespace parse {
/**
 * @brief parse_args - парсер входных параметров процесса
 * @param argc - количество входных параметров
 * @param argv - параметры процесса (наименование процесса системы, полный путь к файлу конфигурации процесса системы)
 */
std::pair<boost::program_options::variables_map,
    boost::program_options::options_description> parse_args(int argc, char **argv);
}       /// <--- parse
}   /// <--- mps

#endif
