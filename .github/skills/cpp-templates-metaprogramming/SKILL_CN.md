---
name: cpp-templates-metaprogramming
user-invocable: false
description: 用于创建基于模板、SFINAE、概念和编译期元编程的泛型且类型安全的 C++ 库。
allowed-tools:
  - Bash
  - Read
  - Write
  - Edit
---

# C++ 模板与元编程

掌握 C++ 模板、模板元编程、SFINAE、概念以及编译期计算。本技能使您能够创建具有编译期保证的泛型、类型安全且高效的 C++ 库。

## 函数模板

### 基本函数模板

```cpp
// 简单的函数模板
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// 使用示例
int i = max(10, 20);           // T = int
double d = max(3.14, 2.71);    // T = double
// auto x = max(10, 3.14);     // 错误：无法推导 T

// 多个模板参数
template<typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

auto result = add(5, 3.14);    // T = int, U = double, 返回 double

// C++14：更简洁的返回类型推导
template<typename T, typename U>
auto multiply(T a, U b) {
    return a * b;
}
```

### 模板特化

```cpp
// 主模板
template<typename T>
T absolute(T value) {
    return (value < 0) ? -value : value;
}

// 针对 const char* 的全特化
template<>
const char* absolute<const char*>(const char* value) {
    return value; // 字符串没有绝对值
}

// 针对 std::string 的全特化
template<>
std::string absolute<std::string>(std::string value) {
    return value;
}

// 使用示例
int a = absolute(-5);              // 使用主模板
const char* b = absolute("test");  // 使用 const char* 特化
```

### 函数模板重载

```cpp
// 重载 1：泛型模板
template<typename T>
void print(T value) {
    std::cout << "泛型: " << value << std::endl;
}

// 重载 2：指针特化
template<typename T>
void print(T* ptr) {
    std::cout << "指针: " << *ptr << std::endl;
}

// 重载 3：非模板重载
void print(const char* str) {
    std::cout << "字符串: " << str << std::endl;
}

// 使用示例
int x = 42;
print(x);       // 重载 1
print(&x);      // 重载 2
print("hello"); // 重载 3（精确匹配优先）
```

## 类模板

### 基本类模板

```cpp
// 简单的类模板
template<typename T>
class Container {
    T value;

public:
    Container(T v) : value(v) {}

    T get() const { return value; }
    void set(T v) { value = v; }
};

// 使用示例
Container<int> intContainer(42);
Container<std::string> strContainer("hello");

// 多个模板参数
template<typename K, typename V>
class KeyValuePair {
    K key;
    V value;

public:
    KeyValuePair(K k, V v) : key(k), value(v) {}

    K getKey() const { return key; }
    V getValue() const { return value; }
};

KeyValuePair<std::string, int> pair("answer", 42);
```

### 模板成员函数

```cpp
template<typename T>
class Array {
    T* data;
    size_t size;

public:
    Array(size_t s) : size(s), data(new T[s]) {}
    ~Array() { delete[] data; }

    // 模板成员函数
    template<typename Func>
    void forEach(Func func) {
        for (size_t i = 0; i < size; ++i) {
            func(data[i]);
        }
    }

    // 模板转换运算符
    template<typename U>
    operator Array<U>() const {
        Array<U> result(size);
        for (size_t i = 0; i < size; ++i) {
            result.data[i] = static_cast<U>(data[i]);
        }
        return result;
    }
};

// 使用示例
Array<int> arr(5);
arr.forEach([](int& x) { x *= 2; });
```

### 类模板特化

```cpp
// 主模板
template<typename T>
class Storage {
    T data;

public:
    Storage(T d) : data(d) {}
    T get() const { return data; }
};

// 针对指针的全特化
template<typename T>
class Storage<T*> {
    T* data;

public:
    Storage(T* d) : data(d) {}
    T* get() const { return data; }
    T& operator*() { return *data; }
};

// 针对 bool 的全特化（位优化）
template<>
class Storage<bool> {
    unsigned char data : 1;

public:
    Storage(bool d) : data(d) {}
    bool get() const { return data; }
};
```

