#pragma once

#include "cls2srctrl/config.hpp"
#include "cls2srctrl/types.hpp"

namespace fs = std::experimental::filesystem;

namespace cls2st {
class cls2srctrl {
public:
    explicit cls2srctrl(const config& cfg_) : _cfg(cfg_) {
    }

    void parse_cls() {
        std::ifstream ifile(_cfg.cls_input());
        read_json(ifile);
    }

    void create_srctrldb() {
        auto is_open = _srctrl_db_writer.open(_cfg.srctrldb_output().string());
        throw_on_error(is_open);
        _srctrl_db_writer.beginTransaction();
        try {
            // record();
            record_sample_data();
        } catch (const std::exception& ex) {
            std::cout << "caught exception:" << ex.what() << std::endl;
            auto is_rooledback = _srctrl_db_writer.rollbackTransaction();
            throw_on_error(is_rooledback);
            auto is_closed = _srctrl_db_writer.close();
            throw_on_error(is_closed);
            return;
        }
        //_srctrl_db_writer.commitTransaction();
        auto is_closed = _srctrl_db_writer.close();
        throw_on_error(is_closed);
    }

private:
    void throw_on_error(bool is_valid) {
        if(!is_valid) {
            auto error = _srctrl_db_writer.getLastError();
            std::cerr << "ERROR: " << error << std::endl;
            throw std::runtime_error(error);
        }
    }

    void read_json(std::istream& is) {
        pt::ptree root;
        pt::read_json(is, root);

        const auto& sourceinfo_ = root.get_child("sourceinfo");
        _sourceinfo.read_json(sourceinfo_);
        // _sourceinfo.print(std::cout);
    }

    void record_sample_data() {
        auto id = _srctrl_db_writer.recordSymbol({"::", {{"void", __FUNCTION__, "()"}}});
        throw_on_error(id != 0);
    }
    // @todo: shanmukha: SRP
    void record() {
        for (const auto& pkg : _sourceinfo._packages) {
            record_path(pkg._path);
            sourcetrail::NameHierarchy namespace_name;
            namespace_name.nameDelimiter = "::";
            namespace_name.nameElements.push_back({ "", pkg._name, "" });
            auto namespace_id = _srctrl_db_writer.recordSymbol(namespace_name);
	        _srctrl_db_writer.recordSymbolDefinitionKind(namespace_id, sourcetrail::DefinitionKind::EXPLICIT);
	        _srctrl_db_writer.recordSymbolKind(namespace_id, sourcetrail::SymbolKind::NAMESPACE);
            for (const auto& cls : pkg._classes) {
                record_path(cls._path);
                sourcetrail::NameHierarchy class_name = namespace_name;
                class_name.nameElements.push_back({ "", cls._name, "" });
                auto class_id = _srctrl_db_writer.recordSymbol(class_name);
	            _srctrl_db_writer.recordSymbolDefinitionKind(class_id, sourcetrail::DefinitionKind::EXPLICIT);
	            _srctrl_db_writer.recordSymbolKind(class_id, sourcetrail::SymbolKind::CLASS);

                for(const auto& parent: cls._parents) {
                    auto base_id = _srctrl_db_writer.recordSymbol( { "::", { { "", parent, "" } } } );
	                _srctrl_db_writer.recordReference(class_id, base_id, sourcetrail::ReferenceKind::INHERITANCE);
                }
                auto methods = cls._methods._public;
                methods.insert(methods.begin(), cls._methods._private.begin(),
                               cls._methods._private.end());
                for (const auto& meth : methods) {
                    record_path(meth._path);
                    	sourcetrail::NameHierarchy method_name = class_name;
	                    method_name.nameElements.push_back( { "", meth._name, "()" } );
                        auto method_id = _srctrl_db_writer.recordSymbol(method_name);
                        _srctrl_db_writer.recordSymbolDefinitionKind(method_id, sourcetrail::DefinitionKind::EXPLICIT);
                        _srctrl_db_writer.recordSymbolKind(method_id, sourcetrail::SymbolKind::METHOD);
                }
            }

            for (const auto& fcn : pkg._functions) {
                record_path(fcn._path);
            }
        }
    }

    void record_path(const fs::path& path_) {
        return;
        std::cout << "Recoding path:" << path_ << std::endl;
        if (path_.has_extension()) {
            auto ext = path_.extension().string();
            ext.erase(ext.begin());
            auto file_id = _srctrl_db_writer.recordFile(path_.string());
            auto is_recorded = _srctrl_db_writer.recordFileLanguage(file_id, _sourceinfo._language);
            if(!is_recorded) {
                std::cerr << _srctrl_db_writer.getLastError() << std::endl;
            }
        }
    }

    const config& _cfg;
    types::sourceinfo _sourceinfo{};
    sourcetrail::SourcetrailDBWriter _srctrl_db_writer{};
};
} // namespace cls2st
