= リスト機能サンプル集

== 基本的なリスト表記

=== シンプルなコードリスト

//list[simple_code][基本的なPythonコード]{
def hello_world():
    print("Hello, World!")
    return True
//}

=== 行番号付きリスト

//list[with_lineno][行番号表示][lineno=on]{
function calculate(a, b) {
    const sum = a + b;
    const diff = a - b;
    const product = a * b;
    const quotient = a / b;
    return { sum, diff, product, quotient };
}
//}

=== 言語指定とシンタックスハイライト

//list[syntax_highlight][TypeScriptコード][lang=typescript]{
interface User {
    id: number;
    name: string;
    email: string;
    createdAt: Date;
}

class UserService {
    async getUser(id: number): Promise<User> {
        // APIからユーザー情報を取得
        const response = await fetch(`/api/users/${id}`);
        return response.json();
    }
}
//}

== 拡張オプション

=== ファイル名表示

//list[with_filename][設定ファイル][filename=config.yml,lang=yaml]{
database:
  host: localhost
  port: 5432
  name: myapp_db
  
redis:
  host: localhost
  port: 6379
  
logging:
  level: debug
  output: stdout
//}

=== 行強調表示

//list[highlight_lines][重要な行を強調][lineno=on,highlight=3-5,lang=python]{
import os
import sys
# 以下の3行が重要
from myapp import create_app
from myapp.config import Config
from myapp.database import init_db

app = create_app(Config)
init_db(app)

if __name__ == "__main__":
    app.run(debug=True)
//}

=== フォントサイズ調整

//list[font_size][小さめの文字でコマンド表示][fontsize=small]{
$ git clone https://github.com/example/repo.git
$ cd repo
$ npm install
$ npm run build
$ npm start
//}

== コード折り返し機能

=== 長い行の自動折り返し

//list[wrap_example][折り返しサンプル][wrap=80,foldmark=↩]{
public static final String VERY_LONG_CONSTANT_NAME = "This is a very long string that contains some important configuration data that should be wrapped properly when displayed in documentation to maintain readability";

// 長いメソッドチェーン
const result = someObject.method1().method2().method3().method4().method5().method6().filter(x => x > 0).map(x => x * 2).reduce((a, b) => a + b, 0);
//}

=== 折り返し無効化

//list[no_wrap][折り返し無効][fold=off]{
// この行は折り返されません。画面幅を超えても一行で表示されます。
const noWrap = "This line will not be wrapped regardless of its length and will extend beyond the viewport if necessary";
//}

== 差分表示

=== インライン差分フォーマット

//list[diff_inline][変更箇所の表示][diffformat=inline,lang=javascript]{
  function processData(input) {
-     const result = input * 2;
+     const result = input * 3;  // 係数を2から3に変更
      console.log(result);
      return result;
  }
//}

== HTML専用オプション

=== コピーボタン付きコード

//list[copy_button][コピー可能なコード][copybutton=on,lang=bash]{
#!/bin/bash
# セットアップスクリプト

echo "Installing dependencies..."
npm install

echo "Building project..."
npm run build

echo "Running tests..."
npm test

echo "Setup complete!"
//}

=== 折りたたみ可能なコード

//list[foldable][長いコードを折りたたみ表示][foldable=on,lang=python]{
class DataProcessor:
    """データ処理クラス"""
    
    def __init__(self, config):
        self.config = config
        self.data = []
        
    def load_data(self, filepath):
        """データをファイルから読み込む"""
        with open(filepath, 'r') as f:
            self.data = json.load(f)
            
    def process(self):
        """データを処理する"""
        results = []
        for item in self.data:
            processed = self._process_item(item)
            results.append(processed)
        return results
        
    def _process_item(self, item):
        """個別アイテムの処理"""
        # 複雑な処理ロジック
        return item
        
    def save_results(self, results, output_path):
        """結果を保存"""
        with open(output_path, 'w') as f:
            json.dump(results, f, indent=2)
//}

== 旧形式からの変換例

=== emlist → list 変換

変換前の書き方：
@<code>{//emlist[caption]{...}}

変換後（現在の推奨）：

//list[emlist_sample][emlistからの変換例]{
# 旧emlist形式から変換されたコード
echo "This was originally an emlist block"
echo "Now converted to standard list format"
//}

=== emlistnum → list with lineno

変換前の書き方：
@<code>{//emlistnum[caption]{...}}

変換後（現在の推奨）：

//list[emlistnum_sample][emlistnumからの変換例][lineno=on]{
print("Line 1: Originally from emlistnum")
print("Line 2: Now with explicit lineno option")
print("Line 3: Line numbers are displayed")
//}

=== cmd → list with styling

変換前の書き方：
@<code>{//cmd[caption]{...}}

変換後（現在の推奨）：

//list[cmd_sample][コマンド実行例][fontsize=small]{
$ docker build -t myapp:latest .
$ docker run -p 8080:8080 myapp:latest
$ docker-compose up -d
//}

=== source → list with filename

変換前の書き方：
@<code>{//source[filename][caption]{...}}

変換後（現在の推奨）：

//list[source_sample][ソースファイルの例][filename=main.go,lang=go]{
package main

import "fmt"

func main() {
    fmt.Println("Hello, Go!")
}
//}

== 複合オプションの例

=== 複数オプションの組み合わせ

//list[complex_options][全部入り][lineno=on,lang=ruby,filename=app.rb,highlight=5-7,fontsize=small,wrap=60,foldmark=⤸]{
# Rubyアプリケーションのエントリーポイント
require 'sinatra'
require 'json'

# ここがメインの処理部分（5-7行目が強調表示される）
get '/api/users/:id' do
  user = User.find(params[:id])
  content_type :json
  user.to_json
end

# この行は60文字で折り返されます。長いコメントや文字列リテラルがある場合に便利な機能です。
post '/api/users' do
  user = User.create(JSON.parse(request.body.read))
  status 201
  user.to_json
end
//}

== まとめ

本サンプルで紹介した機能：

 * 基本的なリスト表記
 * 行番号表示（@<code>{lineno=on}）
 * 言語指定（@<code>{lang=言語名}）
 * ファイル名表示（@<code>{filename=ファイル名}）
 * 行強調（@<code>{highlight=行番号}）
 * フォントサイズ（@<code>{fontsize=サイズ}）
 * 折り返し制御（@<code>{wrap=文字数}, @<code>{fold=on/off}）
 * 差分表示（@<code>{diffformat=inline/side}）
 * HTMLオプション（@<code>{copybutton=on}, @<code>{foldable=on}）

これらのオプションは組み合わせて使用することができ、ドキュメントの用途に応じて最適な表示を実現できます。