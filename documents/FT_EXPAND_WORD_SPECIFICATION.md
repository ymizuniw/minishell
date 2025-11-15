# ft_expand_word() 関数 - 要件定義書と詳細設計書

## プロジェクト概要

### プロジェクト名
**ft_expand_word (Word Expansion Engine)**

### 目的
パーサーから渡されたトークン配列を受け取り、変数展開（ドル記号`$`）とワイルドカード展開（`*`, `?`）を実行し、最終的なコマンド引数配列（`char **argv`）を生成する。

### スコープ
- トークン値の単語単位への分解（`gen_word()`）
- ドル記号変数展開（`$VAR`, `$?`等）
- ワイルドカードパターンマッチング
- クォート処理（シングル/ダブル）
- 複数トークンから単一`argv`配列への統合

---

## 1. 要件定義

### 1.1 機能要件

#### FR-1: トークンから単語リスト生成
- **ID**: FR-EW-001
- **優先度**: 高
- **説明**: 各トークンの値文字列を解析し、展開可能な単語単位（`t_word`構造体リスト）に分解する
- **入力**: トークン配列 `t_token **tokens`, トークン数 `size_t token_count`
- **出力**: 各トークンに対応する単語リスト `t_word *`
- **受け入れ基準**:
  - シングルクォート内は展開なし（リテラル扱い）
  - ダブルクォート内はドル展開のみ実行
  - クォート外はドル展開とワイルドカード展開両方を実行
  - 空トークンは空文字列として扱う

#### FR-2: ドル記号変数展開
- **ID**: FR-EW-002
- **優先度**: 高
- **説明**: `$VAR`形式の変数を環境変数の値に展開する、`$?`は終了ステータスに展開する
- **入力**: 単語 `t_word *` とシェル状態 `t_shell *`
- **出力**: 展開後の文字列
- **受け入れ基準**:
  - `$HOME` → `/home/user` （環境変数の値）
  - `$?` → `0` （最後の終了ステータス）
  - `$UNDEFINED` → `""` （未定義変数は空文字列）
  - シングルクォート内の`$`は展開しない
  - ダブルクォート内の`$`は展開する
  - `$` 単独、または`$` + 無効文字はリテラル扱い

#### FR-3: ワイルドカード展開
- **ID**: FR-EW-003
- **優先度**: 高
- **説明**: `*` と `?` パターンをファイル名にマッチングし、展開する
- **入力**: パターン文字列、検索パス
- **出力**: マッチしたファイル名の配列
- **受け入れ基準**:
  - `*.c` → `["main.c", "utils.c"]` （カレントディレクトリの.cファイル）
  - `test?.txt` → `["test1.txt", "test2.txt"]`
  - マッチ無しの場合はパターンそのものを返す
  - クォート内のワイルドカードは展開しない
  - ドット開始ファイル（`.hidden`）は`.*`明示時のみマッチ

#### FR-4: 複数トークンの統合
- **ID**: FR-EW-004
- **優先度**: 高
- **説明**: 各トークンの展開結果を単一の`char **`配列に統合する
- **入力**: トークンごとの展開結果配列 `char ***token_results`, 結果数配列 `size_t *token_result_counts`
- **出力**: 統合された`char **argv` （NULL終端）
- **受け入れ基準**:
  - トークン数3、各トークンが1, 2, 1個に展開 → argv長5（+ NULL）
  - ワイルドカード展開で複数ファイルにマッチした場合、全て含める
  - 正しい順序を保持（トークン順、展開順）

#### FR-5: メモリ管理
- **ID**: FR-EW-005
- **優先度**: 高
- **説明**: 動的確保したメモリを適切に管理し、リークを防ぐ
- **受け入れ基準**:
  - 成功時: 結果配列のみを返す、中間データは全て解放
  - 失敗時: 全ての確保済みメモリを解放し、NULLを返す
  - Valgrind検証で0 bytes lost

### 1.2 非機能要件

