---
name: cpp-coding-standards
description: 基于C++ Core Guidelines（isocpp.github.io）的C++编码标准。在编写、审查或重构C++代码时使用，以强制执行现代、安全和惯用的实践。
origin: ECC
---

# C++编码标准（C++ Core Guidelines）

基于[C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)的现代C++（C++17/20/23）全面编码标准。强制执行类型安全、资源安全、不可变性和清晰度。

## 何时使用

- 编写新的C++代码（类、函数、模板）
- 审查或重构现有C++代码
- 在C++项目中做出架构决策
- 在C++代码库中强制执行一致的样式
- 在语言特性之间进行选择（例如，`enum` vs `enum class`，原始指针vs智能指针）

### 何时不使用

- 非C++项目
- 无法采用现代C++特性的遗留C代码库
- 嵌入式/裸金属上下文，其中特定指南与硬件约束冲突（选择性适配）

## 跨领域原则

这些主题在整个指南中反复出现，形成基础：

1. **随时随地使用RAII**（P.8, R.1, E.6, CP.20）：将资源生命周期绑定到对象生命周期
2. **默认不可变性**（P.10, Con.1-5, ES.25）：从`const`/`constexpr`开始；可变是例外
3. **类型安全**（P.4, I.4, ES.46-49, Enum.3）：使用类型系统在编译时防止错误
4. **表达意图**（P.3, F.1, NL.1-2, T.10）：名称、类型和概念应传达目的
5. **最小化复杂性**（F.2-3, ES.5, Per.4-5）：简单代码是正确的代码
6. **值语义优于指针语义**（C.10, R.3-5, F.20, CP.31）：优先返回按值返回和作用域对象

## 哲学与接口（P.*, I.*）

### 关键规则

| 规则 | 摘要 |
|------|---------|
| **P.1** | 用代码直接表达想法 |
| **P.3** | 表达意图 |
| **P.4** | 理想情况下，程序应是静态类型安全的 |
| **P.5** | 优先编译时检查而非运行时检查 |
| **P.8** | 不要泄漏任何资源 |
| **P.10** | 优先不可变数据而非可变数据 |
| **I.1** | 使接口显式 |
| **I.2** | 避免非const全局变量 |
| **I.4** | 使接口精确且强类型化 |
| **I.11** | 绝不通过原始指针或引用转移所有权 |
| **I.23** | 保持函数参数数量少 |

### 应该做

```cpp
// P.10 + I.4：不可变、强类型化接口
struct Temperature {
    double kelvin;
};

Temperature boil(const Temperature& water);
```

### 不应该做

```cpp
// 弱接口：所有权不清晰，单位不清晰
double boil(double* temp);

// 非const全局变量
int g_counter = 0;  // I.2违规
```

## 函数（F.*）

### 关键规则

| 规则 | 摘要 |
|------|---------|
| **F.1** | 将有意义的操作打包成精心命名的函数 |
| **F.2** | 函数应执行单个逻辑操作 |
| **F.3** | 保持函数简短简单 |
| **F.4** | 如果函数可能在编译时求值，声明为`constexpr` |
| **F.6** | 如果函数必须不抛出，声明为`noexcept` |
| **F.8** | 优先纯函数 |
| **F.16** | 对于"输入"参数，按值传递廉价类型，其他按`const&`传递 |
| **F.20** | 对于"输出"值，优先返回值而非输出参数 |
| **F.21** | 要返回多个"输出"值，优先返回结构体 |
| **F.43** | 绝不返回指向局部对象的指针或引用 |

### 参数传递

```cpp
// F.16：廉价类型按值传递，其他按const&传递
void print(int x);                           // 廉价：按值
void analyze(const std::string& data);       // 昂贵：按const&
void transform(std::string s);               // 接收：按值传递（将移动）

// F.20 + F.21：返回值，而非输出参数
struct ParseResult {
    std::string token;
    int position;
};

ParseResult parse(std::string_view input);   // 好：返回结构体

// 不好：输出参数
void parse(std::string_view input,
           std::string& token, int& pos);    // 避免这样
```

### 纯函数和constexpr

```cpp
// F.4 + F.8：尽可能使用纯函数和constexpr
constexpr int factorial(int n) noexcept {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

static_assert(factorial(5) == 120);
```

### 反模式

- 从函数返回`T&&`（F.45）
- 使用`va_arg`/C风格可变参数（F.55）
- 在传递给其他线程的lambda中按引用捕获（F.53）
- 返回`const T`以抑制移动语义（F.49）

