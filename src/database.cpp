#include "database.hpp"

//std
#include <fstream>
#include <iostream>

//json
#include "json.hpp"

namespace dict_gui {
    nlohmann::json read_json_from_file(const std::string& filename) {
        std::fstream file(filename, std::fstream::in);
        if (!file.is_open())
            throw std::runtime_error("file \'" + filename + "\' is not found");

        nlohmann::json result = nlohmann::json::parse(file);
        file.close();

        return result;
    }
    void write_json_to_file(const std::string& filename, const nlohmann::json& json) {
        std::fstream file(filename, std::fstream::out | std::fstream::trunc);
        if (!file.is_open())
            throw std::runtime_error("file \'" + filename + "\' is not found");

        file << json;
        file.close();
    }

    int Database::load(const std::string& filename) {
        nlohmann::json data;

        try {
            data = read_json_from_file(filename);
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << '\n';
            return -1;
        }

        _container.clear();

        const auto& raw_records = data["records"].get<std::vector<nlohmann::json>>();
        _container.reserve(raw_records.size());
        for (const auto& it : raw_records) {
            auto ptr = std::make_shared<record_t>(
                it["english"].get<std::vector<std::string>>(),
                it["romaji"].get<std::vector<std::string>>(),
                it["japanese"].get<std::vector<std::string>>()
            );

            _container.emplace_back(ptr);
        }

        return 0;
    }
    int Database::save(const std::string& filename) const {
        nlohmann::json temp;
        temp["records"] = nlohmann::json::array();

        auto& records = temp["records"];
        for (const auto& it : _container) {
            auto& curr = records.emplace_back(nlohmann::json::object());
            for (const auto& jt : it->english)
                curr["english"].emplace_back(jt);
            for (const auto& jt : it->romaji)
                curr["romaji"].emplace_back(jt);
            for (const auto& jt : it->japanese)
                curr["japanese"].emplace_back(jt);
        }

        try {
            write_json_to_file(filename, temp);
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << '\n';
            return -1;
        }

        return 0;
    }

    size_t Database::size() const {
        return _container.size();
    }

    record_ptr Database::operator[](const size_t& idx) {
        return _container[idx];
    }

    void Database::emplace(record_ptr ptr) {
        _container.emplace_back(ptr);
    }

    void Database::clear() {
        _container.clear();
    }
}
