= 行折り返し機能の包括的テスト

== はじめに

このドキュメントは、Re:VIEW 5.8におけるコードブロックの行折り返し機能を、LaTeXスタイルファイル（review-custom.sty）で実現する方法を示します。

== 基本的な折り返し機能

=== デフォルト設定（自動折り返しあり）

以下のコードは、デフォルト設定により自動的に折り返されます。長い行は赤い折り返し記号（→）と共に次の行に続きます。

//list[wrap_default][デフォルトの自動折り返し]{
public class VeryLongClassNameForDemonstrationPurposes implements InterfaceWithLongName, AnotherInterfaceWithEvenLongerName, YetAnotherVeryVeryLongInterfaceName {
    private static final String VERY_LONG_CONSTANT_NAME_FOR_DEMONSTRATION = "This is a very long string constant that demonstrates how automatic line wrapping works in Re:VIEW with our custom LaTeX style settings";
    
    public void methodWithVeryLongSignature(String firstParameter, String secondParameter, String thirdParameter, String fourthParameter, String fifthParameter) {
        System.out.println("This method has a very long signature that would normally overflow the page width without proper line wrapping configuration");
    }
}
//}

=== 日本語を含むコード

日本語コメントを含むコードも適切に折り返されます。

//list[wrap_japanese][日本語コメント付きコード]{
public class 日本語クラス {
    // これは非常に長い日本語のコメントで、自動折り返し機能が日本語文字に対しても正しく動作することを確認するためのテストケースです。
    private String 長い変数名を持つフィールド = "日本語の文字列リテラルも正しく折り返されることを確認するための長いテキスト";
    
    public void 処理実行メソッド() {
        // 複雑な処理の説明：このメソッドは様々な処理を実行し、その結果をログに出力します。長い説明文も適切に折り返されます。
        System.out.println("処理を開始します：" + 長い変数名を持つフィールド);
    }
}
//}

== 様々なプログラミング言語での折り返し

=== JavaScript/TypeScriptの例

//list[wrap_js][JavaScriptの長い行]{
const complexConfiguration = { apiEndpoint: "https://api.example.com/v1/very/long/endpoint/path", authToken: "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ", timeout: 30000 };

async function performComplexAsyncOperation(param1, param2, param3, param4, param5, param6, param7, param8) {
    const result = await fetch(complexConfiguration.apiEndpoint, { method: 'POST', headers: { 'Authorization': complexConfiguration.authToken, 'Content-Type': 'application/json' }, body: JSON.stringify({ param1, param2, param3, param4, param5, param6, param7, param8 }) });
    return result.json();
}
//}

=== Pythonの例

//list[wrap_python][Pythonの長い行]{
def complex_data_processing_function(dataframe, column_names=['very_long_column_name_1', 'very_long_column_name_2', 'very_long_column_name_3', 'very_long_column_name_4'], transformation_params={'scale': 1.0, 'offset': 0.0, 'normalization': True, 'outlier_removal': True}):
    """
    この関数は複雑なデータ処理を実行します。非常に長いドキュメント文字列も自動的に折り返されて、読みやすい形式で表示されます。
    """
    result = dataframe[column_names].apply(lambda x: (x * transformation_params['scale'] + transformation_params['offset']) if transformation_params['normalization'] else x)
    return result
//}

=== SQLの例

//list[wrap_sql][SQLの長いクエリ]{
SELECT customers.customer_id, customers.first_name, customers.last_name, orders.order_id, orders.order_date, order_items.product_id, products.product_name, order_items.quantity, order_items.unit_price FROM customers INNER JOIN orders ON customers.customer_id = orders.customer_id INNER JOIN order_items ON orders.order_id = order_items.order_id INNER JOIN products ON order_items.product_id = products.product_id WHERE orders.order_date BETWEEN '2024-01-01' AND '2024-12-31' AND customers.country = 'Japan' ORDER BY orders.order_date DESC, customers.last_name ASC;
//}

== 実際のユースケース

=== 設定ファイルの例

設定ファイルなど、長い値を持つコードも見やすく表示されます。

//list[wrap_config][複雑な設定ファイル]{
{
  "applicationSettings": {
    "databaseConnection": "Server=myServerAddress;Database=myDataBase;User Id=myUsername;Password=myPassword;Connection Timeout=30;Command Timeout=60;Max Pool Size=100;Min Pool Size=5;Load Balance Timeout=30;ApplicationName=MyApplication",
    "apiConfiguration": {
      "endpoints": {
        "authentication": "https://api.production.example.com/auth/v2/authenticate",
        "userManagement": "https://api.production.example.com/users/v3/management",
        "dataProcessing": "https://api.production.example.com/data/v4/process"
      }
    }
  }
}
//}

=== コマンドラインの例

長いコマンドラインも適切に折り返されます。

//cmd[長いコマンドライン]{
$ docker run --name my-application-container --env DATABASE_URL="postgresql://username:password@hostname:5432/database_name" --env REDIS_URL="redis://username:password@hostname:6379/0" --env API_KEY="very-long-api-key-that-needs-to-be-kept-secret" --volume /host/path/to/data:/container/path/to/data --publish 8080:80 --restart unless-stopped --memory 2g --cpus 1.5 my-application:latest
//}

== 折り返しに関する注意事項

=== メリット

 * 長い行がページ幅を超えることがない
 * コードの可読性が向上
 * PDFでの表示が最適化される
 * 印刷時にコードが切れない

=== 現在の制限事項

 * Re:VIEW 5.8の標準機能では、個別のコードブロックごとの折り返し制御はできない
 * 全てのコードブロックに一括で適用される
 * 折り返し位置の細かい制御は限定的

=== 今後の改善案

 1. Re:VIEW本体がオプション引数をサポートした場合、個別制御が可能になる
 2. カスタム環境（reviewlistwrap/reviewlistnowrap）を使い分けることで、部分的な制御は可能
 3. 言語別のスタイル定義により、より適切な折り返しが実現できる

== まとめ

LaTeXスタイルファイル（review-custom.sty）を使用することで、Re:VIEW 5.8でも実用的な行折り返し機能を実現できます。この方法は：

 * 既存の原稿に影響を与えない
 * 確実に動作する
 * メンテナンスが容易
 * Re:VIEWのバージョンアップにも対応しやすい

という利点があり、現時点で最も推奨される解決策です。