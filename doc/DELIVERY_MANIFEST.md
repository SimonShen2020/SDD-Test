# Delivery Manifest - v0.1.0

发布日期：2026-03-25

## 1. 可执行与动态库产物（Debug）

- build/src/core/Debug/CameraCore.dll
- build/src/core/Debug/CameraCore.lib
- build/src/plugins/null/Debug/CameraNull.dll
- build/src/plugins/huarui/Debug/CameraHuarui.dll
- build/tests/Debug/camera_core_tests.exe

## 2. 配置与说明文档

- doc/camera-config-3x8.xml
- doc/README-camera-config.md

## 3. 规格工件（SDD）

- specs/main/spec.md
- specs/main/research.md
- specs/main/plan.md
- specs/main/tasks.md
- specs/main/quickstart.md
- specs/main/data-model.md
- specs/main/contracts/core-api.md
- specs/main/analyze.md
- specs/main/review.md

## 4. 发布文档

- doc/VERSION
- doc/CHANGELOG.md
- doc/RELEASE_NOTES.md
- doc/RELEASE_CHECKLIST.md
- doc/README.md
- doc/RISK_REGISTER.md

## 5. 验收基线

- 构建命令：cmake -S . -B build; cmake --build build --config Debug
- 测试命令：ctest --test-dir build -C Debug --output-on-failure
- 验收结果：15/15 通过
