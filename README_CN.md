# JSON 转换 C++ 类生成器

一个使用 nlohmann/json 库从 JSON 对象生成 C++ 类的工具，支持序列化和反序列化。

## 特性

- 从 JSON 对象生成 C++ 类
- 支持嵌套对象（创建内部类）
- 支持数组（使用 std::vector）
- 保留 JSON 中的默认值
- 为所有属性生成 getter 和 setter 方法
- 提供 FromJson 和 ToJson 方法用于序列化和反序列化

## 要求

- C++17 或更高版本
- [nlohmann/json](https://github.com/nlohmann/json) 库

## 安装

1. 克隆仓库：
```bash
git clone https://github.com/0cch/json2class.git
cd json2class
```

2. 构建项目：
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## 使用方法

1. 创建一个 JSON 文件，在第一行指定类名（以 # 开头）：

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

2. 运行生成器：

```bash
./json2class path/to/your/json/file.json
```

3. 生成器将创建一个与类同名的头文件：

```cpp
// person.h
class person {
  // 生成的代码...
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

4. 在代码中使用生成的类：

```cpp
#include "person.h"
#include <nlohmann/json.hpp>
#include <iostream>

int main() {
    // 创建一个 person 对象
    person p;
    
    // 修改属性
    p.set_name("John Doe");
    p.set_age(30);
    
    // 序列化为 JSON
    json j = p.ToJson();
    std::cout << j.dump(2) << std::endl;
    
    // 从 JSON 反序列化
    person p2(j);
    
    return 0;
}
```

## 示例

查看 `person_example.cpp` 文件，了解如何使用生成的类的完整示例。

## 许可证

本项目采用 BSD 许可证 - 详情请参阅 LICENSE 文件。
