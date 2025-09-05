= リスト記法の拡張例

//abstract{
この章では、Re:VIEW Extensions で拡張されたリスト記法の使用例を示します。
従来の emlist/emlistnum/cmd/source を統一的な //list で表現します。
//}

== 基本的なリスト記法

=== シンプルなコードブロック

最も基本的なコードブロックです。ラベルやキャプションは省略可能です。

//list[][]{
function greet(name) {
    console.log(`Hello, ${name}!`);
}
//}

=== キャプション付きコード

説明文を追加する場合は、第2引数にキャプションを指定します。

//list[][挨拶関数の実装]{
function greet(name) {
    console.log(`Hello, ${name}!`);
    return `Welcome, ${name}`;
}
//}

== 行番号とハイライト

=== 行番号表示

`lineno=on` オプションで行番号を表示できます。

//list[][フィボナッチ数列][lineno=on]{
def fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

# 使用例
for i in range(10):
    print(fibonacci(i), end=" ")
//}

=== 特定行からの番号開始

`lineno=100` のように数値を指定すると、その番号から開始します。

//list[][エラーハンドリング部分][lineno=100]{
try:
    result = risky_operation()
except ValueError as e:
    logger.error(f"Value error occurred: {e}")
    raise
//}

=== 強調行の指定

`highlight` オプションで特定の行を強調できます。

//list[][重要な処理][lineno=on,highlight=3-5]{
class UserService:
    def create_user(self, name, email):
        # バリデーション（強調される）
        if not self.validate_email(email):
            raise ValueError("Invalid email")
        
        # ユーザー作成
        user = User(name, email)
        return user
//}

== ファイル情報の表示

=== ファイル名バッジ

`filename` オプションでファイル名を表示できます。

//list[][設定ファイル][filename=config/database.yml]{
development:
  adapter: postgresql
  database: myapp_dev
  host: localhost
  port: 5432
  
production:
  adapter: postgresql
  database: myapp_prod
  host: <%= ENV['DB_HOST'] %>
  port: <%= ENV['DB_PORT'] %>
//}

=== 外部ファイルの読み込み

`file` オプションで外部ファイルを読み込めます（ビルド時に展開）。

//emlist[Makefile の内容]{
# このブロックは //list[][Makefile][file=Makefile]{} に変換されます
all: build test

build:
	go build -o bin/app ./cmd/main.go

test:
	go test ./...

clean:
	rm -rf bin/
//}

== 差分表示

=== インライン差分

`diffformat=inline` で差分を表示します。

//list[][最適化の適用][diffformat=inline,lineno=on]{
  def process_items(items):
-     result = []
-     for item in items:
-         if item.is_valid():
-             result.append(item.transform())
-     return result
+     # リスト内包表記で簡潔に
+     return [
+         item.transform() 
+         for item in items 
+         if item.is_valid()
+     ]
//}

== コマンド例

=== コマンドラインの表示

従来の `//cmd` の代替として、適切なオプションを使用します。

//cmd[パッケージのインストール]{
$ npm install -D textlint textlint-plugin-review
$ npm install -D textlint-rule-prh
$ npm list --depth=0
├── textlint@13.4.1
├── textlint-plugin-review@0.4.2
└── textlint-rule-prh@5.3.0
//}

上記は以下に変換されます：

//list[][パッケージのインストール][fontsize=small]{
$ npm install -D textlint textlint-plugin-review
$ npm install -D textlint-rule-prh
$ npm list --depth=0
├── textlint@13.4.1
├── textlint-plugin-review@0.4.2
└── textlint-rule-prh@5.3.0
//}

== インタラクティブ機能（HTML/ePub向け）

=== コピーボタン

`copybutton=on` でクリップボードコピー機能を追加します（HTML/ePub のみ）。

//list[][環境変数の設定][copybutton=on]{
export NODE_ENV=production
export API_KEY=your-secret-key
export DATABASE_URL=postgresql://user:pass@localhost/db
//}

=== 折りたたみ可能

`foldable=on` で長いコードを折りたたみ可能にします（HTML/ePub のみ）。

//list[][長いコードサンプル][foldable=on,lineno=on]{
// 100行を超える長いコード例
// 初期状態では折りたたまれて表示
// クリックで展開可能
class ComplexImplementation {
    // ... 省略 ...
}
//}

== 高度な機能

=== インデントガイド

`indent=4` でインデントレベルを視覚化します。

//list[][ネストした構造][indent=4,lineno=on]{
function processTree(node, depth = 0) {
    if (!node) {
        return null;
    }
    
    const result = {
        value: node.value,
        depth: depth,
        children: []
    };
    
    if (node.children) {
        for (const child of node.children) {
            const processed = processTree(child, depth + 1);
            if (processed) {
                result.children.push(processed);
            }
        }
    }
    
    return result;
}
//}

=== 全角文字の幅調整

`widecharfit=on` で全角文字を半角2文字分として扱います（PDF向け）。

//list[][日本語コメント付きコード][widecharfit=on,lineno=on]{
def calculate_税額(価格, 税率=0.10):
    """
    税額を計算する関数
    価格: 商品の本体価格（円）
    税率: 消費税率（デフォルト10%）
    """
    税額 = int(価格 * 税率)
    合計 = 価格 + 税額
    return 合計
//}

=== 複数言語のタブ表示

実装言語ごとの例を示す場合（HTML ではタブ切り替え、PDF では縦に連続）。

//list[][同じロジックの実装][tabset=js:JavaScript|py:Python|rb:Ruby]{
// JavaScript
const factorial = (n) => {
    return n <= 1 ? 1 : n * factorial(n - 1);
};

# Python
def factorial(n):
    return 1 if n <= 1 else n * factorial(n - 1)

# Ruby  
def factorial(n)
  n <= 1 ? 1 : n * factorial(n - 1)
end
//}

== 移行時の注意点

=== 非推奨記法の検出

以下のような旧記法を使用すると、textlint が警告を出力します：

 * `//emlist{...}` → `//list[][]{...}`
 * `//emlistnum{...}` → `//list[][][lineno=on]{...}`
 * `//cmd{...}` → `//list[][]{...}` 
 * `//source{...}` → `//list[][][filename=...]{...}`

=== 自動変換

macro-shims により、ビルド時に自動的に新記法へ変換されます：

//list[][前処理コマンド][fontsize=small]{
$ review-preprocess "articles/**/*.re" -o .out --stats

Re:VIEW Preprocessor v0.2.0 - Starting conversion...
Found 3 file(s) to process
✓ articles/ch01.re (5 replacements)
✓ articles/ch02.re (3 replacements)
✓ articles/ch03-list-examples.re (8 replacements)

=== Conversion Statistics ===
Files processed: 3
Total replacements: 16

Rule applications:
  emlist to list: 6
  emlistnum to list: 4
  cmd to list: 3
  source to list: 3
//}

== まとめ

Re:VIEW Extensions のリスト記法拡張により：

 * 統一的な `//list` 記法で一貫性向上
 * 豊富なオプションで表現力アップ
 * PDF/HTML/ePub それぞれに最適化された出力
 * 既存コードの自動移行サポート

詳細は `packages/review-authoring-guides/list-migration.md` を参照してください。