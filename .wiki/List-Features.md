# リスト機能の拡張

Re:VIEW 5.8 対応のリスト機能拡張について説明します。

## 概要

従来の `emlist`、`emlistnum`、`cmd`、`source` タグを統一された `//list` タグに変換し、豊富なオプションで表現力を向上させます。

## 基本構文

```review
//list[ラベル][キャプション][オプション]{
  コード内容
//}
```

## 利用可能なオプション

### 表示制御

| オプション | 説明 | 値の例 |
|-----------|------|--------|
| `lineno` | 行番号表示 | `on`, `off`, `数値` (開始番号) |
| `lang` | シンタックスハイライト言語 | `javascript`, `python`, `ruby` など |
| `fontsize` | フォントサイズ | `small`, `normal`, `large` |
| `captionpos` | キャプション位置 | `top`, `bottom`, `none` |

### 拡張機能

| オプション | 説明 | 値の例 |
|-----------|------|--------|
| `filename` | ファイル名バッジ表示 | `main.js`, `config.yml` など |
| `highlight` | 行強調 | `3`, `5-10`, `1,3,5` |
| `wrap` | 折り返し文字数 | `80`, `100` など |
| `fold` | 折り返し有効/無効 | `on`, `off` |
| `foldmark` | 折り返し記号 | `↩`, `⤸`, `...` など |

### HTML専用オプション

| オプション | 説明 | 値の例 |
|-----------|------|--------|
| `copybutton` | コピーボタン表示 | `on`, `off` |
| `foldable` | 折りたたみ可能 | `on`, `off` |
| `anchorlinks` | アンカーリンク | `on`, `off` |

### 差分表示

| オプション | 説明 | 値の例 |
|-----------|------|--------|
| `diffformat` | 差分表示形式 | `inline`, `side` |

## 使用例

### 行番号付きコード

```review
//list[sample][サンプルコード][lineno=on,lang=javascript]{
function hello() {
    console.log("Hello, World!");
}
//}
```

### ファイル名とハイライト

```review
//list[config][設定ファイル][filename=package.json,highlight=3-5,lang=json]{
{
  "name": "my-app",
  "version": "1.0.0",
  "main": "index.js",
  "license": "MIT"
}
//}
```

### 長い行の折り返し

```review
//list[wrap_demo][折り返しサンプル][wrap=60,foldmark=↩]{
const longString = "This is a very long string that will be wrapped at 60 characters with a fold mark";
//}
```

### 複数オプションの組み合わせ

```review
//list[complex][複合例][lineno=on,lang=python,filename=app.py,highlight=2-4,fontsize=small]{
import os
def main():
    # 重要な処理
    process_data()
    return True
//}
```

## 旧形式からの移行

### emlist → list

```review
# 旧形式
//emlist[キャプション]{
  コード
//}

# 新形式
//list[][キャプション]{
  コード
//}
```

### emlistnum → list with lineno

```review
# 旧形式
//emlistnum[キャプション]{
  コード
//}

# 新形式
//list[][キャプション][lineno=on]{
  コード
//}
```

### cmd → list with fontsize

```review
# 旧形式
//cmd[キャプション]{
  $ command
//}

# 新形式
//list[][キャプション][fontsize=small]{
  $ command
//}
```

### source → list with filename

```review
# 旧形式
//source[ファイル名][キャプション]{
  コード
//}

# 新形式
//list[][キャプション][filename=ファイル名]{
  コード
//}
```

## プリプロセッサによる自動変換

`review-preprocess` コマンドを使用すると、旧形式を自動的に新形式に変換できます：

```bash
npx review-preprocess "articles/**/*.re" --out-dir .out
```

## 注意事項

- すべてのオプションがすべての出力形式でサポートされるわけではありません
- HTML専用オプション（`copybutton`, `foldable`）はPDF出力では無視されます
- `wrap` オプションは実際のコードの動作に影響しません（表示のみ）

## 関連ドキュメント

- [Note/Miniblock機能](Note-Miniblock-Features)
- [サンプルプロジェクト](Sample-Project)
- [プリプロセッサの使い方](Preprocessor-Usage)