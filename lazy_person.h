#ifndef person_H_
#define person_H_

#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

using json = nlohmann::json;

class person {
 public:
  person() = default;
  ~person() = default;

  person(const json& j) { json_ = j; }

  void FromJson(const json& j) {
    active_.reset();
    age_.reset();
    name_.reset();
    salary_.reset();
    scores_.reset();
    skill_.reset();
    json_ = j;
  }

  json ToJson() const {
    json j;
    j["active"] = active_.value_or(decltype(active_)::value_type{true});
    j["age"] = age_.value_or(decltype(age_)::value_type{26});
    j["name"] = name_.value_or(decltype(name_)::value_type{"hello"});
    j["salary"] = salary_.value_or(decltype(salary_)::value_type{1500.500000});
    j["scores"] = scores_.value().ToJson();
    j["skill"] = skill_.value_or(decltype(skill_)::value_type{"c++", "debug"});
    return j;
  }

 private:
  mutable json json_;
  mutable std::optional<bool> active_{decltype(active_)::value_type{true}};
  mutable std::optional<int> age_{decltype(age_)::value_type{26}};
  mutable std::optional<std::string> name_{
      decltype(name_)::value_type{"hello"}};
  mutable std::optional<double> salary_{
      decltype(salary_)::value_type{1500.500000}};
  class scores_type {
   public:
    scores_type() = default;
    ~scores_type() = default;

    void FromJson(const json& j) {
      English_.reset();
      Math_.reset();
      json_ = j;
    }

    json ToJson() const {
      json j;
      j["English"] = English_.value_or(decltype(English_)::value_type{90});
      j["Math"] = Math_.value_or(decltype(Math_)::value_type{95});
      return j;
    }

   private:
    mutable json json_;
    mutable std::optional<int> English_{decltype(English_)::value_type{90}};
    mutable std::optional<int> Math_{decltype(Math_)::value_type{95}};

   public:
    const int& English() const {
      if (!English_) {
        if (json_.contains("English")) {
          English_ = json_["English"].get<int>();
        } else {
          English_ = int{};
        }
      }
      return *English_;
    }

    int& English() {
      if (!English_) {
        if (json_.contains("English")) {
          English_ = json_["English"].get<int>();
        } else {
          English_ = int{};
        }
      }
      return *English_;
    }

    void set_English(const int& value) { English_ = value; }

    const int& Math() const {
      if (!Math_) {
        if (json_.contains("Math")) {
          Math_ = json_["Math"].get<int>();
        } else {
          Math_ = int{};
        }
      }
      return *Math_;
    }

    int& Math() {
      if (!Math_) {
        if (json_.contains("Math")) {
          Math_ = json_["Math"].get<int>();
        } else {
          Math_ = int{};
        }
      }
      return *Math_;
    }

    void set_Math(const int& value) { Math_ = value; }
  };

  mutable std::optional<scores_type> scores_;
  mutable std::optional<std::vector<std::string>> skill_{
      decltype(skill_)::value_type{"c++", "debug"}};

 public:
  const bool& active() const {
    if (!active_) {
      if (json_.contains("active")) {
        active_ = json_["active"].get<bool>();
      } else {
        active_ = bool{};
      }
    }
    return *active_;
  }

  bool& active() {
    if (!active_) {
      if (json_.contains("active")) {
        active_ = json_["active"].get<bool>();
      } else {
        active_ = bool{};
      }
    }
    return *active_;
  }

  void set_active(const bool& value) { active_ = value; }

  const int& age() const {
    if (!age_) {
      if (json_.contains("age")) {
        age_ = json_["age"].get<int>();
      } else {
        age_ = int{};
      }
    }
    return *age_;
  }

  int& age() {
    if (!age_) {
      if (json_.contains("age")) {
        age_ = json_["age"].get<int>();
      } else {
        age_ = int{};
      }
    }
    return *age_;
  }

  void set_age(const int& value) { age_ = value; }

  const std::string& name() const {
    if (!name_) {
      if (json_.contains("name")) {
        name_ = json_["name"].get<std::string>();
      } else {
        name_ = std::string{};
      }
    }
    return *name_;
  }

  std::string& name() {
    if (!name_) {
      if (json_.contains("name")) {
        name_ = json_["name"].get<std::string>();
      } else {
        name_ = std::string{};
      }
    }
    return *name_;
  }

  void set_name(const std::string& value) { name_ = value; }

  const double& salary() const {
    if (!salary_) {
      if (json_.contains("salary")) {
        salary_ = json_["salary"].get<double>();
      } else {
        salary_ = double{};
      }
    }
    return *salary_;
  }

  double& salary() {
    if (!salary_) {
      if (json_.contains("salary")) {
        salary_ = json_["salary"].get<double>();
      } else {
        salary_ = double{};
      }
    }
    return *salary_;
  }

  void set_salary(const double& value) { salary_ = value; }

  const scores_type& scores() const {
    if (!scores_) {
      if (json_.contains("scores") && json_["scores"].is_object()) {
        scores_.emplace();
        scores_->FromJson(json_["scores"]);
      } else {
        scores_.emplace();
      }
    }
    return *scores_;
  }

  scores_type& scores() {
    if (!scores_) {
      if (json_.contains("scores") && json_["scores"].is_object()) {
        scores_.emplace();
        scores_->FromJson(json_["scores"]);
      } else {
        scores_.emplace();
      }
    }
    return *scores_;
  }

  void set_scores(const scores_type& value) { scores_ = value; }

  const std::vector<std::string>& skill() const {
    if (!skill_) {
      if (json_.contains("skill")) {
        if (json_["skill"].is_array()) {
          skill_ = json_["skill"].get<std::vector<std::string>>();
        } else {
          skill_ = std::vector<std::string>{};
        }
      } else {
        skill_ = std::vector<std::string>{};
      }
    }
    return *skill_;
  }

  std::vector<std::string>& skill() {
    if (!skill_) {
      if (json_.contains("skill")) {
        if (json_["skill"].is_array()) {
          skill_ = json_["skill"].get<std::vector<std::string>>();
        } else {
          skill_ = std::vector<std::string>{};
        }
      } else {
        skill_ = std::vector<std::string>{};
      }
    }
    return *skill_;
  }

  void set_skill(const std::vector<std::string>& value) { skill_ = value; }
};

#endif  // person_H_
