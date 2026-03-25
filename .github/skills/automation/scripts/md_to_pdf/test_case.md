# Markdown 转 PDF 测试报告

## 1. 基础格式测试
这是一段普通文本，用于测试字体渲染。
**加粗文本**，*斜体文本*，`行内代码`。

### 代码块测试
```javascript
function hello() {
  console.log("Hello World");
}
```

## 2. Mermaid 图表测试

### 2.1 流程图 (Flowchart)
```mermaid
graph TD
    A[开始] --> B{判断}
    B -- 是 --> C[执行操作]
    B -- 否 --> D[结束]
    C --> D
```

### 2.2 时序图 (Sequence Diagram)
```mermaid
sequenceDiagram
    participant Alice
    participant Bob
    Alice->>John: Hello John, how are you?
    loop Healthcheck
        John->>John: Fight against hypochondria
    end
    Note right of John: Rational thoughts <br/>prevail!
    John-->>Alice: Great!
    John->>Bob: How about you?
    Bob-->>John: Jolly good!
```

### 2.3 甘特图 (Gantt)
```mermaid
gantt
    dateFormat  YYYY-MM-DD
    title Adding GANTT diagram to mermaid
    excludes weekdays 2014-01-10

    section A section
    Completed task            :done,    des1, 2014-01-06,2014-01-08
    Active task               :active,  des2, 2014-01-09, 3d
    Future task               :         des3, after des2, 5d
```

### 2.4 类图 (Class Diagram)
```mermaid
classDiagram
    Animal <|-- Duck
    Animal <|-- Fish
    Animal <|-- Zebra
    Animal : +int age
    Animal : +String gender
    Animal: +isMammal()
    Animal: +mate()
    class Duck{
        +String beakColor
        +swim()
        +quack()
    }
    class Fish{
        -int sizeInFeet
        -canEat()
    }
    class Zebra{
        +bool is_wild
        +run()
    }
```
