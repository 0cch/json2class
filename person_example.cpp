// Copyright 2023 0CCH. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "person.h"

int main() {
  // Create a person object using default values
  person p1;

  // Print default values
  std::cout << "Default person:" << std::endl;
  std::cout << "Name: " << p1.name() << std::endl;
  std::cout << "Age: " << p1.age() << std::endl;
  std::cout << "Salary: " << p1.salary() << std::endl;
  std::cout << "Active: " << (p1.active() ? "Yes" : "No") << std::endl;

  // Print skills
  std::cout << "Skills: ";
  for (const auto& skill : p1.skill()) {
    std::cout << skill << " ";
  }
  std::cout << std::endl;

  // Print scores
  std::cout << "Math score: " << p1.scores().Math() << std::endl;
  std::cout << "English score: " << p1.scores().English() << std::endl;

  // Serialize to JSON
  json j1 = p1.ToJson();
  std::cout << "\nSerialized to JSON:\n" << j1.dump(2) << std::endl;

  // Save to file
  std::ofstream out_file("person_default.json");
  out_file << j1.dump(2);
  out_file.close();

  // Create a new person with modified values
  person p2;
  p2.set_name("John Doe");
  p2.set_age(30);
  p2.set_salary(2500.75);
  p2.set_active(false);

  // Modify skills
  std::vector<std::string> new_skills = {"C++", "Python", "JavaScript"};
  p2.set_skill(new_skills);

  // Modify scores
  p2.scores().set_Math(98);
  p2.scores().set_English(85);

  // Serialize modified person to JSON
  json j2 = p2.ToJson();
  std::cout << "\nModified person serialized to JSON:\n"
            << j2.dump(2) << std::endl;

  // Save to file
  std::ofstream out_file2("person_modified.json");
  out_file2 << j2.dump(2);
  out_file2.close();

  // Deserialize from JSON string
  std::string json_str = R"({
    "name": "Alice Smith",
    "age": 28,
    "salary": 3000.0,
    "active": true,
    "skill": ["Java", "SQL", "Management"],
    "scores": {
      "Math": 92,
      "English": 96
    }
  })";

  json j3 = json::parse(json_str);
  person p3(j3);  // Using constructor with JSON

  // Print deserialized values
  std::cout << "\nDeserialized person:" << std::endl;
  std::cout << "Name: " << p3.name() << std::endl;
  std::cout << "Age: " << p3.age() << std::endl;
  std::cout << "Salary: " << p3.salary() << std::endl;
  std::cout << "Active: " << (p3.active() ? "Yes" : "No") << std::endl;

  // Print skills
  std::cout << "Skills: ";
  for (const auto& skill : p3.skill()) {
    std::cout << skill << " ";
  }
  std::cout << std::endl;

  // Print scores
  std::cout << "Math score: " << p3.scores().Math() << std::endl;
  std::cout << "English score: " << p3.scores().English() << std::endl;

  return 0;
}