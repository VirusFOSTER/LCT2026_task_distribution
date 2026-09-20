#ifndef SYSTEM_CONFIGURATION_GLOBAL_HPP
#define SYSTEM_CONFIGURATION_GLOBAL_HPP

#define SCONFIG_GLOBAL  \
public: \
inline bool configuration_read() const { return this->configuration_read_; }    \
\
inline std::string type() const { return this->configuration_type_; }   \
\
inline std::string version() const { return this->configuration_version_; } \
\
private:    \
std::string make_full_path(const std::string &path_) {  \
    size_t last_pose_ = this->configuration_path_.rfind("/");   \
    if (last_pose_ != std::string::npos) {  \
        return this->configuration_path_.substr(0,last_pose_+1) + path_;    \
    }   \
    \
    this->configuration_read_ = false;  \
    \
    return "";  \
}   \
\
private:    \
bool configuration_read_ = false;   \
\
std::string configuration_type_ ="";    \
\
std::string configuration_version_ = "";    \
\
std::string configuration_path_ = "";

#endif
