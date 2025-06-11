#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <cstring>
#ifdef _WIN32
#include <io.h>
#define ACCESS _access
#define F_OK 0
#else
#include <unistd.h>
#define ACCESS access
#endif

bool isExecutable(const std::string &path) {
#ifdef _WIN32
  const std::vector<std::string> extensions = {".exe", ".bat", ".cmd"};
  for (const auto &ext : extensions) {
    if (path.size() >= ext.size() &&
        path.compare(path.size() - ext.size(), ext.size(), ext) == 0 &&
        ACCESS(path.c_str(), F_OK) == 0) {
      return true;
    }
  }
  return false;
#else
  return ACCESS(path.c_str(), X_OK) == 0;
#endif
}

const std::unordered_set<std::string> builtinCommands = {"exit", "echo", "type"};

void handleExit(std::istringstream &iss)
{
  int status = 0;
  if (iss >> status)
  {
    std::exit(status);
  }
  else
  {
    std::exit(0);
  }
}

void handleEcho(std::istringstream &iss)
{
  std::string rest;
  std::getline(iss >> std::ws, rest);
  std::cout << rest << std::endl;
}

void handleType(std::istringstream &iss)
{
  std::string subcommand;
  while (iss >> subcommand)
  {

    bool found = false;

    if (builtinCommands.count(subcommand))
    {
      std::cout << subcommand << " is a shell builtin" << std::endl;
      return;
    }

    char *pathEnv = std::getenv("PATH");
    if (!pathEnv)
    {
      std::cerr << "PATH not set" << std::endl;
      return;
    }
    std::string pathStr(pathEnv);
    std::stringstream ss(pathStr);
    std::string dir;
    while (std::getline(ss, dir, ':'))
    {
#ifdef _WIN32
      std::string fullPath = dir + "\\" + subcommand;
#else
      std::string fullPath = dir + "/" + subcommand;
#endif
      if (isExecutable(fullPath))
      {
        std::cout << subcommand << " is " << fullPath << std::endl;
        return;
      }
    }
    if (!found)
    {
      std::cout << subcommand << ": not found" << std::endl;
    }
  }
}

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string input;
  while (true)
  {
    std::cout << "$ ";
    if (!std::getline(std::cin, input))
    {
      break;
    }

    if (input.empty())
    {
      continue;
    }

    std::istringstream iss(input);
    std::string command;
    iss >> command;

    if (command == "exit")
    {
      handleExit(iss);
    }
    else if (command == "echo")
    {
      handleEcho(iss);
    }
    else if (command == "type")
    {
      handleType(iss);
    }
    else
    {
      std::cout << input << ": command not found" << std::endl;
    }
  }
  return 0;
}