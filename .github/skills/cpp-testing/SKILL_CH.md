---
name: cpp-testing
description: 仅在编写/更新/修复C++测试、配置GoogleTest/CTest、诊断失败或不稳定的测试、或添加覆盖率/ sanitizer时使用。
origin: ECC
---

# C++测试（Agent技能）

面向Agent的现代C++（C++17/20）测试工作流，使用GoogleTest/GoogleMock和CMake/CTest。

## 何时使用

- 编写新的C++测试或修复现有测试
- 为C++组件设计单元/集成测试覆盖
- 添加测试覆盖、CI门控或回归保护
- 配置CMake/CTest工作流以保持一致执行
- 调查测试失败或不稳定行为
- 启用sanitizer进行内存/竞争诊断

### 何时不使用

- 实现新功能而没有测试更改
- 与测试覆盖或失败无关的大规模重构
- 性能调优没有测试回归需要验证
- 非C++项目或非测试任务

## 核心概念

- **TDD循环**：red → green → refactor（测试优先，最小修复，然后清理）。
- **隔离**：优先依赖注入和fake而非全局状态。
- **测试布局**：`tests/unit`、`tests/integration`、`tests/testdata`。
- **Mock vs fake**：mock用于交互，fake用于有状态行为。
- **CTest发现**：使用`gtest_discover_tests()`进行稳定的测试发现。
- **CI信号**：先运行子集，然后运行完整套件并使用`--output-on-failure`。

## TDD工作流

遵循RED → GREEN → REFACTOR循环：

1. **RED**：编写一个失败的测试来捕获新行为
2. **GREEN**：实现使测试通过的最小更改
3. **REFACTOR**：在测试保持绿色时清理

```cpp
// tests/add_test.cpp
#include <gtest/gtest.h>

int Add(int a, int b); // 由生产代码提供。

TEST(AddTest, AddsTwoNumbers) { // RED
  EXPECT_EQ(Add(2, 3), 5);
}

// src/add.cpp
int Add(int a, int b) { // GREEN
  return a + b;
}

// REFACTOR：测试通过后简化/重命名
```

## 代码示例

### 基本单元测试（gtest）

```cpp
// tests/calculator_test.cpp
#include <gtest/gtest.h>

int Add(int a, int b); // 由生产代码提供。

TEST(CalculatorTest, AddsTwoNumbers) {
    EXPECT_EQ(Add(2, 3), 5);
}
```

### Fixture（gtest）

```cpp
// tests/user_store_test.cpp
// 伪代码存根：请替换为项目类型。
#include <gtest/gtest.h>
#include <memory>
#include <optional>
#include <string>

struct User { std::string name; };
class UserStore {
public:
    explicit UserStore(std::string /*path*/) {}
    void Seed(std::initializer_list<User> /*users*/) {}
    std::optional<User> Find(const std::string &/*name*/) { return User{"alice"}; }
};

class UserStoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        store = std::make_unique<UserStore>(":memory:");
        store->Seed({{"alice"}, {"bob"}});
    }

    std::unique_ptr<UserStore> store;
};

TEST_F(UserStoreTest, FindsExistingUser) {
    auto user = store->Find("alice");
    ASSERT_TRUE(user.has_value());
    EXPECT_EQ(user->name, "alice");
}
```

### Mock（gmock）

```cpp
// tests/notifier_test.cpp
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

class Notifier {
public:
    virtual ~Notifier() = default;
    virtual void Send(const std::string &message) = 0;
};

class MockNotifier : public Notifier {
public:
    MOCK_METHOD(void, Send, (const std::string &message), (override));
};

class Service {
public:
    explicit Service(Notifier &notifier) : notifier_(notifier) {}
    void Publish(const std::string &message) { notifier_.Send(message); }

private:
    Notifier &notifier_;
};

TEST(ServiceTest, SendsNotifications) {
    MockNotifier notifier;
    Service service(notifier);

    EXPECT_CALL(notifier, Send("hello")).Times(1);
    service.Publish("hello");
}
```

### CMake/CTest快速开始

```cmake
# CMakeLists.txt（摘录）
cmake_minimum_required(VERSION 3.20)
project(example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
# 优先使用项目锁定版本。如果使用标签，请根据项目策略使用固定版本。
set(GTEST_VERSION v1.17.0) # 根据项目策略调整。
FetchContent_Declare(
  googletest
  # Google Test框架（官方仓库）
  URL https://github.com/google/googletest/archive/refs/tags/${GTEST_VERSION}.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(example_tests
  tests/calculator_test.cpp
  src/calculator.cpp
)
target_link_libraries(example_tests GTest::gtest GTest::gmock GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(example_tests)
```

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## 运行测试

