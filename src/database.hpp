#pragma once

//std
#include <memory>
#include <string>
#include <list>
#include <unordered_map>

namespace dict_gui {
	using english_str = std::string;
	using romaji_str = std::string;
	using japanese_str = std::wstring;

	struct record_t {
		english_str english;
		romaji_str romaji;
		japanese_str japanese;
	};
	using record_ptr = std::shared_ptr<record_t>;

	class Database {
	public:

	private:
		std::list<record_ptr> _container;
		std::unordered_map<english_str, record_ptr> _sorted_by_english;
		std::unordered_map<romaji_str, record_ptr> _sorted_by_romaji;
		std::unordered_map<japanese_str, record_ptr> _sorted_by_japanese;
	};
}