### 类模板偏特化

```cpp
// 主模板
template<typename T, typename U>
class Pair {
public:
    T first;
    U second;
    void info() { std::cout << "泛型 pair" << std::endl; }
};

// 偏特化：两个类型相同
template<typename T>
class Pair<T, T> {
public:
    T first;
    T second;
    void info() { std::cout << "相同类型 pair" << std::endl; }
};

// 偏特化：第二个类型是指针
template<typename T, typename U>
class Pair<T, U*> {
public:
    T first;
    U* second;
    void info() { std::cout << "第二个是指针" << std::endl; }
};

// 使用示例
Pair<int, double> p1;    // 泛型
Pair<int, int> p2;       // 相同类型
Pair<int, double*> p3;   // 第二个是指针
```

## 模板参数

### 类型参数

```cpp
// 单个类型参数
template<typename T>
class Vector {
    T* data;
};

// 多个类型参数
template<typename T, typename Allocator>
class CustomVector {
    T* data;
    Allocator alloc;
};

// 默认类型参数
template<typename T, typename Compare = std::less<T>>
class Set {
    Compare comp;
public:
    bool less(const T& a, const T& b) {
        return comp(a, b);
    }
};
```

### 非类型参数

```cpp
// 整数非类型参数
template<typename T, size_t N>
class Array {
    T data[N];

public:
    constexpr size_t size() const { return N; }

    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }
};

Array<int, 10> arr1;  // 10 个 int 的数组
Array<double, 5> arr2; // 5 个 double 的数组

// 布尔非类型参数
template<typename T, bool IsSorted>
class Container {
public:
    void insert(T value) {
        if constexpr (IsSorted) {
            insert_sorted(value);
        } else {
            insert_unsorted(value);
        }
    }

private:
    void insert_sorted(T value) { /* ... */ }
    void insert_unsorted(T value) { /* ... */ }
};

// 指针非类型参数（C++17）
template<auto* Ptr>
class StaticWrapper {
public:
    auto& get() { return *Ptr; }
};
```

### 模板模板参数

```cpp
// 模板模板参数
template<typename T, template<typename> class Container>
class Stack {
    Container<T> data;

public:
    void push(const T& value) {
        data.push_back(value);
    }

    T pop() {
        T value = data.back();
        data.pop_back();
        return value;
    }
};

// 使用示例
Stack<int, std::vector> intStack;
Stack<double, std::deque> doubleStack;

// 带多个参数的模板模板参数
template<typename T,
         template<typename, typename> class Container,
         typename Allocator = std::allocator<T>>
class AdvancedStack {
    Container<T, Allocator> data;
};
```

## 可变参数模板

### 参数包

```cpp
// 基本可变参数模板
template<typename... Args>
void print(Args... args) {
    (std::cout << ... << args) << std::endl; // C++17 折叠表达式
}

print(1, 2, 3, "hello", 3.14);

// 获取包大小
template<typename... Args>
constexpr size_t count(Args... args) {
    return sizeof...(args);
}

size_t n = count(1, 2, 3, 4); // 4

// 递归处理参数包（C++17 之前）
template<typename T>
void print_recursive(T value) {
    std::cout << value << std::endl;
}

template<typename T, typename... Args>
void print_recursive(T first, Args... rest) {
    std::cout << first << " ";
    print_recursive(rest...); // 递归调用
}
```

### 折叠表达式（C++17）

```cpp
// 一元右折叠：(args op ...)
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);
}

auto result = sum(1, 2, 3, 4, 5); // 15

// 一元左折叠：(... op args)
template<typename... Args>
auto sum_left(Args... args) {
    return (... + args);
}

// 二元右折叠：(args op ... op init)
template<typename... Args>
auto sum_with_init(Args... args) {
    return (args + ... + 0);
}

// 二元左折叠：(init op ... op args)
template<typename... Args>
auto multiply_with_init(Args... args) {
    return (1 * ... * args);
}

// 逻辑折叠表达式
template<typename... Args>
bool all_true(Args... args) {
    return (args && ...);
}

template<typename... Args>
bool any_true(Args... args) {
    return (args || ...);
}

// 逗号折叠用于副作用
template<typename... Args>
void print_all(Args... args) {
    (std::cout << ... << args) << std::endl;
}
```

