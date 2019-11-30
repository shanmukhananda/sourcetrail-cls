#include "common/pch.hpp"

#include "cls2srctrl/cls2srctrl.hpp"

namespace cls2st {

cls2srctrl::cls2srctrl(const config& cfg_) : _cfg(cfg_) {
}

void cls2srctrl::parse_cls() {
    std::ifstream ifile(_cfg.cls_input());
    read_json(ifile);
}

void cls2srctrl::create_srctrldb() {
    // srctrldb created this way, fails to load, reason UNKNOWN
    // std::string output_fname = _cfg.srctrldb_output().string();
    // std::string output_fname =
    // _cfg.srctrldb_output().filename().string();
    // @todo: debug why the file created by std::filesystem::path fails to
    // load in Sourcetrail
    std::string output_fname = "project.srctrldb";
    auto is_open = _srctrl_db_writer.open(output_fname);
    throw_on_error(is_open);
    auto is_begin_success = _srctrl_db_writer.beginTransaction();
    throw_on_error(is_begin_success);

    try {
        record();
        // record_sample_data();
    } catch (const std::exception& ex) {
        std::cerr << "ERROR: caught exception:" << ex.what() << std::endl;
        auto is_rolledback = _srctrl_db_writer.rollbackTransaction();
        throw_on_error(is_rolledback);
        auto is_closed = _srctrl_db_writer.close();
        throw_on_error(is_closed);
        return;
    }
    auto is_commited = _srctrl_db_writer.commitTransaction();
    throw_on_error(is_commited);
    auto is_closed = _srctrl_db_writer.close();
    throw_on_error(is_closed);
}

void cls2srctrl::throw_on_error(bool is_valid) {
    if (is_valid) {
        return;
    }
    auto error = _srctrl_db_writer.getLastError();
    std::cerr << "ERROR: " << error << std::endl;
    throw std::runtime_error(error);
}

void cls2srctrl::read_json(std::istream& is) {
    pt::ptree root;
    pt::read_json(is, root);

    const auto& sourceinfo_ = root.get_child("sourceinfo");
    _sourceinfo.read_json(sourceinfo_);
    // _sourceinfo.print(std::cout);
}

void cls2srctrl::record_sample_data() {
    auto id = _srctrl_db_writer.recordSymbol(
        {"::", {{"void", "hello_sourcetraildb", "()"}}});
    throw_on_error(id != 0);
}

// @todo: check the return values, throw on error
void cls2srctrl::record() {
    for (const auto& pkg : _sourceinfo._packages) {
        record_package(pkg);
    }
}

void cls2srctrl::record_package(const types::package& pkg) {
    record_path(pkg._path);
    sourcetrail::NameHierarchy namespace_name;
    record_package_name(namespace_name, pkg);
    record_package_classes(namespace_name, pkg);
    record_package_functions(namespace_name, pkg);
    record_package_variables(namespace_name, pkg);
}

void cls2srctrl::record_package_name(sourcetrail::NameHierarchy& namespace_name,
                                     const types::package& pkg) {
    namespace_name.nameDelimiter = NAMESPACE_DELIMITER;
    namespace_name.nameElements.push_back({"", pkg._name, ""});
    auto namespace_id = _srctrl_db_writer.recordSymbol(namespace_name);
    _srctrl_db_writer.recordSymbolDefinitionKind(
        namespace_id, sourcetrail::DefinitionKind::EXPLICIT);
    _srctrl_db_writer.recordSymbolKind(namespace_id,
                                       sourcetrail::SymbolKind::NAMESPACE);
}

void cls2srctrl::record_package_classes(
    sourcetrail::NameHierarchy& namespace_name, const types::package& pkg) {
    for (const auto& cls : pkg._classes) {
        record_class(namespace_name, cls);
    }
}

void cls2srctrl::record_class(sourcetrail::NameHierarchy& namespace_name,
                              const types::a_class& cls) {
    record_path(cls._path);
    sourcetrail::NameHierarchy class_name = namespace_name;
    auto class_id = record_class_name(class_name, cls);
    record_class_parents(class_id, cls);
    // @todo: Find API to represent public and private
    record_class_methods(class_name, cls);
    record_class_properties(class_name, cls);
}

int cls2srctrl::record_class_name(sourcetrail::NameHierarchy& class_name,
                                  const types::a_class& cls) {
    class_name.nameElements.push_back({"", cls._name, ""});
    auto class_id = _srctrl_db_writer.recordSymbol(class_name);
    _srctrl_db_writer.recordSymbolDefinitionKind(
        class_id, sourcetrail::DefinitionKind::EXPLICIT);
    _srctrl_db_writer.recordSymbolKind(class_id,
                                       sourcetrail::SymbolKind::CLASS);
    return class_id;
}

void cls2srctrl::record_class_parents(const int class_id,
                                      const types::a_class& cls) {
    for (const auto& parent : cls._parents) {
        auto base_id = _srctrl_db_writer.recordSymbol(
            {NAMESPACE_DELIMITER, {{"", parent, ""}}});
        _srctrl_db_writer.recordReference(
            class_id, base_id, sourcetrail::ReferenceKind::INHERITANCE);
    }
}

void cls2srctrl::record_class_methods(sourcetrail::NameHierarchy& class_name,
                                      const types::a_class& cls) {
    auto methods = cls._methods._public;
    methods.insert(methods.begin(), cls._methods._private.begin(),
                   cls._methods._private.end());
    for (const auto& meth : methods) {
        record_path(meth._path);
        sourcetrail::NameHierarchy method_name = class_name;
        method_name.nameElements.push_back({"", meth._name, "()"});
        auto method_id = _srctrl_db_writer.recordSymbol(method_name);
        _srctrl_db_writer.recordSymbolDefinitionKind(
            method_id, sourcetrail::DefinitionKind::EXPLICIT);
        _srctrl_db_writer.recordSymbolKind(method_id,
                                           sourcetrail::SymbolKind::METHOD);
        record_calls(method_id, meth._calls);
    }
}

void cls2srctrl::record_class_properties(sourcetrail::NameHierarchy& class_name,
                                         const types::a_class& cls) {
    auto propertes = cls._properties._public;
    propertes.insert(propertes.begin(), cls._properties._private.begin(),
                     cls._properties._private.end());

    for (const auto& prop : propertes) {
        sourcetrail::NameHierarchy property_name = class_name;
        property_name.nameElements.push_back({"", prop, ""});
        auto prop_id = _srctrl_db_writer.recordSymbol(property_name);
        _srctrl_db_writer.recordSymbolDefinitionKind(
            prop_id, sourcetrail::DefinitionKind::EXPLICIT);
        _srctrl_db_writer.recordSymbolKind(prop_id,
                                           sourcetrail::SymbolKind::FIELD);
    }
}

void cls2srctrl::record_package_functions(
    sourcetrail::NameHierarchy& namespace_name, const types::package& pkg) {
    for (const auto& fcn : pkg._functions) {
        record_path(fcn._path);
        sourcetrail::NameHierarchy functions_name = namespace_name;
        functions_name.nameElements.push_back({"", fcn._name, "()"});
        auto fcn_id = _srctrl_db_writer.recordSymbol(functions_name);
        _srctrl_db_writer.recordSymbolDefinitionKind(
            fcn_id, sourcetrail::DefinitionKind::EXPLICIT);
        _srctrl_db_writer.recordSymbolKind(fcn_id,
                                           sourcetrail::SymbolKind::FUNCTION);
        record_calls(fcn_id, fcn._calls);
    }
}

void cls2srctrl::record_package_variables(
    sourcetrail::NameHierarchy& namespace_name, const types::package& pkg) {
    for (const auto& var : pkg._variables) {
        sourcetrail::NameHierarchy variables_name = namespace_name;
        variables_name.nameElements.push_back({"", var, ""});
        auto var_id = _srctrl_db_writer.recordSymbol(variables_name);
        _srctrl_db_writer.recordSymbolDefinitionKind(
            var_id, sourcetrail::DefinitionKind::EXPLICIT);
        _srctrl_db_writer.recordSymbolKind(
            var_id, sourcetrail::SymbolKind::GLOBAL_VARIABLE);
    }
}

void cls2srctrl::record_calls(const int caller_id,
                              const types::calls_t& calls) {
    for (const auto& call : calls) {
        record_path(call._path);
        sourcetrail::NameHierarchy callee_name;
        callee_name.nameDelimiter = NAMESPACE_DELIMITER;
        for (const auto& pkg_name : call._package_hierarchy) {
            callee_name.nameElements.push_back({"", pkg_name, ""});
        }
        for (const auto& cls_name : call._class_hierarchy) {
            callee_name.nameElements.push_back({"", cls_name, ""});
        }
        callee_name.nameElements.push_back({"", call._name, "()"});
        auto callee_id = _srctrl_db_writer.recordSymbol(callee_name);

        _srctrl_db_writer.recordSymbolKind(callee_id,
                                           sourcetrail::SymbolKind::FUNCTION);
        auto call_id = _srctrl_db_writer.recordReference(
            caller_id, callee_id, sourcetrail::ReferenceKind::CALL);
    }
}

void cls2srctrl::record_path(const types::path_t& path_) {
    auto file_id = _srctrl_db_writer.recordFile(path_);
    auto is_recorded =
        _srctrl_db_writer.recordFileLanguage(file_id, _sourceinfo._language);
    throw_on_error(is_recorded);
}
} // namespace cls2st
