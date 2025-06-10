#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string input;
  while (true)
  {
    std::cout << "$ ";
    if (!std::getline(std::cin, input)) {
      break;
    }

    if (input.empty()) {
      continue;
    }

    std::istringstream iss(input);
    std::string command;
    iss >> command;

    if (command == "exit") {
      int status = 0;
      if (iss >> status) {
        std::exit(status);
      } else {
        std::exit(0);
      }
    }

    std::cout << input << ": command not found" << std::endl;
  }
  return 0;
}