### 可变参数类模板

```cpp
// 类元组
template<typename... Types>
class Tuple;

// 基本情况：空元组
template<>
class Tuple<> {};

// 递归情况
template<typename Head, typename... Tail>
class Tuple<Head, Tail...> : private Tuple<Tail...> {
    Head value;

public:
    Tuple(Head h, Tail... t) : Tuple<Tail...>(t...), value(h) {}

    Head& head() { return value; }
    Tuple<Tail...>& tail() { return *this; }
};

// 使用示例
Tuple<int, double, std::string> t(42, 3.14, "hello");

// 带完美转发的可变参数模板
template<typename... Args>
auto make_unique_custom(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

## SFINAE（替换失败不是错误）

### 基本 SFINAE

```cpp
// 当类型有 begin() 方法时启用
template<typename T>
auto process(T container) -> decltype(container.begin(), void()) {
    std::cout << "有 begin() 的容器" << std::endl;
}

// 当类型是算术类型时启用
template<typename T>
auto process(T value)
    -> typename std::enable_if<std::is_arithmetic<T>::value>::type {
    std::cout << "算术类型" << std::endl;
}

// 使用示例
std::vector<int> vec;
process(vec);    // 第一个重载
process(42);     // 第二个重载
```

### std::enable_if

```cpp
// 仅对整型启用函数
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
increment(T value) {
    return value + 1;
}

// C++14：使用 enable_if_t 的更简洁语法
template<typename T>
std::enable_if_t<std::is_integral<T>::value, T>
decrement(T value) {
    return value - 1;
}

// 作为模板参数（C++14）
template<typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
T half(T value) {
    return value / 2;
}

// 多个 enable_if 条件
template<typename T>
std::enable_if_t<std::is_pointer<T>::value &&
                 !std::is_const<std::remove_pointer_t<T>>::value, void>
modify(T ptr) {
    *ptr = {};
}
```

### 标签分发

```cpp
// 带标签的实现函数
template<typename Iterator>
void advance_impl(Iterator& it, int n, std::random_access_iterator_tag) {
    it += n; // 随机访问：O(1)
}

template<typename Iterator>
void advance_impl(Iterator& it, int n, std::input_iterator_tag) {
    while (n--) ++it; // 输入迭代器：O(n)
}

// 分发函数
template<typename Iterator>
void advance(Iterator& it, int n) {
    advance_impl(it, n,
        typename std::iterator_traits<Iterator>::iterator_category());
}
```

### if constexpr（C++17）

```cpp
// 替代许多 SFINAE 用例
template<typename T>
auto process(T value) {
    if constexpr (std::is_integral_v<T>) {
        return value * 2;
    } else if constexpr (std::is_floating_point_v<T>) {
        return value * 3.14;
    } else if constexpr (std::is_pointer_v<T>) {
        return *value;
    } else {
        return value;
    }
}

// 带 if constexpr 的可变参数模板
template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
        print(rest...);
    } else {
        std::cout << std::endl;
    }
}
```

## 概念（C++20）

### 定义概念

```cpp
#include <concepts>

// 简单概念
template<typename T>
concept Integral = std::is_integral_v<T>;

// 带 requires 表达式的概念
template<typename T>
concept Incrementable = requires(T x) {
    { ++x } -> std::same_as<T&>;
    { x++ } -> std::same_as<T>;
};

// 复合概念
template<typename T>
concept Number = std::is_arithmetic_v<T>;

template<typename T>
concept SignedNumber = Number<T> && std::is_signed_v<T>;