#### NFR-1: パフォーマンス
- **ID**: NFR-EW-001
- **優先度**: 中
- **説明**: 通常のコマンドライン（～100トークン、～1000ファイル）を10ms以内で処理
- **測定方法**: ベンチマークテスト

#### NFR-2: スケーラビリティ
- **ID**: NFR-EW-002
- **優先度**: 中
- **説明**: 大量のワイルドカードマッチ（～10,000ファイル）でもクラッシュしない
- **制約**: メモリ使用量が合理的範囲内

#### NFR-3: エラーハンドリング
- **ID**: NFR-EW-003
- **優先度**: 高
- **説明**: メモリ不足等のエラーで部分的にデータを残さない
- **測定方法**: エラー注入テスト

---

## 2. 詳細設計

### 2.1 関数シグネチャ

```c
char **ft_expand_word(
    t_token **tokens,      // [IN] トークンポインタ配列
    size_t token_count,    // [IN] トークン数
    t_shell *shell         // [IN] シェル状態（環境変数、終了ステータス等）
);
```

### 2.2 引数仕様

| 引数名 | 型 | 方向 | 説明 |
|--------|------|------|------|
| `tokens` | `t_token **` | IN | トークンへのポインタ配列。各要素は`t_token *`で、トークンの`value`フィールドに文字列が格納されている |
| `token_count` | `size_t` | IN | `tokens`配列の要素数 |
| `shell` | `t_shell *` | IN | シェルのグローバル状態（環境変数リスト、最終終了ステータス等） |

### 2.3 戻り値

| 値 | 説明 |
|----|------|
| `char **` | 成功: NULL終端された文字列配列（`argv`形式）。呼び出し元が`execve()`等に渡す |
| `NULL` | 失敗: メモリ不足または致命的エラー |

### 2.4 データ構造

#### 2.4.1 t_token 構造体

```c
typedef struct s_token {
    t_token_type type;      // トークン種別（TK_WORD等）
    char *value;            // トークンの文字列値（例: "echo", "$HOME", "*.c"）
    bool in_squote;         // シングルクォート内フラグ
    bool in_dquote;         // ダブルクォート内フラグ
    struct s_token *next;   // 次のトークン
} t_token;
```

#### 2.4.2 t_word 構造体

```c
typedef struct s_word {
    char *word;                 // 単語文字列（例: "$HOME", "test", "'literal'"）
    t_word_type type;           // WD_LITERAL, WD_DOLLER, WD_OTHER
    bool to_expand_doller;      // ドル展開フラグ
    bool to_expand_wildcard;    // ワイルドカード展開フラグ
    struct s_word *next;        // 次の単語
} t_word;
```

#### 2.4.3 t_token_result 構造体（内部使用）

```c
typedef struct s_token_result {
    char ***token_results;          // トークンごとの展開結果配列（三重ポインタ）
    size_t *token_result_counts;    // 各トークンの展開結果数
    size_t total_count;             // 全展開結果の総数
    size_t i;                       // ループカウンタ
} t_token_result;
```

**三重ポインタの意味**:
```
token_results[0] → ["echo", NULL]              // トークン0: 1個に展開
token_results[1] → ["file1.c", "file2.c", NULL] // トークン1: 2個に展開
token_results[2] → ["arg", NULL]               // トークン2: 1個に展開
↓ 統合
final_result → ["echo", "file1.c", "file2.c", "arg", NULL]
```

### 2.5 処理フロー

