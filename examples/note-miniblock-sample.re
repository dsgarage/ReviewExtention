= Note/Miniblock機能サンプル集

== 基本的なnoteブロック

=== シンプルなnote

//note[補足情報]{
これは基本的なnoteブロックです。
追加の説明や補足情報を記載する際に使用します。
//}

=== タイトル付きnote

//note[API使用上の注意]{
APIを呼び出す際は、必ず認証トークンをヘッダーに含めてください。
レート制限は1分間に60リクエストまでです。
//}

=== ラベル付きnote（参照可能）

//note[api_auth][認証について]{
OAuth 2.0を使用した認証フローについて説明します。
詳細は@<noteref>{api_auth}を参照してください。
//}

== 情報系ミニブロック

=== info（情報）

//info[システム要件]{
このアプリケーションを実行するには以下の環境が必要です：

 * Node.js 18.0以上
 * npm 9.0以上
 * MongoDB 6.0以上
//}

//info[]{
タイトルなしの情報ブロックも使用できます。
簡潔な情報提供に適しています。
//}

=== tip（ヒント）

//tip[パフォーマンス改善のヒント]{
クエリのパフォーマンスを改善するには：

 1. インデックスを適切に設定する
 2. N+1問題を避ける
 3. キャッシュを活用する
//}

//tip[便利なショートカット]{
@<code>{Ctrl+Shift+P}でコマンドパレットを開くことができます。
//}

=== memo（メモ）

//memo[開発メモ]{
TODO: この機能は次のスプリントで改善予定
- エラーハンドリングの追加
- ユニットテストの作成
- ドキュメントの更新
//}

//memo[]{
将来的にはWebSocket対応も検討中。
//}

== 警告系ミニブロック

=== warning（警告）

//warning[非推奨のAPI]{
このAPIは将来のバージョンで削除される予定です。
新しいAPIへの移行を推奨します。

旧: @<code>{getUserById(id)}
新: @<code>{getUser({ id: id })}
//}

//warning[セキュリティ警告]{
本番環境では必ずHTTPSを使用してください。
HTTPでの通信は中間者攻撃のリスクがあります。
//}

=== caution（注意）

//caution[データベースマイグレーション]{
マイグレーションを実行する前に必ずバックアップを取得してください。

//list[][]{
# バックアップコマンド
$ mongodump --out=/backup/$(date +%Y%m%d)
$ npm run migrate
//}
//}

//caution[メモリ使用量に注意]{
大量のデータを処理する際は、メモリ使用量を監視してください。
必要に応じてストリーミング処理を検討してください。
//}

=== important（重要）

//important[破壊的変更]{
バージョン3.0.0には破壊的変更が含まれています：

 * APIエンドポイントの変更
 * 設定ファイルフォーマットの変更
 * 依存関係の更新

アップグレード前に移行ガイドを必ず確認してください。
//}

//important[緊急セキュリティアップデート]{
CVE-2024-XXXX の脆弱性に対応するため、
即座にバージョン2.5.1以上にアップデートしてください。
//}

== お知らせ系ミニブロック

=== notice（お知らせ）

//notice[メンテナンス予定]{
以下の日程でシステムメンテナンスを実施します：

日時: 2024年12月25日 02:00-05:00 (JST)
影響: サービスが一時的に利用できません

ご不便をおかけしますが、ご理解のほどよろしくお願いします。
//}

//notice[新機能リリース]{
バージョン2.4.0で以下の新機能が追加されました：

 * ダークモード対応
 * 多言語サポート（日本語、英語、中国語）
 * パフォーマンスの大幅改善
//}

== 実践的な使用例

=== エラーハンドリングのドキュメント

//important[エラーハンドリング必須]{
すべてのAPI呼び出しには適切なエラーハンドリングを実装してください。
//}

//list[error_handling][エラーハンドリングの実装例][lang=javascript]{
async function fetchUserData(userId) {
    try {
        const response = await fetch(`/api/users/${userId}`);
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        
        return await response.json();
    } catch (error) {
        console.error('Failed to fetch user data:', error);
        
        // ユーザーへの通知
        showNotification('データの取得に失敗しました', 'error');
        
        // エラーの再スロー or デフォルト値の返却
        throw error;
    }
}
//}

