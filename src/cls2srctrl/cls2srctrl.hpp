#pragma once

#include "common/pch.hpp"

#include "cls2srctrl/config.hpp"
#include "cls2srctrl/types.hpp"

namespace cls2st {
class cls2srctrl {
public:
    explicit cls2srctrl(const config&);
    void parse_cls();
    void create_srctrldb();

private:
    void throw_on_error(bool is_valid);
    void read_json(std::istream& is);
    void record();
    void record_package(const types::package&);
    void
    record_package_name(sourcetrail::NameHierarchy&, const types::package&);
    void
    record_package_classes(sourcetrail::NameHierarchy&, const types::package&);
    void record_class(sourcetrail::NameHierarchy&, const types::a_class&);
    int record_class_name(sourcetrail::NameHierarchy&, const types::a_class&);
    void record_class_parents(const int, const types::a_class&);
    void
    record_class_methods(sourcetrail::NameHierarchy&, const types::a_class&);
    void
    record_class_properties(sourcetrail::NameHierarchy&, const types::a_class&);
    void record_package_functions(sourcetrail::NameHierarchy&,
                                  const types::package&);
    void record_package_variables(sourcetrail::NameHierarchy&,
                                  const types::package&);
    void record_calls(const int, const types::calls_t&);
    void record_path(const types::path_t&);

    const config& _cfg;
    types::sourceinfo _sourceinfo{};
    sourcetrail::SourcetrailDBWriter _srctrl_db_writer{};
    const std::string NAMESPACE_DELIMITER = "::";
};
} // namespace cls2st
