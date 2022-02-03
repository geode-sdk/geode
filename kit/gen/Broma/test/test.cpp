#include "../src/broma.hpp"

int main() {
	stringstream s;
	s << "#include <test/ftest.bro>";
	auto toks = lexStream(s);
	auto cacRoot = parseTokens(toks);

	for (auto& g : cacRoot.classes) {
		std::cout << g.first << endl;
	}
}