## 类与类层次结构（C.*）

### 关键规则

| 规则 | 摘要 |
|------|---------|
| **C.2** | 如果存在不变量使用`class`；如果数据成员独立变化使用`struct` |
| **C.9** | 最小化成员暴露 |
| **C.20** | 如果可以避免定义默认操作，就避免（零规则） |
| **C.21** | 如果定义或`=delete`任何复制/移动/析构函数，处理全部（五规则） |
| **C.35** | 基类析构函数：public virtual或protected non-virtual |
| **C.41** | 构造函数应创建完全初始化的对象 |
| **C.46** | 将单参数构造函数声明为`explicit` |
| **C.67** | 多态类应抑制公共复制/移动 |
| **C.128** | 虚函数：精确指定`virtual`、`override`或`final`之一 |

### 零规则

```cpp
// C.20：让编译器生成特殊成员
struct Employee {
    std::string name;
    std::string department;
    int id;
    // 不需要析构函数、复制/移动构造函数或赋值运算符
};
```

### 五规则

```cpp// C.21：如果必须管理资源，定义全部五个
class Buffer {
public:
    explicit Buffer(std::size_t size)
        : data_(std::make_unique<char[]>(size)), size_(size) {}

    ~Buffer() = default;

    Buffer(const Buffer& other)
        : data_(std::make_unique<char[]>(other.size_)), size_(other.size_) {
        std::copy_n(other.data_.get(), size_, data_.get());
    }

    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            auto new_data = std::make_unique<char[]>(other.size_);
            std::copy_n(other.data_.get(), other.size_, new_data.get());
            data_ = std::move(new_data);
            size_ = other.size_;
        }
        return *this;
    }

    Buffer(Buffer&&) noexcept = default;
    Buffer& operator=(Buffer&&) noexcept = default;

private:
    std::unique_ptr<char[]> data_;
    std::size_t size_;
};
```

### 类层次结构

```cpp
// C.35 + C.128：虚析构函数，使用override
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;  // C.121：纯接口
};

class Circle : public Shape {
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return 3.14159 * radius_ * radius_; }

private:
    double radius_;
};
```

### 反模式

- 在构造函数/析构函数中调用虚函数（C.82）
- 对非平凡类型使用`memset`/`memcpy`（C.90）
- 为虚函数和重写器提供不同的默认参数（C.140）
- 将数据成员设为`const`或引用，这会抑制移动/复制（C.12）

## 资源管理（R.*）

### 关键规则

| 规则 | 摘要 |
|------|---------|
| **R.1** | 使用RAII自动管理资源 |
| **R.3** | 原始指针（`T*`）是非拥有的 |
| **R.5** | 优先作用域对象；不必要地避免堆分配 |
| **R.10** | 避免`malloc()`/`free()` |
| **R.11** | 避免显式调用`new`和`delete` |
| **R.20** | 使用`unique_ptr`或`shared_ptr`表示所有权 |
| **R.21** | 除非共享所有权，否则优先`unique_ptr`而非`shared_ptr` |
| **R.22** | 使用`make_shared()`创建`shared_ptr` |

### 智能指针使用

```cpp
// R.11 + R.20 + R.21：使用智能指针的RAII
auto widget = std::make_unique<Widget>("config");  // 独占所有权
auto cache  = std::make_shared<Cache>(1024);        // 共享所有权

// R.3：原始指针 = 非拥有观察者
void render(const Widget* w) {  // 不拥有w
    if (w) w->draw();
}

render(widget.get());
```

### RAII模式

```cpp
// R.1：资源获取即初始化
class FileHandle {
public:
    explicit FileHandle(const std::string& path)
        : handle_(std::fopen(path.c_str(), "r")) {
        if (!handle_) throw std::runtime_error("Failed to open: " + path);
    }

    ~FileHandle() {
        if (handle_) std::fclose(handle_);
    }

    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    FileHandle(FileHandle&& other) noexcept
        : handle_(std::exchange(other.handle_, nullptr)) {}
    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (handle_) std::fclose(handle_);
            handle_ = std::exchange(other.handle_, nullptr);
        }
        return *this;
    }

private:
    std::FILE* handle_;
};
```

### 反模式

- 裸`new`/`delete`（R.11）
- 在C++代码中使用`malloc()`/`free()`（R.10）
- 在单个表达式中进行多次资源分配（R.13——异常安全危险）
- 在`unique_ptr`足够时使用`shared_ptr`（R.21）

## 表达式与语句（ES.*）

### 关键规则

