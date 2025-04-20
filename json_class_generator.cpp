// Copyright 2025 0CCH. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "json_class_generator.h"

#include <iostream>
#include <sstream>
#include <unordered_set>

JsonClassGenerator::JsonClassGenerator() {}

JsonClassGenerator::~JsonClassGenerator() {}

std::string JsonClassGenerator::GenerateClass(const std::string& class_name,
                                              const json& j,
                                              bool lazy_parsing) {
  std::stringstream ss;

  // Add header includes
  ss << "#ifndef " << class_name << "_H_\n";
  ss << "#define " << class_name << "_H_\n\n";
  ss << "#include <string>\n";
  ss << "#include <vector>\n";
  ss << "#include <map>\n";
  ss << "#include <nlohmann/json.hpp>\n";
  if (lazy_parsing) {
    ss << "#include <optional>\n";
  }
  ss << "\n";
  ss << "using json = nlohmann::json;\n\n";

  // Generate class definition
  ss << "class " << class_name << " {\n";
  ss << " public:\n";
  ss << "  " << class_name << "() = default;\n";
  ss << "  ~" << class_name << "() = default;\n\n";

  // JSON constructor
  ss << "  " << class_name << "(const json& j) {\n";
  if (lazy_parsing) {
    ss << "    json_ = j;\n";
  } else {
    ss << "    FromJson(j);\n";
  }
  ss << "  }\n\n";

  // Add FromJson and ToJson method implementations (directly in the class)
  ss << "  void FromJson(const json& j) {\n";
  ss << GenerateFromJsonMethod(class_name, j, 2, lazy_parsing);
  if (lazy_parsing) {
    ss << "    json_ = j;\n";
  }
  ss << "  }\n\n";

  ss << "  json ToJson() const {\n";
  ss << GenerateToJsonMethod(class_name, j, 2, lazy_parsing);
  ss << "  }\n\n";

  // Add member variables
  ss << " private:\n";
  if (lazy_parsing) {
    ss << "  mutable json json_;\n";
  }
  ss << GenerateClassContent(class_name, j, 1, lazy_parsing);

  // Add getter and setter methods
  ss << " public:\n";
  ss << GenerateGettersSetters(class_name, j, 1, lazy_parsing);

  ss << "};\n\n";
  ss << "#endif  // " << class_name << "_H_\n";

  return ss.str();
}

std::string JsonClassGenerator::GenerateClassContent(
    const std::string& class_name,
    const json& j,
    int indent_level,
    bool lazy_parsing) {
  (void)class_name;
  std::stringstream ss;

  for (auto it = j.begin(); it != j.end(); ++it) {
    const std::string& key = it.key();
    std::string sanitized_key = SanitizeIdentifier(key);
    const json& value = it.value();

    if (value.is_object()) {
      // Nested object, create inner class
      std::string nested_class_name = sanitized_key + "_type";
      ss << Indent(indent_level) << "class " << nested_class_name << " {\n";
      ss << Indent(indent_level) << " public:\n";
      ss << Indent(indent_level + 1) << nested_class_name << "() = default;\n";
      ss << Indent(indent_level + 1) << "~" << nested_class_name
         << "() = default;\n\n";

      // Add member variables and method declarations
      ss << Indent(indent_level + 1) << "void FromJson(const json& j) {\n";
      ss << GenerateFromJsonMethod(nested_class_name, value, indent_level + 2,
                                   lazy_parsing);
      if (lazy_parsing) {
        ss << Indent(indent_level + 2) << "json_ = j;\n";
      }
      ss << Indent(indent_level + 1) << "}\n\n";

      ss << Indent(indent_level + 1) << "json ToJson() const {\n";
      ss << GenerateToJsonMethod(nested_class_name, value, indent_level + 2,
                                 lazy_parsing);
      ss << Indent(indent_level + 1) << "}\n\n";

      ss << Indent(indent_level) << " private:\n";
      if (lazy_parsing) {
        ss << Indent(indent_level + 1) << "mutable json json_;\n";
      }
      ss << GenerateClassContent(nested_class_name, value, indent_level + 1,
                                 lazy_parsing);

      // Add getter and setter methods
      ss << Indent(indent_level) << " public:\n";
      ss << GenerateGettersSetters(nested_class_name, value, indent_level + 1,
                                   lazy_parsing);

      ss << Indent(indent_level) << "};\n\n";

      // Add instance of the inner class
      if (lazy_parsing) {
        ss << Indent(indent_level) << "mutable std::optional<"
           << nested_class_name << "> " << sanitized_key << "_;\n";
      } else {
        ss << Indent(indent_level) << nested_class_name << " " << sanitized_key
           << "_;\n";
      }
    } else {
      // Regular member variable
      std::string type = GetTypeForValue(value);
      std::string default_value = GetDefaultValueString(value);

      if (lazy_parsing) {
        ss << Indent(indent_level) << "mutable std::optional<" << type << "> "
           << sanitized_key << "_{" << "decltype(" << sanitized_key
           << "_)::value_type{" << default_value << "}};\n";
      } else {
        ss << Indent(indent_level) << type << " " << sanitized_key << "_{"
           << default_value << "};\n";
      }
    }
  }

  return ss.str();
}

