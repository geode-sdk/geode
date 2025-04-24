#include <chrono>
#include <iostream>

int main() {
	auto start = std::chrono::high_resolution_clock::now();
	system("buildgc");
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "\n\nBuild took: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s\n";
	return 0;
}
