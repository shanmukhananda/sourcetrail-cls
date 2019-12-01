#include "common/pch.hpp"

#include "cls2srctrl/types.hpp"

namespace cls2st {
namespace types {
void a_call::read_json(const pt::ptree& root) {
    _name = root.get<std::string>("name");
    _path = root.get<types::path_t>("path");

    const auto& pkg_hier_ = root.get_child("package_hierarchy");
    for (const auto& pkg_name : pkg_hier_) {
        auto p = pkg_name.second.get_value<std::string>();
        _package_hierarchy.emplace_back(p);
    }

    const auto& cls_hier_ = root.get_child("class_hierarchy");
    for (const auto& cls_name : cls_hier_) {
        auto c = cls_name.second.get_value<std::string>();
        _class_hierarchy.emplace_back(c);
    }
}

void a_call::print(std::ostream& os) const {
    os << "_name:" << _name << "\n";
    os << "_path:" << _path << "\n";

    std::size_t pkg_hier_count{};
    for (const auto& pkg_hier : _package_hierarchy) {
        os << "package_hierarchy(" << ++pkg_hier_count << "):\n";
        os << pkg_hier << "\n";
    }

    std::size_t cls_hier_count{};
    for (const auto& cls_hier : _class_hierarchy) {
        os << "class_hierarchy(" << ++cls_hier_count << "):\n";
        os << cls_hier << "\n";
    }
}

void a_function::read_json(const pt::ptree& root) {
    _name = root.get<std::string>("name");
    _path = root.get<types::path_t>("path");
    const auto& calls_ = root.get_child("calls");
    for (const auto& call : calls_) {
        a_call call_;
        call_.read_json(call.second);
        _calls.emplace_back(call_);
    }
}

void a_function::print(std::ostream& os) const {
    os << "_name:" << _name << "\n";
    os << "_path:" << _path << "\n";

    std::size_t calls_count{};
    for (const auto& call : _calls) {
        os << "calls(" << ++calls_count << "):\n";
        call.print(os);
    }
}

void methods::read_json(const pt::ptree& root) {
    populate_property(root, "public", _public);
    populate_property(root, "private", _private);
}

void methods::print(std::ostream& os) const {
    print_imp("public", _public, os);
    print_imp("private", _private, os);
}

void methods::print_imp(const std::string& child_name,
                        const functions_t& method, std::ostream& os) const {
    std::size_t method_count{};
    for (const auto& met : method) {
        os << child_name << "(" << ++method_count << "):\n";
        met.print(os);
    }
}

void methods::populate_property(const pt::ptree& root,
                                const std::string& child_name,
                                functions_t& method) {
    const auto& method_to_update_ = root.get_child(child_name);
    for (const auto& method_ : method_to_update_) {
        a_function func;
        func.read_json(method_.second);
        method.emplace_back(func);
    }
}

void properties::read_json(const pt::ptree& root) {
    populate_variables(root, "public", _public);
    populate_variables(root, "private", _private);
}

void properties::print(std::ostream& os) const {
    print_imp("public", _public, os);
    print_imp("private", _private, os);
}

void properties::print_imp(const std::string& child_name,
                           const variables_t& vars, std::ostream& os) const {
    std::size_t var_count{};
    for (const auto& var : vars) {
        os << child_name << "(" << ++var_count << "):\n";
        os << var << "\n";
    }
}

void properties::populate_variables(const pt::ptree& root,
                                    const std::string& child_name,
                                    variables_t& var) {
    const auto& var_to_update = root.get_child(child_name);
    for (const auto& var_ : var_to_update) {
        auto v = var_.second.get_value<std::string>();
        var.emplace_back(v);
    }
}

void a_class::read_json(const pt::ptree& root) {
    _name = root.get<std::string>("name");
    _path = root.get<types::path_t>("path");
    const auto& parents_ = root.get_child("parents");
    for (const auto& parent : parents_) {
        auto p = parent.second.get_value<std::string>();
        _parents.emplace_back(p);
    }
    const auto& methods_ = root.get_child("methods");
    _methods.read_json(methods_);

    const auto& properties_ = root.get_child("properties");
    _properties.read_json(properties_);
}

void a_class::print(std::ostream& os) const {
    os << "_name:" << _name << "\n";
    os << "_path:" << _path << "\n";

    std::size_t count_parent{};
    for (const auto& parent : _parents) {
        os << "parent(" << ++count_parent << "):\n";
        os << parent << "\n";
    }
    os << "methods:"
       << "\n";
    _methods.print(os);
    os << "properties:"
       << "\n";
    _properties.print(os);
}

void package::read_json(const pt::ptree& root) {
    _name = root.get<std::string>("name");
    _path = root.get<types::path_t>("path");
    const auto& classes_ = root.get_child("classes");
    for (const auto& cls : classes_) {
        a_class c;
        c.read_json(cls.second);
        _classes.emplace_back(c);
    }

    const auto& functions_ = root.get_child("functions");
    for (const auto& fcn : functions_) {
        a_function f;
        f.read_json(fcn.second);
        _functions.emplace_back(f);
    }

    const auto& variables_ = root.get_child("variables");
    for (const auto& var : variables_) {
        auto v = var.second.get_value<std::string>();
        _variables.emplace_back(v);
    }
}

void package::print(std::ostream& os) const {
    os << "_name:" << _name << "\n";
    os << "_path:" << _path << "\n";

    std::size_t cls_count{};
    for (const auto& cls : _classes) {
        os << "classes(" << ++cls_count << "):\n";
        cls.print(os);
    }
    std::size_t fcn_count{};
    for (const auto& fcn : _functions) {
        os << "functions(" << ++fcn_count << "):\n";
        fcn.print(os);
    }

    std::size_t var_count{};
    for (const auto& var : _variables) {
        os << "variables(" << ++fcn_count << "):\n";
        os << var << "\n";
    }
}

void sourceinfo::read_json(const pt::ptree& root) {
    _name = root.get<std::string>("name");
    _language = root.get<std::string>("language");
    _path = root.get<types::path_t>("path");
    const auto& packages_ = root.get_child("packages");
    for (const auto& pkg : packages_) {
        package p;
        p.read_json(pkg.second);
        _packages.emplace_back(p);
    }
}

void sourceinfo::print(std::ostream& os) const {
    os << "sourceinfo:"
       << "\n";
    os << "_name:" << _name << "\n";
    os << "_language:" << _language << "\n";
    os << "_path:" << _path << "\n";

    std::size_t count{};
    for (const auto& pkg : _packages) {
        os << "package(" << ++count << "):\n";
        pkg.print(os);
    }
}
} // namespace types
} // namespace cls2st
