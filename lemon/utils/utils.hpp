#pragma once

#include "../general_deps.hpp"

#undef min
#undef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

namespace utils {
	template <typename T>
	std::pair<std::string, std::string> split_typename() {
		auto input = std::string(typeid(T).name());

		std::regex pattern(R"((?:class|struct)\s+(\w+::)?(\w+))");

		std::smatch match;
		if (std::regex_match(input, match, pattern)) {
			if (match.size() == 3) {
				std::string name_space = match[1];
				name_space.erase(std::remove(name_space.begin(), name_space.end(), ':'), name_space.end());

				return {name_space, match[2]};
			}
		}

		return {"", ""};
	}
}  // namespace utils