```
[開始] ft_expand_word(tokens, token_count, shell)
  ↓
[1. 初期化] init_token_results()
  - token_results[token_count] 配列確保
  - token_result_counts[token_count] 配列確保
  - total_count = 0
  ↓
[2. トークンループ] for (i = 0; i < token_count; i++)
  ↓
  [2-1. 単語リスト生成] gen_word(tokens[i]->value)
    ↓ 戻り値: t_word *word_list
  [2-2. 展開判定]
    ↓ YES: has_wildcard_to_expand(word_list)
    ↓
  [2-3a. ワイルドカード展開]
    - expand_token_words() でドル展開してパターン取得
    - expand_wildcard() でファイルマッチング
    - マッチ無し → パターンそのまま
    - token_results[i] = 展開結果配列
    - token_result_counts[i] = 結果数
    ↓
  [2-3b. ドル展開のみ]
    - expand_token_words() でドル展開
    - token_results[i] = [展開文字列, NULL]
    - token_result_counts[i] = 1
    ↓
  [2-4. カウント更新]
    - total_count += token_result_counts[i]
  ↓
[3. 最終配列構築] build_final_res()
  - final_res[total_count + 1] 確保
  - 各token_results[i]の要素を順にコピー
  - final_res[total_count] = NULL
  ↓
[4. クリーンアップ] cleanup_token_results()
  - token_results配列を全て解放
  - token_result_counts配列を解放
  ↓
[成功: final_res 返却]
```

### 2.6 主要サブ関数

#### 2.6.1 gen_word()

```c
t_word *gen_word(char *value, size_t value_len, size_t *addition);
```

**目的**: トークン値を単語リストに分解

**アルゴリズム**:
1. 入力をスキャンし、クォート・ドル記号・リテラル部分を検出
2. 各部分を`t_word`ノードとして生成
3. `to_expand_doller`, `to_expand_wildcard`フラグを設定
4. リンクリストとして返す

**例**:
```
入力: "echo '$HOME'*.c"
出力:
  word1: { word="echo", type=WD_LITERAL, to_expand_doller=false, to_expand_wildcard=false }
  word2: { word="'$HOME'", type=WD_LITERAL, to_expand_doller=false, to_expand_wildcard=false }
  word3: { word="*.c", type=WD_LITERAL, to_expand_doller=false, to_expand_wildcard=true }
```

#### 2.6.2 expand_token_words()

```c
static char *expand_token_words(t_word *word_list, t_shell *shell);
```

**目的**: 単語リストの各ノードを展開し、連結した文字列を返す

**アルゴリズム**:
1. 結果文字列を空文字列で初期化
2. 各単語ノードを順に処理:
   - `to_expand_doller == true` → `expand_doller()` または `expand_string_with_dollars()`
   - それ以外 → `ft_strdup(word->word)`
3. 展開結果を結果文字列に連結（`ft_strjoin()`）
4. 連結した文字列を返す

**例**:
```
入力: word_list = [{"$HOME", to_expand_doller=true}, {"/test", to_expand_doller=false}]
      shell->env["HOME"] = "/home/user"
出力: "/home/user/test"
```

#### 2.6.3 expand_wildcard()

```c
char **expand_wildcard(const char *pattern, const char *path, size_t *wildcard_count);
```

**目的**: ワイルドカードパターンをファイル名にマッチング

**アルゴリズム**:
1. `opendir(path)` でディレクトリを開く
2. `readdir()` で全エントリを走査
3. パターンマッチング（`*`, `?` 対応）
4. マッチしたファイル名を動的配列に格納
5. マッチ数とファイル名配列を返す

**特殊ケース**:
- マッチ無し: `wildcard_count = 0`, 配列 = NULL
- ドット開始ファイル: `.*` パターンのみマッチ

#### 2.6.4 expand_doller()

```c
char *expand_doller(t_word *word, t_shell *shell);
```

**目的**: ドル記号変数を展開

**アルゴリズム**:
1. 変数名を抽出（`word->word + 1` から開始）
2. `$?` の場合: `shell->last_exit_status` を文字列化
3. それ以外: `get_env_value(shell->env_list, var_name)`
4. 未定義の場合: 空文字列 `""`

#### 2.6.5 build_final_res()

```c
int build_final_res(char ***final_res, t_token_result *tr, size_t token_count);
```

**目的**: 各トークンの展開結果を単一配列に統合

**アルゴリズム**:
1. `total_count + 1` サイズの配列を確保
2. 二重ループで各トークンの結果をコピー:
   ```c
   for (i = 0; i < token_count; i++)
       for (j = 0; j < token_result_counts[i]; j++)
           final_res[k++] = ft_strdup(token_results[i][j]);
   ```