```bash
ctest --test-dir build --output-on-failure
ctest --test-dir build -R ClampTest
ctest --test-dir build -R "UserStoreTest.*" --output-on-failure
```

```bash
./build/example_tests --gtest_filter=ClampTest.*
./build/example_tests --gtest_filter=UserStoreTest.FindsExistingUser
```

## 调试失败

1. 使用gtest过滤器重新运行单个失败测试。
2. 在失败断言周围添加作用域日志。
3. 启用sanitizer后重新运行。
4. 修复根本原因后扩展到完整套件。

## 覆盖率

优先使用目标级设置而非全局标志。

```cmake
option(ENABLE_COVERAGE "启用覆盖率标志" OFF)

if(ENABLE_COVERAGE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(example_tests PRIVATE --coverage)
    target_link_options(example_tests PRIVATE --coverage)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(example_tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    target_link_options(example_tests PRIVATE -fprofile-instr-generate)
  endif()
endif()
```

GCC + gcov + lcov：

```bash
cmake -S . -B build-cov -DENABLE_COVERAGE=ON
cmake --build build-cov -j
ctest --test-dir build-cov
lcov --capture --directory build-cov --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.info
genhtml coverage.info --output-directory coverage
```

Clang + llvm-cov：

```bash
cmake -S . -B build-llvm -DENABLE_COVERAGE=ON -DCMAKE_CXX_COMPILER=clang++
cmake --build build-llvm -j
LLVM_PROFILE_FILE="build-llvm/default.profraw" ctest --test-dir build-llvm
llvm-profdata merge -sparse build-llvm/default.profraw -o build-llvm/default.profdata
llvm-cov report build-llvm/example_tests -instr-profile=build-llvm/default.profdata
```

## Sanitizer

```cmake
option(ENABLE_ASAN "启用AddressSanitizer" OFF)
option(ENABLE_UBSAN "启用UndefinedBehaviorSanitizer" OFF)
option(ENABLE_TSAN "启用ThreadSanitizer" OFF)

if(ENABLE_ASAN)
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address)
endif()
if(ENABLE_UBSAN)
  add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=undefined)
endif()
if(ENABLE_TSAN)
  add_compile_options(-fsanitize=thread)
  add_link_options(-fsanitize=thread)
endif()
```

## 不稳定测试护栏

- 绝不要使用`sleep`进行同步；使用条件变量或闩锁。
- 为每个测试生成唯一临时目录并始终清理它们。
- 避免单元测试中的真实时间、网络或文件系统依赖。
- 对随机化输入使用确定性种子。

## 最佳实践

### 应该做

- 保持测试确定性和隔离
- 优先依赖注入而非全局
- 对前置条件使用`ASSERT_*`，对多个检查使用`EXPECT_*`
- 在CTest标签或目录中分离单元测试与集成测试
- 在CI中运行sanitizer进行内存和竞争检测

### 不应该做

- 单元测试中依赖真实时间或网络
- 当可以使用条件变量时使用sleep作为同步
- 过度mock简单值对象
- 对非关键日志使用脆弱的字符串匹配

### 常见陷阱

- **使用固定临时路径** → 为每个测试生成唯一临时目录并清理。
- **依赖挂钟时间** → 注入时钟或使用假时间源。
- **不稳定的并发测试** → 使用条件变量/闩锁和有界等待。
- **隐藏全局状态** → 在fixture中重置全局状态或移除全局。
- **过度mock** → 优先使用fake处理有状态行为，只mock交互。
- **缺少sanitizer运行** → 在CI中添加ASan/UBSan/TSan构建。
- **仅在debug构建上覆盖率** → 确保覆盖率目标使用一致的标志。

## 可选附录：模糊测试/属性测试

仅在项目已支持LLVM/libFuzzer或属性测试库时使用。

- **libFuzzer**：最适合I/O最少的纯函数。
- **RapidCheck**：属性测试以验证不变量。

最小libFuzzer harness（伪代码：请替换ParseConfig）：

```cpp
#include <cstddef>
#include <cstdint>
#include <string>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    std::string input(reinterpret_cast<const char *>(data), size);
    // ParseConfig(input); // 项目函数
    return 0;
}
```

## GoogleTest替代方案

- **Catch2**：header-only，富有表现力的matcher
- **doctest**：轻量级，最小编译开销
