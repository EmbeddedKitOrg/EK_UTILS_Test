# Issue tracker: GitHub

本仓库的 issue 与 spec 存放在 GitHub Issues。所有操作使用 `gh` CLI。

## 约定

- **创建 issue**：`gh issue create --title "..." --body "..."`。多行正文用 heredoc。
- **读取 issue**：`gh issue view <number> --comments`，用 `jq` 过滤评论，并同时取 labels。
- **列出 issue**：`gh issue list --state open --json number,title,body,labels,comments --jq '[.[] | {number, title, body, labels: [.labels[].name], comments: [.comments[].body]}]'`，按需加 `--label` / `--state` 过滤。
- **评论**：`gh issue comment <number> --body "..."`
- **加/删标签**：`gh issue edit <number> --add-label "..."` / `--remove-label "..."`
- **关闭**：`gh issue close <number> --comment "..."`

仓库由 `git remote -v` 推断；在 clone 内运行时 `gh` 会自动识别。

## PR 作为 triage 入口

**PRs as a request surface: no.**（若本仓库把外部 PR 当作 feature request，改为 `yes`；`/triage` 会读这个标记。）

设为 `yes` 时，PR 走与 issue 相同的标签和状态，用 `gh pr` 对应命令：

- **读取 PR**：`gh pr view <number> --comments`，diff 用 `gh pr diff <number>`。
- **列出待 triage 的外部 PR**：`gh pr list --state open --json number,title,body,labels,author,authorAssociation,comments`，只保留 `authorAssociation` 为 `CONTRIBUTOR`、`FIRST_TIME_CONTRIBUTOR`、`NONE` 的项（丢弃 `OWNER`/`MEMBER`/`COLLABORATOR`）。
- **评论 / 打标签 / 关闭**：`gh pr comment`、`gh pr edit --add-label`/`--remove-label`、`gh pr close`。

GitHub 的 issue 与 PR 共用一个编号空间，裸 `#42` 可能是两者之一：先 `gh pr view 42`，失败再 `gh issue view 42`。

## 技能说"发布到 issue tracker"时

创建 GitHub issue。

## 技能说"取出相关 ticket"时

运行 `gh issue view <number> --comments`。

## Wayfinding 操作

供 `/wayfinder` 使用。**map** 是一个带子 issue 的 issue，**child** issue 即 ticket。

- **Map**：单个 issue，标签 `wayfinder:map`，正文包含 Notes / Decisions-so-far / Fog。`gh issue create --label wayfinder:map`。
- **Child ticket**：通过 GitHub sub-issue 关联到 map（`gh api` 调 sub-issues endpoint）。未启用 sub-issue 时，在 map 正文里加 task list，并在 child 正文顶部写 `Part of #<map>`。标签：`wayfinder:<type>`（`research`/`prototype`/`grilling`/`task`）。认领后指派给推进的开发者。
- **Blocking**：使用 GitHub 原生 issue dependencies，这是规范且 UI 可见的表示。用 `gh api --method POST repos/<owner>/<repo>/issues/<child>/dependencies/blocked_by -F issue_id=<blocker-db-id>` 加边，`<blocker-db-id>` 是阻塞者数字 **database id**（`gh api repos/<owner>/<repo>/issues/<n> --jq .id`，不是 `#number` 或 `node_id`）。GitHub 用 `issue_dependencies_summary.blocked_by` 报告（仅未关闭的阻塞者，即实时闸门）。dependencies 不可用时回退为 child 正文顶部的 `Blocked by: #<n>, #<n>` 行。所有阻塞者都关闭后 ticket 即解除阻塞。
- **Frontier query**：列出 map 的未关闭 child（`gh issue list --state open`，限定在 map 的 sub-issue / task list 内），剔除有未关闭阻塞者（`issue_dependencies_summary.blocked_by > 0`，或 `Blocked by` 行内有未关闭 issue）或已有 assignee 的项；map 顺序最先者胜。
- **认领**：`gh issue edit <n> --add-assignee @me`，这是会话的第一次写操作。
- **解决**：`gh issue comment <n> --body "<answer>"`，再 `gh issue close <n>`，最后把上下文指针（要点 + 链接）追加到 map 的 Decisions-so-far。
