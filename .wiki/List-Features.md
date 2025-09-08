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

| オプション | 説明 | 値の例 | 備考 |
|-----------|------|--------|------|
| `filename` | ファイル名バッジ表示 | `main.js`, `config.yml` など | - |
| `highlight` | 行強調 | `3`, `5-10`, `1,3,5` | - |
| `wrap` | 折り返し文字数 | `80`, `100` など | **現在動作しない（Issue #20）** |
| `fold` | 折り返し有効/無効 | `on`, `off` | **現在動作しない（Issue #20）** |
| `foldmark` | 折り返し記号 | `↩`, `⤸`, `...` など | wrap/foldと共に使用 |

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

## 外部ファイル参照機能

Re:VIEWでは、外部ファイルのコードを参照して表示する機能を提供しています。

### #@mapfile ディレクティブ

外部ファイルの内容全体を取り込みます：

```review
//listnum[sample][sample.cs]{
#@mapfile(code/sample.cs)
#@end
//}
```

### #@maprange ディレクティブ

外部ファイルの一部範囲を取り込みます：

```review
//listnum[partial][部分的なコード]{
#@maprange(code/sample.cs,start_marker,end_marker)
#@end
//}
```

マーカーはコメント内に記述します。例：
```csharp
// sample.cs
public class Sample {
    // start_marker
    public void ImportantMethod() {
        // この部分だけが取り込まれる
    }
    // end_marker
}
```

### #@mapoutput ディレクティブ

コマンドの実行結果を取り込みます：

```review
//cmd[実行結果]{
#@mapoutput(ls -la)
#@end
//}
```

### ファイルパスの指定

- 相対パス：原稿ファイルからの相対パス
- 絶対パス：プロジェクトルートからのパス（先頭に`/`）

### 使用上の注意

1. **ファイルの存在確認**：参照先ファイルが存在しない場合、ビルド時にエラーとなります
2. **文字エンコーディング**：UTF-8を推奨
3. **改行コード**：LF（Unix形式）を推奨
4. **パフォーマンス**：大きなファイルを頻繁に参照する場合は、ビルド時間に影響する可能性があります

### トラブルシューティング

もし`#@mapfile`等が正しく展開されない場合：

1. Re:VIEWのバージョンを確認（5.0以降で対応）
2. プリプロセッサが有効になっているか確認
3. ファイルパスが正しいか確認
4. 以下のコマンドで手動プリプロセス実行：
   ```bash
   review-preprocess --replace source.re
   ```

## 既知の問題と回避策

### wrap/foldオプションの問題（Issue #20）

現在、`wrap`および`fold`オプションはRe:VIEW 5.8の仕様上、PDF出力時に動作しません。

#### 問題の原因

1. **Re:VIEW 5.8の制限**：`//list`コマンドは第3引数のオプション（wrap、foldなど）を認識しない
2. **構造的な問題**：プリプロセッサでの折り返しは、LaTeX処理時に無効化される

#### 推奨される解決策：LaTeXスタイルファイルでの対応

**改良版review-custom.sty**を使用することで、包括的な折り返し機能を実現：

1. `sty/review-custom.sty`を作成
2. 以下の機能を提供：

```latex
% デフォルト設定（全コードブロックで自動折り返し）
\lstset{
  breaklines=true,                % 自動改行を有効化
  breakatwhitespace=true,          % 空白で改行
  breakindent=20pt,                % 折り返し行のインデント
  postbreak=\mbox{\textcolor{red}{$\hookrightarrow$}\space}, % 赤い折り返し記号
  columns=fullflexible,            % 文字間隔の最適化
  frame=single,                     % 枠線
  % 日本語対応
  extendedchars=true,
  inputencoding=utf8
}
```

3. `config.yml`に追加：

```yaml
texstyle: ["reviewmacro", "review-custom"]
```

#### 提供される機能

- ✅ **自動折り返し**：長い行を自動的にページ幅内に収める
- ✅ **日本語対応**：日本語コメントも適切に処理
- ✅ **視覚的な折り返し記号**：赤い→記号で折り返しを明示
- ✅ **言語別スタイル**：C#、JavaScript、Python用の設定済み
- ✅ **カスタム環境**：将来の拡張に対応（reviewlistwrap/reviewlistnowrap）

#### 利点

- 既存の原稿に影響なし
- 確実に動作する
- Re:VIEWのバージョンアップに対応しやすい
- メンテナンスが容易

詳細なサンプルは`examples/mapfile-sample/mapfile-demo/wrap-test.re`を参照してください。

## 注意事項

- すべてのオプションがすべての出力形式でサポートされるわけではありません
- HTML専用オプション（`copybutton`, `foldable`）はPDF出力では無視されます
- `wrap` オプションは実際のコードの動作に影響しません（表示のみ）
- 外部ファイル参照は、プリプロセッサ実行時に展開されます
- `#@maprange`はRe:VIEW 5.8のプリプロセッサで引数エラーが発生する場合があります

## 関連ドキュメント

- [Note/Miniblock機能](Note-Miniblock-Features)
- [サンプルプロジェクト](Sample-Project)
- [プリプロセッサの使い方](Preprocessor-Usage)