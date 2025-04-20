// Copyright 2025 0CCH. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "json_class_generator.h"

using json = nlohmann::json;

void printUsage(const char* programName) {
  std::cout << "Version: " << JSON_CLASS_GENERATOR_VERSION << std::endl;
  std::cout << "Usage: " << programName << " <json_file_path> [options]"
            << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  --lazy-parsing    启用延迟解析模式" << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printUsage(argv[0]);
    return 1;
  }

  std::string file_path = argv[1];
  bool lazy_parsing = false;

  // Parse command-line arguments
  for (int i = 2; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--lazy-parsing" || arg == "-l") {
      lazy_parsing = true;
    }
  }

  std::ifstream file(file_path);

  if (!file.is_open()) {
    std::cerr << "Unable to open file: " << file_path << std::endl;
    return 1;
  }

  std::string json_content;
  std::string line;
  std::string class_name;

  // Read the first line to get the class name (format: #className)
  if (std::getline(file, line)) {
    if (line.size() > 1 && line[0] == '#') {
      class_name = line.substr(1);
    }
  }

  // Read the remaining JSON content
  while (std::getline(file, line)) {
    json_content += line;
  }

  if (class_name.empty()) {
    std::cerr << "Class name not found. Please ensure the JSON file's first "
                 "line contains a class name in the format #className"
              << std::endl;
    return 1;
  }

  try {
    json j = json::parse(json_content);
    JsonClassGenerator generator;
    std::string cpp_class =
        generator.GenerateClass(class_name, j, lazy_parsing);

    std::cout << "Generation mode: "
              << (lazy_parsing ? "Lazy parsing" : "Direct parsing")
              << std::endl;

    // Output the generated C++ class
    std::cout << cpp_class << std::endl;

    // Save to file
    std::string output_file_name = class_name + ".h";
    std::ofstream output_file(output_file_name);
    if (output_file.is_open()) {
      output_file << cpp_class;
      output_file.close();
      std::cout << "Generated C++ class file: " << output_file_name
                << std::endl;
    } else {
      std::cerr << "Unable to create output file: " << output_file_name
                << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}