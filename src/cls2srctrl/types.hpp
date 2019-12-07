#pragma once

#include "common/pch.hpp"

namespace pt = boost::property_tree;
namespace cls2st {
namespace types {
using path_t = std::string;
using package_hierarchy_t = std::vector<std::string>;
using class_hierarchy_t = std::vector<std::string>;
using cmdline_args_t = gsl::span<const char* const>;
using entry_function_t = std::function<void(types::cmdline_args_t)>;

// @todo: replace print with operator << or write method to get back json file
class a_call {
public:
    void read_json(const pt::ptree&);
    void print(std::ostream&) const;

    std::string _name{};
    types::path_t _path{};
    package_hierarchy_t _package_hierarchy{};
    class_hierarchy_t _class_hierarchy{};
};

using calls_t = std::vector<a_call>;

class a_function {
public:
    void read_json(const pt::ptree&);
    void print(std::ostream& os) const;

    std::string _name{};
    types::path_t _path{};
    calls_t _calls{};
};

using variables_t = std::vector<std::string>;
using functions_t = std::vector<a_function>;

class methods {
public:
    void read_json(const pt::ptree&);
    void print(std::ostream&) const;

    functions_t _public{};
    functions_t _private{};

private:
    void print_imp(const std::string&, const functions_t&, std::ostream&) const;
    void populate_property(const pt::ptree&, const std::string&, functions_t&);
};

class properties {
public:
    void read_json(const pt::ptree&);
    void print(std::ostream&) const;
    variables_t _public{};
    variables_t _private{};

private:
    void print_imp(const std::string&, const variables_t&, std::ostream&) const;
    void populate_variables(const pt::ptree&, const std::string&, variables_t&);
};

class a_class {
public:
    void read_json(const pt::ptree&);
    void print(std::ostream&) const;

    std::string _name{};
    types::path_t _path{};
    std::vector<std::string> _parents{};
    methods _methods{};
    properties _properties{};
};

using classes_t = std::vector<a_class>;

class package {
public:
    void read_json(const pt::ptree&);
    void print(std::ostream& os) const;

    std::string _name{};
    types::path_t _path{};
    classes_t _classes{};
    functions_t _functions{};
    variables_t _variables{};
};

using packages_t = std::vector<package>;
class sourceinfo {
public:
    void read_json(const pt::ptree&);
    void print(std::ostream&) const;

    std::string _name{};
    std::string _language{};
    types::path_t _path{};
    packages_t _packages{};
};
} // namespace types
} // namespace cls2st