// 带多个要求的概念
template<typename T>
concept Container = requires(T c) {
    typename T::value_type;
    typename T::iterator;
    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
    { c.size() } -> std::convertible_to<std::size_t>;
};
```

### 使用概念

```cpp
// 约束函数模板
template<Integral T>
T add(T a, T b) {
    return a + b;
}

// requires 子句
template<typename T>
    requires Integral<T>
T multiply(T a, T b) {
    return a * b;
}

// 缩写函数模板（C++20）
auto divide(Integral auto a, Integral auto b) {
    return a / b;
}

// 约束类模板
template<Container C>
class Processor {
    C container;
public:
    void process() {
        for (auto& item : container) {
            // 处理元素
        }
    }
};

// 多个约束
template<typename T>
    requires std::is_arithmetic_v<T> && std::is_signed_v<T>
T absolute(T value) {
    return value < 0 ? -value : value;
}
```

### 概念特化

```cpp
// 基于概念的不同实现
template<typename T>
void process(T value) {
    if constexpr (Integral<T>) {
        std::cout << "处理整数: " << value << std::endl;
    } else if constexpr (std::floating_point<T>) {
        std::cout << "处理浮点数: " << value << std::endl;
    } else {
        std::cout << "处理其他类型: " << value << std::endl;
    }
}

// 基于概念的重载
void handle(Integral auto value) {
    std::cout << "整型: " << value << std::endl;
}

void handle(std::floating_point auto value) {
    std::cout << "浮点: " << value << std::endl;
}

void handle(Container auto container) {
    std::cout << "容器大小: " << container.size() << std::endl;
}
```

## 类型特征

### 标准类型特征

```cpp
#include <type_traits>

// 类型属性
static_assert(std::is_integral_v<int>);
static_assert(std::is_floating_point_v<double>);
static_assert(std::is_pointer_v<int*>);
static_assert(std::is_array_v<int[5]>);
static_assert(std::is_const_v<const int>);

// 类型关系
static_assert(std::is_same_v<int, int>);
static_assert(std::is_base_of_v<Base, Derived>);
static_assert(std::is_convertible_v<int, double>);

// 类型修改
using NoConst = std::remove_const_t<const int>;      // int
using NoRef = std::remove_reference_t<int&>;         // int
using NoPointer = std::remove_pointer_t<int*>;       // int
using AddConst = std::add_const_t<int>;              // const int
using AddPointer = std::add_pointer_t<int>;          // int*

// 条件类型
using Type = std::conditional_t<true, int, double>;  // int
```

### 自定义类型特征

```cpp
// 检查类型是否有 size() 方法
template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};

template<typename T>
inline constexpr bool has_size_v = has_size<T>::value;

// 使用示例
static_assert(has_size_v<std::vector<int>>);
static_assert(!has_size_v<int>);

// 检查类型是否可迭代
template<typename T, typename = void>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<T, std::void_t<
    decltype(std::declval<T>().begin()),
    decltype(std::declval<T>().end())>>
    : std::true_type {};

template<typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;
```

## 模板元编程

### 编译期计算

```cpp
// 编译期计算阶乘
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

constexpr int fact5 = Factorial<5>::value; // 120

// 编译期计算斐波那契数列
template<int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template<>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

template<>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

constexpr int fib10 = Fibonacci<10>::value; // 55
```

### 类型列表

```cpp
// 类型列表定义
template<typename... Types>
struct TypeList {};

// 获取类型列表大小
template<typename List>
struct Length;

template<typename... Types>
struct Length<TypeList<Types...>> {
    static constexpr size_t value = sizeof...(Types);
};

// 获取指定索引处的类型
template<size_t Index, typename List>
struct At;

template<size_t Index, typename Head, typename... Tail>
struct At<Index, TypeList<Head, Tail...>>
    : At<Index - 1, TypeList<Tail...>> {};

template<typename Head, typename... Tail>
struct At<0, TypeList<Head, Tail...>> {
    using type = Head;
};