std::string JsonClassGenerator::GenerateFromJsonMethod(
    const std::string& class_name,
    const json& j,
    int indent_level,
    bool lazy_parsing) {
  (void)class_name;
  std::stringstream ss;

  if (!lazy_parsing) {
    // Directly parse JSON
    for (auto it = j.begin(); it != j.end(); ++it) {
      const std::string& key = it.key();
      const json& value = it.value();

      if (value.is_object()) {
        // Nested object
        ss << Indent(indent_level) << "if (j.contains(\"" << key
           << "\") && j[\"" << key << "\"].is_object()) {\n";
        ss << Indent(indent_level + 1) << key << "_.FromJson(j[\"" << key
           << "\"]);\n";
        ss << Indent(indent_level) << "}\n";
      } else if (value.is_array()) {
        // Array
        ss << Indent(indent_level) << "if (j.contains(\"" << key
           << "\") && j[\"" << key << "\"].is_array()) {\n";
        ss << Indent(indent_level + 1) << key << "_ = j[\"" << key << "\"].get<"
           << GetTypeForValue(value) << ">();\n";
        ss << Indent(indent_level) << "}\n";
      } else {
        // Basic type
        std::string type = GetTypeForValue(value);
        ss << Indent(indent_level) << "if (j.contains(\"" << key << "\")) {\n";
        ss << Indent(indent_level + 1) << key << "_ = j[\"" << key << "\"].get<"
           << type << ">();\n";
        ss << Indent(indent_level) << "}\n";
      }
    }
  } else {
    // Lazy parsing version
    for (auto it = j.begin(); it != j.end(); ++it) {
      const std::string& key = it.key();
      ss << Indent(indent_level) << key << "_.reset();\n";
    }
  }

  return ss.str();
}

std::string JsonClassGenerator::GenerateToJsonMethod(
    const std::string& class_name,
    const json& j,
    int indent_level,
    bool lazy_parsing) {
  (void)class_name;
  std::stringstream ss;

  ss << Indent(indent_level) << "json j;\n";

  for (auto it = j.begin(); it != j.end(); ++it) {
    const std::string& key = it.key();
    const json& value = it.value();

    if (value.is_object()) {
      // Nested object
      if (lazy_parsing) {
        ss << Indent(indent_level) << "j[\"" << key << "\"] = " << key
           << "_.value().ToJson();\n";
      } else {
        ss << Indent(indent_level) << "j[\"" << key << "\"] = " << key
           << "_.ToJson();\n";
      }

    } else {
      // Basic type or array
      if (lazy_parsing) {
        ss << Indent(indent_level) << "j[\"" << key << "\"] = " << key
           << "_.value_or(decltype(" << key << "_)::value_type{"
           << GetDefaultValueString(value) << "});\n";
      } else {
        ss << Indent(indent_level) << "j[\"" << key << "\"] = " << key
           << "_;\n";
      }
    }
  }

  ss << Indent(indent_level) << "return j;\n";

  return ss.str();
}

