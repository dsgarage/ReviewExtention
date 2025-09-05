# ReviewMCP との併用メリット

[ReviewMCP](https://github.com/dsgarage/ReviewMCP) は、Re:VIEW 5.8/2.5 向けに設計された **最小 MCP サーバ**です。
Re:VIEW Extensions と組み合わせることで、以下の利点があります。

## メリット
- **タグ検査の自動化**: Lint を超えて、章・節の ID や相互参照の一貫性をチェック
- **レビュー効率化**: CI/CD で自動実行し、原稿レビューを省力化
- **安全なマイグレーション**: 旧 Starter Template 由来の記法を使っていても MCP 側で検査・修正可能

## 推奨ワークフロー
1. **Lint**: textlint + Re:VIEW Extensions
2. **MCP**: ReviewMCP サーバで構造チェック
3. **Build**: Re:VIEW で PDF/HTML 出力

この三段階を CI に組み込むと、品質が大幅に安定します。