//warning[エラーログの取り扱い]{
エラーログに個人情報やパスワードが含まれないよう注意してください。
//}

=== デプロイ手順のドキュメント

//info[デプロイ前の確認事項]{
デプロイを実行する前に、以下の項目を確認してください：

 * すべてのテストが成功している
 * コードレビューが完了している
 * ステージング環境での動作確認済み
//}

//list[deploy_script][デプロイスクリプト][lang=bash,lineno=on]{
#!/bin/bash
# デプロイスクリプト

echo "Starting deployment..."

# 1. テストの実行
npm test
if [ $? -ne 0 ]; then
    echo "Tests failed. Aborting deployment."
    exit 1
fi

# 2. ビルド
npm run build

# 3. デプロイ
npm run deploy:production

echo "Deployment completed!"
//}

//caution[ロールバック手順]{
問題が発生した場合は、以下のコマンドでロールバックできます：

@<code>{npm run rollback:production}
//}

=== セキュリティガイドライン

//important[セキュリティベストプラクティス]{
以下のセキュリティ対策を必ず実施してください：
//}

//tip[パスワード管理]{
パスワードは環境変数または専用のシークレット管理ツールで管理しましょう。
ソースコードに直接記述しないでください。
//}

//warning[SQLインジェクション対策]{
ユーザー入力を直接SQLクエリに組み込まないでください。
必ずプリペアドステートメントまたはORMを使用してください。
//}

//list[secure_query][安全なクエリの例][lang=javascript]{
// 危険な例（使用禁止）
const query = `SELECT * FROM users WHERE id = ${userId}`;

// 安全な例（推奨）
const query = 'SELECT * FROM users WHERE id = ?';
db.query(query, [userId], (error, results) => {
    // 処理
});
//}

== 複合的な使用例

=== APIドキュメンテーション

//info[エンドポイント情報]{
@<code>{POST /api/users}
新規ユーザーを作成します。
//}

//list[request_body][リクエストボディ][lang=json]{
{
  "name": "山田太郎",
  "email": "yamada@example.com",
  "password": "securePassword123"
}
//}

//list[response_success][成功レスポンス][lang=json]{
{
  "id": "usr_123456",
  "name": "山田太郎",
  "email": "yamada@example.com",
  "createdAt": "2024-01-01T00:00:00Z"
}
//}

//warning[認証必須]{
このエンドポイントにはBearerトークンによる認証が必要です。
ヘッダーに @<code>{Authorization: Bearer <token>} を含めてください。
//}

//tip[レート制限]{
APIのレート制限は1時間あたり1000リクエストです。
制限に達した場合は429ステータスコードが返されます。
//}

=== トラブルシューティングガイド

//important[よくある問題と解決方法]{
以下は頻繁に報告される問題とその解決方法です。
//}

//caution[接続エラー]{
「Connection refused」エラーが発生する場合：

 1. サービスが起動しているか確認
 2. ポート番号が正しいか確認
 3. ファイアウォール設定を確認
//}

//memo[デバッグ用コマンド]{
以下のコマンドで詳細なログを確認できます：

@<code>{DEBUG=* npm start}
//}

//notice[サポート情報]{
問題が解決しない場合は、以下の情報と共にサポートチームにお問い合わせください：

 * エラーメッセージの全文
 * 実行環境の情報
 * 再現手順
//}

== まとめ

本サンプルで紹介した機能：

=== noteブロック
 * 基本的なnote
 * タイトル付きnote
 * ラベル付きnote（参照可能）

=== ミニブロック（7種類）
 * @<code>{//info} - 一般的な情報
 * @<code>{//tip} - ヒントやコツ
 * @<code>{//memo} - メモや備忘録
 * @<code>{//warning} - 警告
 * @<code>{//caution} - 注意
 * @<code>{//important} - 重要事項
 * @<code>{//notice} - お知らせ

各ミニブロックは用途に応じて使い分けることで、
読者にとって分かりやすいドキュメントを作成できます。
アイコンや背景色による視覚的な区別により、
重要度や種類が一目で判断できるようになっています。