// 使用示例
using MyList = TypeList<int, double, char, std::string>;
static_assert(Length<MyList>::value == 4);
using SecondType = At<1, MyList>::type; // double
```

### CRTP（奇异递归模板模式）

```cpp
// 通过 CRTP 实现静态多态
template<typename Derived>
class Base {
public:
    void interface() {
        static_cast<Derived*>(this)->implementation();
    }

    void common_functionality() {
        std::cout << "公共代码" << std::endl;
    }
};

class Derived1 : public Base<Derived1> {
public:
    void implementation() {
        std::cout << "Derived1 实现" << std::endl;
    }
};

class Derived2 : public Base<Derived2> {
public:
    void implementation() {
        std::cout << "Derived2 实现" << std::endl;
    }
};

// 使用示例
template<typename T>
void use(Base<T>& obj) {
    obj.interface(); // 无虚函数开销
}

Derived1 d1;
Derived2 d2;
use(d1); // Derived1 实现
use(d2); // Derived2 实现
```

### 表达式模板

```cpp
// 用于惰性求值的表达式模板
template<typename E>
class VecExpression {
public:
    double operator[](size_t i) const {
        return static_cast<const E&>(*this)[i];
    }

    size_t size() const {
        return static_cast<const E&>(*this).size();
    }
};

class Vec : public VecExpression<Vec> {
    std::vector<double> data;

public:
    Vec(size_t n) : data(n) {}

    double& operator[](size_t i) { return data[i]; }
    double operator[](size_t i) const { return data[i]; }
    size_t size() const { return data.size(); }

    template<typename E>
    Vec& operator=(const VecExpression<E>& expr) {
        for (size_t i = 0; i < size(); ++i) {
            data[i] = expr[i];
        }
        return *this;
    }
};

// 加法表达式
template<typename E1, typename E2>
class VecSum : public VecExpression<VecSum<E1, E2>> {
    const E1& lhs;
    const E2& rhs;

public:
    VecSum(const E1& l, const E2& r) : lhs(l), rhs(r) {}

    double operator[](size_t i) const {
        return lhs[i] + rhs[i];
    }

    size_t size() const { return lhs.size(); }
};

// 运算符重载
template<typename E1, typename E2>
VecSum<E1, E2> operator+(const VecExpression<E1>& lhs,
                          const VecExpression<E2>& rhs) {
    return VecSum<E1, E2>(static_cast<const E1&>(lhs),
                          static_cast<const E2&>(rhs));
}

// 使用示例：单循环求值
Vec v1(1000), v2(1000), v3(1000), v4(1000);
v4 = v1 + v2 + v3; // 高效：无临时向量
```

## constexpr 与 consteval

### constexpr 函数

```cpp
// constexpr 函数（可在编译期或运行期执行）
constexpr int square(int n) {
    return n * n;
}

constexpr int value1 = square(5);  // 编译期
int x = 5;
int value2 = square(x);            // 运行期

// 带复杂逻辑的 constexpr（C++14+）
constexpr int fibonacci(int n) {
    if (n <= 1) return n;

    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// 带 std::array 的 constexpr
constexpr auto make_array() {
    std::array<int, 5> arr{};
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = i * i;
    }
    return arr;
}

constexpr auto squares = make_array();
```

### consteval 函数（C++20）

```cpp
// 必须在编译期求值
consteval int cube(int n) {
    return n * n * n;
}

constexpr int value3 = cube(5);  // OK：编译期
// int y = 5;
// int value4 = cube(y);         // 错误：不是编译期

// is_constant_evaluated
constexpr int conditional_compute(int n) {
    if (std::is_constant_evaluated()) {
        // 编译期路径
        return n * n;
    } else {
        // 运行期路径（可能使用硬件指令）
        return n * n; // 可以使用内联汇编
    }
}
```

## 模板调试

### 编译期调试

```cpp
// 在编译期打印类型（通过产生包含类型信息的错误）
template<typename T>
struct DebugType;

