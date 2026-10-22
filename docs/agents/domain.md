# Domain Docs

工程技能在探索本仓库时如何消费领域文档。

## 探索之前先读

- **`GLOSSARY.md`**（仓库根目录），或
- **`GLOSSARY-MAP.md`**（仓库根目录，如果存在）：它指向每个 context 的 `GLOSSARY.md`。读取与当前主题相关的每一个。
- **`docs/adr/`**：读涉及你即将改动区域的 ADR。多上下文仓库还要检查 `src/<context>/docs/adr/` 中的 context 级决策。

若这些文件不存在，**静默继续**。不要指出它们的缺失，也不要主动建议创建。`/domain-modeling` 技能（经由 `/grill-with-docs` 和 `/improve-codebase-architecture` 到达）会在术语或决策真正确定时按需创建它们。

## 文件结构

单上下文仓库（大多数仓库）：

```
/
├── GLOSSARY.md
├── docs/adr/
│   ├── 0001-event-sourced-orders.md
│   └── 0002-postgres-for-write-model.md
└── src/
```

多上下文仓库（根目录存在 `GLOSSARY-MAP.md`）：

```
/
├── GLOSSARY-MAP.md
├── docs/adr/                          ← 系统级决策
└── src/
    ├── ordering/
    │   ├── GLOSSARY.md
    │   └── docs/adr/                  ← context 级决策
    └── billing/
        ├── GLOSSARY.md
        └── docs/adr/
```

## 使用术语表词汇

当输出中出现领域概念（issue 标题、重构建议、假设、测试名）时，使用 `GLOSSARY.md` 中定义的术语。不要漂移到术语表明确避免的同义词。

如果需要概念尚未进入术语表，这是一个信号：要么你在发明项目不使用的语言（重新考虑），要么存在真实缺口（记为 `/domain-modeling` 的事项）。

## 标记 ADR 冲突

若输出与现有 ADR 矛盾，明确揭示，而不是静默覆盖：

> _与 ADR-0007（event-sourced orders）矛盾，但值得重开，因为……_
