// Copyright 2025 0CCH. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JSON_CLASS_GENERATOR_H_
#define JSON_CLASS_GENERATOR_H_

#include <nlohmann/json.hpp>
#include <set>
#include <string>
#include <vector>

// Define version
#define JSON_CLASS_GENERATOR_VERSION "0.0.2"

using json = nlohmann::json;

// Generates C++ classes from JSON objects with support for serialization
// and deserialization.
class JsonClassGenerator {
 public:
  JsonClassGenerator();
  ~JsonClassGenerator();

  // Generates a C++ class definition from a JSON object.
  // |class_name|: The name of the class to generate.
  // |j|: The JSON object to generate the class from.
  // |lazy_parsing|: If true, the class will use lazy parsing for JSON
  // objects.
  // Returns: A string containing the C++ class definition.
  std::string GenerateClass(const std::string& class_name,
                            const json& j,
                            bool lazy_parsing = false);

 private:
  // Generates the content of a class (member variables and nested classes).
  std::string GenerateClassContent(const std::string& class_name,
                                   const json& j,
                                   int indent_level = 0,
                                   bool lazy_parsing = false);

  // Generates the implementation of the FromJson method.
  std::string GenerateFromJsonMethod(const std::string& class_name,
                                     const json& j,
                                     int indent_level = 0,
                                     bool lazy_parsing = false);

  // Generates the implementation of the ToJson method.
  std::string GenerateToJsonMethod(const std::string& class_name,
                                   const json& j,
                                   int indent_level = 0,
                                   bool lazy_parsing = false);

  // Generates getter and setter methods for class members.
  std::string GenerateGettersSetters(const std::string& class_name,
                                     const json& j,
                                     int indent_level = 0,
                                     bool lazy_parsing = false);

  // Returns the C++ type corresponding to a JSON value.
  std::string GetTypeForValue(const json& value);

  // Returns a string representation of the default value for a JSON value.
  std::string GetDefaultValueString(const json& value);

  // Returns a string with the specified number of spaces for indentation.
  std::string Indent(int level);

  // Checks if a string is a valid C++ identifier.
  std::string SanitizeIdentifier(const std::string& identifier);

  // Set of class names that have already been generated.
  std::set<std::string> generated_classes_;
};

#endif  // JSON_CLASS_GENERATOR_H_