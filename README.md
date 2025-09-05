# Re:VIEW Extensions

Re:VIEW Extensions は、[Re:VIEW Starter Template](https://github.com/dsgarage/ReVIEWStarterTemplate) 由来の拡張機能を独立再構築したリポジトリです。
Re:VIEW 5.x (5.6/5.7/5.8 以降) で安定利用できる **Lint プリセット・用語辞書・著者ガイド・最小マクロ互換** を提供します。

## 構成
```
packages/
  review-lint-preset/     # textlint プリセット（.re/.md）
  review-prh-dict/        # 用語統一辞書 (prh.yml)
  review-authoring-guides/# 著者向けガイド（差分タグの代替ポリシー）
  review-macro-shims/     # 任意の簡易前処理（最小・安全）
examples/
  sample-book/            # Re:VIEW 5.x 用の検証用プロジェクト
.github/workflows/ci.yml  # Node(18/20)×Re:VIEW(5.6/5.7/5.8) のCI
scripts/install.sh        # 既存プロジェクトへの導入ヘルパ
```

## クイックセットアップ（既存プロジェクト）
1) サブモジュール追加
```bash
git submodule add https://github.com/your-org/review-extensions vendor/review-extensions
```

2) Node 依存をインストール（推奨: Node 18+）
```bash
npm i -D textlint textlint-plugin-review textlint-rule-preset-japanese textlint-rule-prh textlint-rule-spellcheck-tech-word
```

3) `.textlintrc.json` を追加
```json
{
  "extends": ["vendor/review-extensions/packages/review-lint-preset"],
  "rules": {
    "prh": { "rulePaths": ["vendor/review-extensions/packages/review-prh-dict/prh.yml"] }
  }
}
```

4) Lint 実行
```bash
npm run lint
npm run lint:fix
```

5) Re:VIEW 本体でビルド
```bash
gem install review
review-pdfmaker config.yml
```

## ドキュメント
- 詳細は [GitHub Wiki](../../wiki) を参照してください。