3. `final_res[k] = NULL` で終端
4. エラー時は`free_double_array()`で解放

### 2.7 メモリ管理戦略

#### 確保タイミング

| フェーズ | 確保対象 | サイズ | 解放タイミング |
|---------|---------|--------|---------------|
| 初期化 | `token_results` | `sizeof(char**) * token_count` | 最後のクリーンアップ |
| 初期化 | `token_result_counts` | `sizeof(size_t) * token_count` | 最後のクリーンアップ |
| トークン処理 | `token_results[i]` | 展開結果数に応じて | 最後のクリーンアップ |
| 最終構築 | `final_res` | `sizeof(char*) * (total_count+1)` | 呼び出し元が解放 |

#### エラー時の解放順序

```
エラー発生（例: gen_word() が NULL）
  ↓
1. 現在のword_listを解放（free_word_list()）
  ↓
2. これまでのtoken_resultsを解放（cleanup_token_results()）
   - token_results[0..i-1]の各配列をfree_double_array()
   - token_results配列をxfree()
   - token_result_counts配列をxfree()
  ↓
3. NULL を返す
```

### 2.8 クォート処理ルール

| クォート種別 | ドル展開 | ワイルドカード展開 | 例 |
|-------------|---------|-------------------|-----|
| シングル `'...'` | **しない** | **しない** | `'$HOME/*.c'` → `$HOME/*.c` |
| ダブル `"..."` | **する** | **しない** | `"$HOME/*.c"` → `/home/user/*.c` |
| クォート無し | **する** | **する** | `$HOME/*.c` → `/home/user/file1.c /home/user/file2.c` |

### 2.9 エラーケース

| エラー条件 | 処理 | 戻り値 |
|-----------|------|--------|
| `init_token_results()` 失敗 | メモリ解放、エラーログ | `NULL` |
| `gen_word()` が `NULL` | 既存データ解放 | `NULL` |
| `expand_token_words()` が `NULL` | 既存データ解放 | `NULL` |
| `build_final_res()` 失敗 | 全データ解放 | `NULL` |

### 2.10 テストケース

#### 正常系

| テストID | 入力トークン | シェル状態 | 期待出力 | 説明 |
|---------|------------|-----------|---------|------|
| TC-EW-001 | `["echo"]` | - | `["echo", NULL]` | 単純なコマンド |
| TC-EW-002 | `["$HOME"]` | `HOME=/home/user` | `["/home/user", NULL]` | 変数展開 |
| TC-EW-003 | `["$?"]` | `last_exit_status=42` | `["42", NULL]` | 終了ステータス |
| TC-EW-004 | `["*.c"]` | ディレクトリに`a.c`, `b.c` | `["a.c", "b.c", NULL]` | ワイルドカード |
| TC-EW-005 | `["'$HOME'"]` | `HOME=/home/user` | `["'$HOME'", NULL]` | シングルクォート（展開なし） |
| TC-EW-006 | `["\"$HOME\""]` | `HOME=/home/user` | `["\"/home/user\"", NULL]` | ダブルクォート（展開あり） |
| TC-EW-007 | `["echo", "*.txt", "$USER"]` | `USER=alice`, `test.txt`存在 | `["echo", "test.txt", "alice", NULL]` | 複数トークン統合 |
| TC-EW-008 | `["$UNDEFINED"]` | 変数未定義 | `["", NULL]` | 未定義変数は空文字列 |

#### エッジケース

| テストID | 入力トークン | 期待出力 | 説明 |
|---------|------------|---------|------|
| TC-EW-E01 | `[""]` | `["", NULL]` | 空トークン |
| TC-EW-E02 | `["*.nonexistent"]` | `["*.nonexistent", NULL]` | マッチ無しパターン |
| TC-EW-E03 | `["$"]` | `["$", NULL]` | ドル単独はリテラル |
| TC-EW-E04 | `["$$"]` | `["$$", NULL]` | 連続ドルはリテラル |

#### 異常系

