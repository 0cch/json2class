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

