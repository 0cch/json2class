# JSON to C++ Class Generator

A tool for generating C++ classes from JSON objects with support for serialization and deserialization using the nlohmann/json library.

## Features

- Generates C++ classes from JSON objects
- Supports nested objects (creates inner classes)
- Supports arrays (using std::vector)
- Preserves default values from the JSON
- Generates getters and setters for all properties
- Provides FromJson and ToJson methods for serialization and deserialization

## Requirements

- C++17 or later
- [nlohmann/json](https://github.com/nlohmann/json) library

## Installation

1. Clone the repository:
```bash
git clone https://github.com/0cch/json2class.git
cd json2class
```

2. Build the project:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

1. Create a JSON file with a class name in the first line (prefixed with #):

```json
#person
{
    "name": "hello",
    "skill": [
        "c++",
        "debug"
    ],
    "age": 26,
    "salary": 1500.5,
    "active": true,
    "scores": {
        "Math": 95,
        "English": 90
    }
}
```

2. Run the generator:

```bash
./json2class path/to/your/json/file.json
```

3. The generator will create a header file with the same name as the class:

```cpp
// person.h
class person {
  // Generated code...
   public:
  person() = default;
  ~person() = default;

  person(const json& j) {
    FromJson(j);
  }

  void FromJson(const json& j) {
    if (j.contains("active")) {
      active_ = j["active"].get<bool>();
    }
    if (j.contains("age")) {
      age_ = j["age"].get<int>();
    }
    if (j.contains("name")) {
      name_ = j["name"].get<std::string>();
    }
    if (j.contains("salary")) {
      salary_ = j["salary"].get<double>();
    }
    if (j.contains("scores") && j["scores"].is_object()) {
      scores_.FromJson(j["scores"]);
    }
    if (j.contains("skill") && j["skill"].is_array()) {
      skill_ = j["skill"].get<std::vector<std::string>>();
    }
  }

  json ToJson() const {
    json j;
    j["active"] = active_;
    j["age"] = age_;
    j["name"] = name_;
    j["salary"] = salary_;
    j["scores"] = scores_.ToJson();
    j["skill"] = skill_;
    return j;
  }

 private:
  bool active_{true};
  int age_{26};
  std::string name_{"hello"};
  double salary_{1500.500000};
  class scores_type {
   public:
    scores_type() = default;
    ~scores_type() = default;

    void FromJson(const json& j) {
      if (j.contains("English")) {
        English_ = j["English"].get<int>();
      }
      if (j.contains("Math")) {
        Math_ = j["Math"].get<int>();
      }
    }

    json ToJson() const {
      json j;
      j["English"] = English_;
      j["Math"] = Math_;
      return j;
    }

   private:
    int English_{90};
    int Math_{95};
   public:
    const int& English() const {
      return English_;
    }

    int& English() {
      return English_;
    }

    void set_English(const int& value) {
      English_ = value;
    }

    const int& Math() const {
      return Math_;
    }

    int& Math() {
      return Math_;
    }

    void set_Math(const int& value) {
      Math_ = value;
    }

  };

  scores_type scores_;
  std::vector<std::string> skill_{"c++", "debug"};
 public:
  const bool& active() const {
    return active_;
  }

  bool& active() {
    return active_;
  }

  void set_active(const bool& value) {
    active_ = value;
  }

  const int& age() const {
    return age_;
  }

  int& age() {
    return age_;
  }

  void set_age(const int& value) {
    age_ = value;
  }

  const std::string& name() const {
    return name_;
  }

  std::string& name() {
    return name_;
  }

  void set_name(const std::string& value) {
    name_ = value;
  }

  const double& salary() const {
    return salary_;
  }

  double& salary() {
    return salary_;
  }

  void set_salary(const double& value) {
    salary_ = value;
  }

  const scores_type& scores() const {
    return scores_;
  }

  scores_type& scores() {
    return scores_;
  }

  void set_scores(const scores_type& value) {
    scores_ = value;
  }

  const std::vector<std::string>& skill() const {
    return skill_;
  }

  std::vector<std::string>& skill() {
    return skill_;
  }

  void set_skill(const std::vector<std::string>& value) {
    skill_ = value;
  }
};
```

4. Use the generated class in your code:

```cpp
#include "person.h"
#include <nlohmann/json.hpp>
#include <iostream>

int main() {
    // Create a person object
    person p;
    
    // Modify properties
    p.set_name("John Doe");
    p.set_age(30);
    
    // Serialize to JSON
    json j = p.ToJson();
    std::cout << j.dump(2) << std::endl;
    
    // Deserialize from JSON
    person p2(j);
    
    return 0;
}
```

## Example

See the `person_example.cpp` file for a complete example of how to use the generated classes.

## License

This project is licensed under the BSD License - see the LICENSE file for details.