| 规则 | 摘要 |
|------|---------|
| **ES.5** | 保持作用域小 |
| **ES.20** | 始终初始化对象 |
| **ES.23** | 优先`{}`初始化语法 |
| **ES.25** | 将对象声明为`const`或`constexpr`，除非打算修改 |
| **ES.28** | 使用lambda进行`const`变量的复杂初始化 |
| **ES.45** | 避免魔法常量；使用符号常量 |
| **ES.46** | 避免 narrowing/有损算术转换 |
| **ES.47** | 使用`nullptr`而非`0`或`NULL` |
| **ES.48** | 避免强制类型转换 |
| **ES.50** | 不要const_cast去掉const |

### 初始化

```cpp
// ES.20 + ES.23 + ES.25：始终初始化，优先{}，默认const
const int max_retries{3};
const std::string name{"widget"};
const std::vector<int> primes{2, 3, 5, 7, 11};

// ES.28：lambda用于复杂const初始化
const auto config = [&] {
    Config c;
    c.timeout = std::chrono::seconds{30};
    c.retries = max_retries;
    c.verbose = debug_mode;
    return c;
}();
```

### 反模式

- 未初始化变量（ES.20）
- 使用`0`或`NULL`作为指针（ES.47——使用`nullptr`）
- C风格强制类型转换（ES.48——使用`static_cast`、`const_cast`等）
- 去掉const的强制类型转换（ES.50）
- 魔法数字没有命名常量（ES.45）
- 混合有符号和无符号算术（ES.100）
- 在嵌套作用域中重用名称（ES.12）

## 错误处理（E.*）

### 关键规则

| 规则 | 摘要 |
|------|---------|
| **E.1** | 在设计早期制定错误处理策略 |
| **E.2** | 抛出异常以表示函数无法执行其分配的任务 |
| **E.6** | 使用RAII防止泄漏 |
| **E.12** | 当不可能或不可接受抛出时使用`noexcept` |
| **E.14** | 使用专门设计的用户定义类型作为异常 |
| **E.15** | 按值抛出，按引用捕获 |
| **E.16** | 析构函数、释放和swap绝不能失败 |
| **E.17** | 不要在每个函数中捕获每个异常 |

### 异常层次结构

```cpp
// E.14 + E.15：自定义异常类型，按值抛出，按引用捕获
class AppError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class NetworkError : public AppError {
public:
    NetworkError(const std::string& msg, int code)
        : AppError(msg), status_code(code) {}
    int status_code;
};

void fetch_data(const std::string& url) {
    // E.2：抛出以表示失败
    throw NetworkError("connection refused", 503);
}

void run() {
    try {
        fetch_data("https://api.example.com");
    } catch (const NetworkError& e) {
        log_error(e.what(), e.status_code);
    } catch (const AppError& e) {
        log_error(e.what());
    }
    // E.17：不要在这里捕获所有异常——让意外错误传播
}
```

### 反模式

- 抛出内置类型如`int`或字符串字面量（E.14）
- 按值捕获（切片风险）（E.15）
- 静默吞掉错误的空catch块
- 使用异常进行流程控制（E.3）
- 基于全局状态如`errno`的错误处理（E.28）

## 常量与不可变性（Con.*）

### 所有规则

| 规则 | 摘要 |
|------|---------|
| **Con.1** | 默认情况下，使对象不可变 |
| **Con.2** | 默认情况下，使成员函数为`const` |
| **Con.3** | 默认情况下，按`const`传递指针和引用 |
| **Con.4** | 用于构造后不改变的值使用`const` |
| **Con.5** | 用于可在编译时计算的值使用`constexpr` |

```cpp
// Con.1到Con.5：默认不可变性
class Sensor {
public:
    explicit Sensor(std::string id) : id_(std::move(id)) {}

    // Con.2：默认const成员函数
    const std::string& id() const { return id_; }
    double last_reading() const { return reading_; }

    // 仅在需要变异时非const
    void record(double value) { reading_ = value; }

private:
    const std::string id_;  // Con.4：构造后永不改变
    double reading_{0.0};
};

// Con.3：按const引用传递
void display(const Sensor& s) {
    std::cout << s.id() << ": " << s.last_reading() << '\n';
}

// Con.5：编译时常量
constexpr double PI = 3.14159265358979;
constexpr int MAX_SENSORS = 256;
```

## 并发与并行（CP.*）

### 关键规则

