#pragma once

//std
#include <memory>
#include <string>
#include <vector>

namespace dict_gui {
	struct record_t {
		std::vector<std::string> english;
		std::vector<std::string> romaji;
		std::vector<std::string> japanese;
	};
	using record_ptr = std::shared_ptr<record_t>;

	class Database {
	public:
		int load(const std::string& filename);
		int save(const std::string& filename) const;

		size_t size() const;

		record_ptr operator[](const size_t& idx);

		void emplace(record_ptr ptr);
		void clear();

	private:
		std::vector<record_ptr> _container;
	};
}