// DebugType<decltype(value)> debug; // 错误会显示类型

// 用于调试的 static_assert
template<typename T>
void check_type(T value) {
    static_assert(std::is_integral_v<T>, "T 必须是整型");
    static_assert(sizeof(T) >= 4, "T 必须至少 4 字节");
}

// 用于更好错误信息的概念
template<typename T>
concept AtLeast4Bytes = sizeof(T) >= 4;

template<AtLeast4Bytes T>
void process(T value) {
    // 如果 T 不满足概念，会显示清晰的错误信息
}
```

### 模板错误简化

```cpp
// C++20 之前：晦涩的错误
template<typename T>
void old_process(T value) {
    value.size(); // 如果 T 没有 size() 会报错
}

// C++20：清晰的概念化错误
template<typename T>
concept HasSize = requires(T t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

template<HasSize T>
void new_process(T value) {
    value.size(); // 如果 T 不满足 HasSize 会显示清晰错误
}

// 尽早使用 static_assert
template<typename T>
void checked_process(T value) {
    static_assert(HasSize<T>, "T 必须有 size() 方法");
    value.size();
}
```

## 最佳实践

1. **优先使用概念而非 SFINAE（C++20）**：错误信息更清晰，约束更易读
2. **使用类型特征进行类型检查**：利用 std::is_same、std::is_integral 等
3. **优先使用 constexpr 而非模板元编程**：更易读、更易调试
4. **使用 if constexpr 进行条件编译**：替代许多 SFINAE 用例
5. **避免深层模板递归**：可能导致编译时间过长和错误
6. **谨慎使用缩写函数模板**：可能隐藏重要的类型信息
7. **提供清晰的错误信息**：使用 static_assert 或概念来引导用户
8. **使用 std::forward 进行完美转发**：在模板代码中保持值类别
9. **使用可变参数模板实现灵活接口**：优于重载集合
10. **记录模板要求**：明确指定类型必须支持的操作

## 常见陷阱

1. **两阶段查找问题**：模板中的名称查找行为不同
2. **依赖名称解析**：必须正确使用 typename 和 template 关键字
3. **模板实例化膨胀**：每次实例化都会创建新代码
4. **编译期爆炸**：复杂的元编程可能导致编译时间过长
5. **晦涩的错误信息**：模板错误可能难以理解
6. **缺少 typename 关键字**：依赖类型名需要 typename
7. **缺少 template 关键字**：依赖模板名需要 template
8. **忘记 std::forward**：破坏完美转发
9. **概念包含问题**：更具体的概念必须包含较不具体的概念
10. **constexpr 限制**：并非所有操作都允许在 constexpr 上下文中

## 使用场景

在以下情况下使用本技能：

- 创建泛型算法和数据结构
- 构建可复用的库代码
- 实现编译期计算
- 使用概念约束模板参数
- 执行类型内省和操作
- 通过零开销抽象优化性能
- 创建领域特定嵌入式语言（DSEL）
- 使用 CRTP 实现静态多态
- 构建表达式模板库
- 教授或学习高级 C++ 技术

## 参考资料

- [C++ Reference - Templates](https://en.cppreference.com/w/cpp/language/templates)
- [C++ Reference - SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)
- [C++ Reference - Concepts](https://en.cppreference.com/w/cpp/language/constraints)
- [C++ Reference - Type Traits](https://en.cppreference.com/w/cpp/header/type_traits)
- [C++ Reference - Fold Expressions](https://en.cppreference.com/w/cpp/language/fold)
- [C++ Reference - Constexpr](https://en.cppreference.com/w/cpp/language/constexpr)
- [C++ Templates: The Complete Guide by Vandevoorde, Josuttis, Gregor](http://www.tmplbook.com/)
- [Modern C++ Design by Andrei Alexandrescu](https://erdani.com/index.php/books/modern-c-design/)
- [CppCon Talks on Template Metaprogramming](https://www.youtube.com/user/CppCon)
