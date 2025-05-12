# 生命苹果（Life Apple）

灵感来自于「生命水晶」，它是一种可以提升玩家最大生命值的物品。

这是一个用于演示的项目，主要展示如何使用 `AttributeModifier` 实现一个简单的血量提升功能。

## 功能

* 玩家可以食用苹果，每次食用会永久提升 **1 点最大生命值**。
* 最大生命值的提升上限为 **20 点**。

## 实现

* **核心逻辑：** 全部实现于 `src\mod\LifeApple.cpp` 文件中。
* **事件钩子：**

  * `PlayerEat`：处理玩家吃苹果的逻辑。
  * `SharedAttributesLoadAttirbute`：修复玩家重新进入游戏后，血量超过 20 会被 BDS 重置的问题。

## 安装

```bash
lip install github.com/Pixovate/LifeApple
```
## 协议

MIT License © Dofes