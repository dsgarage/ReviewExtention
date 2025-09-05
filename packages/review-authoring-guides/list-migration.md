# リスト記法移行ガイド

Re:VIEW Extensions におけるコードリスト記法の統一と拡張機能の解説

## 概要

Re:VIEW Starter Template で使用されていた複数のリスト記法（`emlist`, `emlistnum`, `cmd`, `source`）を、統一的な `//list` 記法に移行します。これにより、一貫性のあるコード表示と豊富な拡張オプションが利用可能になります。

## 移行対応表

### 基本的な移行

| 旧記法 | 新記法 | 用途 |
|--------|--------|------|
| `//emlist{...}` | `//list[][]{...}` | 基本のコードブロック |
| `//emlistnum{...}` | `//list[][][lineno=on]{...}` | 行番号付きコード |
| `//cmd{...}` | `//list[][コマンド][fontsize=small]{...}` | コマンド例 |
| `//source[file][caption]{...}` | `//list[][caption][filename=file]{...}` | ソースコード |

### 自動変換

macro-shims により、ビルド時に自動的に変換されます：

```bash
# 前処理実行
$ review-preprocess "articles/**/*.re" -o .out

# 変換統計を表示
$ review-preprocess "articles/**/*.re" --stats
```

## 拡張オプション

### 基本構文

```
//list[<ラベル>][<キャプション>][<オプション>]{
  コード内容
//}
```

### オプション一覧

#### 表示制御

| オプション | 値 | デフォルト | 説明 |
|------------|-----|------------|------|
| `captionpos` | top/bottom/none | top | キャプション位置 |
| `filename` | 文字列 | なし | ファイル名バッジ表示 |
| `fontsize` | small/normal/large | normal | フォントサイズ |
| `theme` | auto/light/dark | auto | カラーテーマ（HTML） |

#### 行番号・強調

| オプション | 値 | デフォルト | 説明 |
|------------|-----|------------|------|
| `lineno` | on/off/数値 | off | 行番号表示 |
| `linenowidth` | 数値 | 3 | 行番号の桁数 |
| `highlight` | 行番号リスト | なし | 強調行（例: 1,3-5,10） |
| `showlinenumbers` | on/off/hover | on | 行番号表示方式（HTML） |

#### 高度な機能

| オプション | 値 | デフォルト | 説明 |
|------------|-----|------------|------|
| `diffformat` | none/inline/side-by-side | none | 差分表示形式 |
| `copybutton` | on/off | off | コピーボタン（HTML） |
| `foldable` | on/off | off | 折りたたみ可能（HTML） |
| `anchorlinks` | on/off | off | 行番号アンカー（HTML） |
| `indent` | 数値 | なし | インデントガイド幅 |
| `widecharfit` | on/off | off | 全角文字幅調整 |

#### 外部ファイル

| オプション | 値 | デフォルト | 説明 |
|------------|-----|------------|------|
| `file` | パス | なし | 外部ファイル読み込み |
| `include_region` | 開始,終了 | なし | 部分抽出マーカー |
| `enc` | utf-8/shift_jis/euc-jp/auto | auto | 文字コード |
| `onmissing` | error/warn/skip | error | ファイルなし時の挙動 |

## 使用例

### 1. 基本的なコードブロック

旧:
```review
//emlist{
function hello() {
  console.log("Hello, World!");
}
//}
```

新:
```review
//list[][]{
function hello() {
  console.log("Hello, World!");
}
//}
```

### 2. 行番号付きコード

旧:
```review
//emlistnum{
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)
//}
```

新:
```review
//list[][階乗の計算][lineno=on]{
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)
//}
```

### 3. ファイル名と強調行

```review
//list[user-class][Userクラスの実装][filename=models/user.py,lineno=on,highlight=3-5]{
class User:
    def __init__(self, name, email):
        self.name = name      # 強調される
        self.email = email    # 強調される
        self.active = True    # 強調される
    
    def activate(self):
        self.active = True
        return self
//}
```

### 4. 差分表示

```review
//list[][パフォーマンス改善][diffformat=inline,lineno=on]{
  def process_data(items):
-     result = []
-     for item in items:
-         result.append(transform(item))
-     return result
+     # リスト内包表記で高速化
+     return [transform(item) for item in items]
//}
```

### 5. コマンド例

旧:
```review
//cmd[インストール方法]{
$ npm install -g review-preprocessor
$ review-preprocess --version
v0.2.0
//}
```

新:
```review
//list[][インストール方法][fontsize=small,lang=bash]{
$ npm install -g review-preprocessor
$ review-preprocess --version
v0.2.0
//}
```

### 6. 外部ファイル読み込み

```review
//list[][設定ファイル][file=config/settings.yml,highlight=10-15]{
//}
```

部分抽出:
```review
//list[][使用例のみ抽出][file=src/main.py,include_region=#region usage,#endregion]{
//}
```

### 7. インタラクティブ機能（HTML/ePub）

```review
//list[][対話的なサンプル][copybutton=on,foldable=on,anchorlinks=on]{
// 長いコードサンプル
// コピーボタンが表示される
// 折りたたみ可能
// 各行にアンカーリンク
//}
```

### 8. タブセット（複数言語）

```review
//list[][実装例][tabset=ts:src/example.ts|js:src/example.js|py:src/example.py]{
//}
```
※ HTML では言語ごとのタブ切り替え、PDF では縦に連続表示

## textlint による警告

非推奨の記法を使用した場合、textlint が警告を表示します：

```bash
$ npm run lint

articles/chapter1.re
  10:1  warning  非推奨のリスト記法 'emlist' が検出されました。
               //list を使用してください。  @dsgarage/no-deprecated-list

✖ 1 problem (0 errors, 1 warning)
```

自動修正:
```bash
$ npm run lint:fix
```

## ベストプラクティス

### 1. 用途に応じたオプション選択

- **通常のコード**: オプションなし、シンプルに
- **重要な部分**: `highlight` で強調
- **外部ファイル**: `file` で読み込み、メンテナンス性向上
- **差分説明**: `diffformat` で変更箇所を明確に

### 2. PDF と HTML の違いを考慮

- PDF専用: `fontsize`, `widecharfit`, `indent`
- HTML専用: `copybutton`, `foldable`, `anchorlinks`, `tabset`
- 共通: `highlight`, `lineno`, `filename`

### 3. パフォーマンス

- 外部ファイル読み込みは必要な部分のみ `include_region` で抽出
- 大量のコードは `foldable=on` で初期状態を折りたたみ（HTML）

### 4. アクセシビリティ

- 重要なコードには適切な `caption` を設定
- `highlight` した行には説明コメントを追加
- `anchorlinks=on` で深いリンクを可能に（HTML）

## トラブルシューティング

### Q: 変換後にレイアウトが崩れる

A: オプションの組み合わせを確認してください。特に `diffformat=side-by-side` と `indent` は併用できません。

### Q: 外部ファイルが読み込めない

A: ファイルパスを確認し、`onmissing=warn` で警告表示に変更して原因を特定してください。

### Q: 行番号が表示されない

A: `lineno=on` を明示的に指定してください。`//listnum` は非推奨です。

## 関連リソース

- [Re:VIEW 公式ドキュメント](https://github.com/kmuto/review)
- [macro-shims リファレンス](../review-macro-shims/README.md)
- [textlint 設定ガイド](../review-lint-preset/README.md)