| テストID | 入力 | 期待動作 | 説明 |
|---------|------|----------|------|
| TC-EW-ERR01 | メモリ不足 | `NULL`返却、全メモリ解放 | リソース枯渇 |

### 2.11 パフォーマンス分析

#### 時間計算量

| 処理 | 計算量 | 説明 |
|------|--------|------|
| トークン分解 | O(T × L) | T=トークン数、L=平均トークン長 |
| ドル展開 | O(W × E) | W=単語数、E=環境変数検索時間（通常O(N)、N=環境変数数） |
| ワイルドカード展開 | O(F × P) | F=ファイル数、P=パターンマッチング時間 |
| 最終統合 | O(R) | R=総展開結果数 |
| **全体** | O(T × L + W × E + F × P + R) | 通常はトークン数とファイル数に依存 |

#### 空間計算量

| データ | 使用量 | 説明 |
|--------|--------|------|
| `t_word` リスト | O(W) | W=単語数（トークン長に比例） |
| `token_results` | O(T × R) | T=トークン数、R=平均展開数 |
| 最終配列 | O(total_count) | 展開後の総引数数 |

### 2.12 依存関係

#### 外部関数

| 関数名 | 用途 | ヘッダ |
|--------|------|--------|
| `gen_word()` | トークン → 単語リスト | `minishell.h` |
| `expand_doller()` | ドル展開 | `minishell.h` |
| `expand_wildcard()` | ワイルドカード展開 | `minishell.h` |
| `get_env_value()` | 環境変数取得 | `minishell.h` |
| `ft_strdup()` | 文字列複製 | `libft.h` |
| `ft_strjoin()` | 文字列連結 | `libft.h` |
| `ft_itoa()` | 整数→文字列 | `libft.h` |
| `xcalloc()` | メモリ確保（ゼロ初期化） | `minishell.h` |
| `xfree()` | メモリ解放 | `minishell.h` |
| `free_double_array()` | 二次元配列解放 | `minishell.h` |
| `free_word_list()` | 単語リスト解放 | `minishell.h` |

---

## 3. 実装ガイドライン

### 3.1 コーディング規約
- 42 School Norm準拠
- 関数の長さ: 25行以内
- 関数の引数: 4個以内
- 複雑な処理は補助関数に分割

### 3.2 デバッグ支援

#### ログ出力（デバッグビルド時）
```c
#ifdef DEBUG
    printf("[DEBUG] token[%zu]: %s\n", i, tokens[i]->value);
    printf("[DEBUG] word_list count: %zu\n", count_word_list(word_list));
    printf("[DEBUG] expand result: %s\n", expanded);
#endif
```

#### Valgrindチェック
```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell
```

### 3.3 レビューチェックリスト
- [ ] 全テストケースが成功
- [ ] Valgrindでメモリリーク0
- [ ] エラーパスで全メモリ解放
- [ ] NULL終端が正しく設定
- [ ] 境界値テスト（空配列、1要素、大量要素）

### 3.4 保守性向上策
- 処理をフェーズごとに関数分割（初期化、展開ループ、統合、クリーンアップ）
- 中間データ構造を明確化（`t_token_result`）
- エラーハンドリングを一箇所に集約

---

## 4. 拡張性

### 将来の拡張候補
1. **ブレース展開**: `{a,b,c}` → `a b c`
2. **チルダ展開**: `~/file` → `/home/user/file`
3. **算術展開**: `$((1+2))` → `3`
4. **コマンド置換**: `$(date)` → コマンド実行結果

### 拡張時の設計変更点
- `gen_word()` に新しい`t_word_type`を追加
- 各展開タイプに対応する展開関数を追加
- `expand_token_words()` に分岐を追加

---

## 5. 変更履歴

| 日付 | バージョン | 変更内容 | 担当者 |
|------|-----------|----------|--------|
| 2025-11-15 | 1.0 | 初版作成 | - |

---

## 6. 承認

| 役割 | 氏名 | 日付 | 署名 |
|------|------|------|------|
| 設計者 | - | 2025-11-15 | - |
| レビュアー | - | - | - |
| 承認者 | - | - | - |