std::string JsonClassGenerator::GenerateGettersSetters(
    const std::string& class_name,
    const json& j,
    int indent_level,
    bool lazy_parsing) {
  (void)class_name;
  std::stringstream ss;

  for (auto it = j.begin(); it != j.end(); ++it) {
    const std::string& key = it.key();
    const json& value = it.value();
    std::string type = GetTypeForValue(value);

    // Capitalized property name for method names
    std::string capitalized_key = key;
    if (!capitalized_key.empty()) {
      capitalized_key[0] = static_cast<char>(std::toupper(capitalized_key[0]));
    }

    // Getter
    if (value.is_object()) {
      // For nested objects, return reference
      std::string nested_class_name = key + "_type";

      if (lazy_parsing) {
        // Lazy parsing version
        ss << Indent(indent_level) << "const " << nested_class_name << "& "
           << key << "() const {\n";
        ss << Indent(indent_level + 1) << "if (!" << key << "_) {\n";
        ss << Indent(indent_level + 2) << "if (json_.contains(\"" << key
           << "\") && json_[\"" << key << "\"].is_object()) {\n";
        ss << Indent(indent_level + 3) << key << "_.emplace();\n";
        ss << Indent(indent_level + 3) << key << "_->FromJson(json_[\"" << key
           << "\"]);\n";
        ss << Indent(indent_level + 2) << "} else {\n";
        ss << Indent(indent_level + 3) << key << "_.emplace();\n";
        ss << Indent(indent_level + 2) << "}\n";
        ss << Indent(indent_level + 1) << "}\n";
        ss << Indent(indent_level + 1) << "return *" << key << "_;\n";
        ss << Indent(indent_level) << "}\n\n";

        ss << Indent(indent_level) << nested_class_name << "& " << key
           << "() {\n";
        ss << Indent(indent_level + 1) << "if (!" << key << "_) {\n";
        ss << Indent(indent_level + 2) << "if (json_.contains(\"" << key
           << "\") && json_[\"" << key << "\"].is_object()) {\n";
        ss << Indent(indent_level + 3) << key << "_.emplace();\n";
        ss << Indent(indent_level + 3) << key << "_->FromJson(json_[\"" << key
           << "\"]);\n";
        ss << Indent(indent_level + 2) << "} else {\n";
        ss << Indent(indent_level + 3) << key << "_.emplace();\n";
        ss << Indent(indent_level + 2) << "}\n";
        ss << Indent(indent_level + 1) << "}\n";
        ss << Indent(indent_level + 1) << "return *" << key << "_;\n";
        ss << Indent(indent_level) << "}\n\n";
      } else {
        ss << Indent(indent_level) << "const " << nested_class_name << "& "
           << key << "() const {\n";
        ss << Indent(indent_level + 1) << "return " << key << "_;\n";
        ss << Indent(indent_level) << "}\n\n";

        ss << Indent(indent_level) << nested_class_name << "& " << key
           << "() {\n";
        ss << Indent(indent_level + 1) << "return " << key << "_;\n";
        ss << Indent(indent_level) << "}\n\n";
      }
    } else {
      if (lazy_parsing) {
        ss << Indent(indent_level) << "const " << type << "& " << key
           << "() const {\n";
        ss << Indent(indent_level + 1) << "if (!" << key << "_) {\n";
        ss << Indent(indent_level + 2) << "if (json_.contains(\"" << key
           << "\")) {\n";
        if (value.is_array()) {
          ss << Indent(indent_level + 3) << "if (json_[\"" << key
             << "\"].is_array()) {\n";
          ss << Indent(indent_level + 4) << key << "_ = json_[\"" << key
             << "\"].get<" << type << ">();\n";
          ss << Indent(indent_level + 3) << "} else {\n";
          ss << Indent(indent_level + 4) << key << "_ = " << type << "{};\n";
          ss << Indent(indent_level + 3) << "}\n";
        } else {
          ss << Indent(indent_level + 3) << key << "_ = json_[\"" << key
             << "\"].get<" << type << ">();\n";
        }
        ss << Indent(indent_level + 2) << "} else {\n";
        ss << Indent(indent_level + 3) << key << "_ = " << type << "{};\n";
        ss << Indent(indent_level + 2) << "}\n";
        ss << Indent(indent_level + 1) << "}\n";
        ss << Indent(indent_level + 1) << "return *" << key << "_;\n";
        ss << Indent(indent_level) << "}\n\n";

        ss << Indent(indent_level) << type << "& " << key << "() {\n";
        ss << Indent(indent_level + 1) << "if (!" << key << "_) {\n";
        ss << Indent(indent_level + 2) << "if (json_.contains(\"" << key
           << "\")) {\n";
        if (value.is_array()) {
          ss << Indent(indent_level + 3) << "if (json_[\"" << key
             << "\"].is_array()) {\n";
          ss << Indent(indent_level + 4) << key << "_ = json_[\"" << key
             << "\"].get<" << type << ">();\n";
          ss << Indent(indent_level + 3) << "} else {\n";
          ss << Indent(indent_level + 4) << key << "_ = " << type << "{};\n";
          ss << Indent(indent_level + 3) << "}\n";
        } else {
          ss << Indent(indent_level + 3) << key << "_ = json_[\"" << key
             << "\"].get<" << type << ">();\n";
        }
        ss << Indent(indent_level + 2) << "} else {\n";
        ss << Indent(indent_level + 3) << key << "_ = " << type << "{};\n";
        ss << Indent(indent_level + 2) << "}\n";
        ss << Indent(indent_level + 1) << "}\n";
        ss << Indent(indent_level + 1) << "return *" << key << "_;\n";
        ss << Indent(indent_level) << "}\n\n";
      } else {
        ss << Indent(indent_level) << "const " << type << "& " << key
           << "() const {\n";
        ss << Indent(indent_level + 1) << "return " << key << "_;\n";
        ss << Indent(indent_level) << "}\n\n";

        ss << Indent(indent_level) << type << "& " << key << "() {\n";
        ss << Indent(indent_level + 1) << "return " << key << "_;\n";
        ss << Indent(indent_level) << "}\n\n";
      }
    }

    // Setter
    if (value.is_object()) {
      std::string nested_class_name = key + "_type";
      if (lazy_parsing) {
        ss << Indent(indent_level) << "void set_" << key << "(const "
           << nested_class_name << "& value) {\n";
        ss << Indent(indent_level + 1) << key << "_ = value;\n";
        ss << Indent(indent_level) << "}\n\n";
      } else {
        ss << Indent(indent_level) << "void set_" << key << "(const "
           << nested_class_name << "& value) {\n";
        ss << Indent(indent_level + 1) << key << "_ = value;\n";
        ss << Indent(indent_level) << "}\n\n";
      }
    } else {
      if (lazy_parsing) {
        ss << Indent(indent_level) << "void set_" << key << "(const " << type
           << "& value) {\n";
        ss << Indent(indent_level + 1) << key << "_ = value;\n";
        ss << Indent(indent_level) << "}\n\n";
      } else {
        ss << Indent(indent_level) << "void set_" << key << "(const " << type
           << "& value) {\n";
        ss << Indent(indent_level + 1) << key << "_ = value;\n";
        ss << Indent(indent_level) << "}\n\n";
      }
    }
  }

  return ss.str();
}

