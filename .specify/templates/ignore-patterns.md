# Ignore Patterns Configuration

本文档定义各类技术栈的忽略文件模式，供 `speckit.implement` 阶段使用。

## 按技术栈分类

### Node.js / JavaScript / TypeScript

```
node_modules/
dist/
build/
*.log
.env*
coverage/
```

### Python

```
__pycache__/
*.pyc
.venv/
venv/
dist/
*.egg-info/
```

### Java

```
target/
*.class
*.jar
.gradle/
build/
```

### C# / .NET

```
bin/
obj/
*.user
*.suo
packages/
```

### Go

```
*.exe
*.test
vendor/
*.out
```

### Rust

```
target/
debug/
release/
*.rs.bk
*.rlib
*.prof*
```

### Kotlin

```
build/
out/
.gradle/
.idea/
*.class
*.jar
*.iml
```

### C / C++

```
build/
bin/
obj/
out/
*.o
*.so
*.a
*.exe
*.dll
```

### Swift

```
.build/
DerivedData/
*.swiftpm/
Packages/
```

### Ruby

```
.bundle/
log/
tmp/
*.gem
vendor/bundle/
```

### PHP

```
vendor/
*.log
*.cache
*.env
```

### R

```
.Rproj.user/
.Rhistory
.RData
.Ruserdata
*.Rproj
packrat/
renv/
```

## 通用模式

```
.DS_Store
Thumbs.db
*.tmp
*.swp
.vscode/
.idea/
```

## 按工具分类

### Docker

```
node_modules/
.git/
Dockerfile*
.dockerignore
*.log*
.env*
coverage/
```

### ESLint

```
node_modules/
dist/
build/
coverage/
*.min.js
```

### Prettier

```
node_modules/
dist/
build/
coverage/
package-lock.json
yarn.lock
pnpm-lock.yaml
```

### Terraform

```
.terraform/
*.tfstate*
*.tfvars
.terraform.lock.hcl
```

### Kubernetes / K8s

```
*.secret.yaml
secrets/
.kube/
kubeconfig*
*.key
*.crt
```

## 使用说明

1. 根据项目技术栈选择对应模式
2. 检查 `.gitignore` 是否已存在，如有则仅追加缺失模式
3. 不要覆盖用户已有的自定义忽略规则