| 规则 | 摘要 |
|------|---------|
| **CP.2** | 避免数据竞争 |
| **CP.3** | 最小化可变数据的显式共享 |
| **CP.4** | 从任务角度思考，而非线程 |
| **CP.8** | 不要使用`volatile`进行同步 |
| **CP.20** | 使用RAII，绝不 plain `lock()`/`unlock()` |
| **CP.21** | 使用`std::scoped_lock`获取多个互斥锁 |
| **CP.22** | 持有锁时绝不调用未知代码 |
| **CP.42** | 不要无条件等待 |
| **CP.44** | 记住命名您的`lock_guard`和`unique_lock` |
| **CP.100** | 除非绝对必要，否则不要使用无锁编程 |

### 安全锁定

```cpp
// CP.20 + CP.44：RAII锁，始终命名
class ThreadSafeQueue {
public:
    void push(int value) {
        std::lock_guard<std::mutex> lock(mutex_);  // CP.44：命名！
        queue_.push(value);
        cv_.notify_one();
    }

    int pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        // CP.42：始终使用条件等待
        cv_.wait(lock, [this] { return !queue_.empty(); });
        const int value = queue_.front();
        queue_.pop();
        return value;
    }

private:
    std::mutex mutex_;             // CP.50：互斥锁与其数据一起
    std::condition_variable cv_;
    std::queue<int> queue_;
};
```

### 多个互斥锁

```cpp
// CP.21：std::scoped_lock用于多个互斥锁（无死锁）
void transfer(Account& from, Account& to, double amount) {
    std::scoped_lock lock(from.mutex_, to.mutex_);
    from.balance_ -= amount;
    to.balance_ += amount;
}
```

### 反模式

- 使用`volatile`进行同步（CP.8——它仅用于硬件I/O）
- 分离线程（CP.26——生命周期管理几乎不可能）
- 未命名的锁守卫：`std::lock_guard<std::mutex>(m);`立即销毁（CP.44）
- 持有锁时调用回调（CP.22——死锁风险）
- 无锁编程没有深度专业知识（CP.100）

## 模板与泛型编程（T.*）

### 关键规则

| 规则 | 摘要 |
|------|---------|
| **T.1** | 使用模板提高代码泛化性 |
| **T.2** | 用概念表达意图 |
| **T.3** | 优先概念而非易失性或命名类型约束 |
| **T.10** | 为每个模板参数指定约束 |
| **T.11** | 使用概念而非启用SFINAE |
| **T.12** | 优先范围而非迭代器 |
| **T.40** | 使用函数对象传递计算而非lambda |

### 概念定义

```cpp
// T.2 + T.10：使用概念表达模板约束
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<Numeric T>
T clamp(T value, T lo, T hi) {
    return std::clamp(value, lo, hi);
}
```

### 模板别名

```cpp
// T.4：使用模板别名简化语法
template<typename T>
using Vec = std::vector<T>;

Vec<int> ints;  // 替代 std::vector<int>
```

### 反模式

- 缺少概念约束的模板（T.10）
- 使用`typename`而非`concepts`（T.11）
- 在可能的情况下使用原始循环而非范围算法（T.12）

## 命名与布局（NL.*）

### 关键规则

| 规则 | 摘要 |
|------|---------|
| **NL.1** | 不要在名称中编码类型信息 |
| **NL.2** | 保持常量命名一致 |
| **NL.3** | 保持命名约定一致 |
| **NL.4** | 标识符应易于朗读 |
| **NL.5** | 保持命名风格一致 |
| **NL.6** | 使用下划线分隔单词 |
| **NL.7** | 使用小写字母开头用于变量和函数 |
| **NL.8** | 使用CamelCase用于类型名称 |
| **NL.9** | 所有大写用于宏 |
| **NL.10** | 避免使用可能混淆的名称 |

### 命名示例

```cpp
// NL.1 + NL.7 + NL.8 + NL.9：一致的命名
int num_elements;          // 变量：小写加下划线
constexpr int max_size{100};  // 常量：一致的小写

class Node {                // 类型：CamelCase
public:
    void add_child();       // 函数：小写加下划线
};

#define MAX_ELEMENTS 100    // 宏：全大写
```

## 总结

这些C++编码标准基于C++ Core Guidelines，旨在帮助您编写：

- **类型安全**：使用强类型系统防止错误
- **资源安全**：使用RAII管理所有资源
- **不可变**：默认使用const，使代码更容易推理
- **清晰**：表达意图，使代码自文档化
- **简洁**：避免不必要的复杂性
- **并发安全**：正确处理线程和数据竞争

遵循这些规则将帮助您编写更安全、更可靠、更易于维护的C++代码。