std::string JsonClassGenerator::GetTypeForValue(const json& value) {
  if (value.is_string()) {
    return "std::string";
  } else if (value.is_boolean()) {
    return "bool";
  } else if (value.is_number_integer()) {
    return "int";
  } else if (value.is_number_unsigned()) {
    return "unsigned int";
  } else if (value.is_number_float()) {
    return "double";
  } else if (value.is_array()) {
    if (value.empty()) {
      return "std::vector<std::string>";  // Default to string array
    } else {
      return "std::vector<" + GetTypeForValue(value[0]) + ">";
    }
  } else if (value.is_object()) {
    return "std::map<std::string, std::string>";  // Default to string map
  } else if (value.is_null()) {
    return "std::string";  // Default to string
  }

  return "std::string";  // Default to string
}

std::string JsonClassGenerator::GetDefaultValueString(const json& value) {
  if (value.is_string()) {
    return "\"" + value.get<std::string>() + "\"";
  } else if (value.is_boolean()) {
    return value.get<bool>() ? "true" : "false";
  } else if (value.is_number_integer()) {
    return std::to_string(value.get<int>());
  } else if (value.is_number_unsigned()) {
    return std::to_string(value.get<unsigned int>());
  } else if (value.is_number_float()) {
    return std::to_string(value.get<double>());
  } else if (value.is_array()) {
    if (value.empty()) {
      return "[]";
    }

    std::stringstream ss;

    for (size_t i = 0; i < value.size(); ++i) {
      ss << GetDefaultValueString(value[i]);
      if (i < value.size() - 1) {
        ss << ", ";
      }
    }

    return ss.str();
  } else if (value.is_null()) {
    return "";
  }

  return "";
}

std::string JsonClassGenerator::Indent(int level) {
  return std::string(level * 2, ' ');
}

std::string JsonClassGenerator::SanitizeIdentifier(
    const std::string& identifier) {
  // C++ Keywords
  static const std::unordered_set<std::string> cpp_keywords = {
      "alignas",       "alignof",     "and",
      "and_eq",        "asm",         "auto",
      "bitand",        "bitor",       "bool",
      "break",         "case",        "catch",
      "char",          "char8_t",     "char16_t",
      "char32_t",      "class",       "compl",
      "concept",       "const",       "consteval",
      "constexpr",     "constinit",   "const_cast",
      "continue",      "co_await",    "co_return",
      "co_yield",      "decltype",    "default",
      "delete",        "do",          "double",
      "dynamic_cast",  "else",        "enum",
      "explicit",      "export",      "extern",
      "false",         "float",       "for",
      "friend",        "goto",        "if",
      "inline",        "int",         "long",
      "mutable",       "namespace",   "new",
      "noexcept",      "not",         "not_eq",
      "nullptr",       "operator",    "or",
      "or_eq",         "private",     "protected",
      "public",        "register",    "reinterpret_cast",
      "requires",      "return",      "short",
      "signed",        "sizeof",      "static",
      "static_assert", "static_cast", "struct",
      "switch",        "template",    "this",
      "thread_local",  "throw",       "true",
      "try",           "typedef",     "typeid",
      "typename",      "union",       "unsigned",
      "using",         "virtual",     "void",
      "volatile",      "wchar_t",     "while",
      "xor",           "xor_eq"};

  // Check if the identifier is a C++ keyword
  if (cpp_keywords.find(identifier) != cpp_keywords.end()) {
    return identifier + "_rn";
  }

  return identifier;
}