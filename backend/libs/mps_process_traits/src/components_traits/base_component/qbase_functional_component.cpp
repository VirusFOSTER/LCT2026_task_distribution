#include "components_traits/base_component/qbase_functional_component.hpp"

using namespace mps;
using namespace process;
using namespace component;
using namespace base;

//--------------------------------

qbase_functional_component::qbase_functional_component(const std::string fc_name_) :
    QThread(nullptr), component_name_(fc_name_) {}
