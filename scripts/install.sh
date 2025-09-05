#!/usr/bin/env bash
set -euo pipefail

# プロジェクト直下で実行してください。
if [ ! -d "vendor/review-extensions" ]; then
  echo "[INFO] サブモジュールを追加します..."
  git submodule add https://github.com/your-org/review-extensions vendor/review-extensions
fi

echo "[INFO] textlint 関連をインストールします..."
npm i -D textlint textlint-plugin-review textlint-rule-preset-japanese textlint-rule-prh textlint-rule-spellcheck-tech-word

if [ ! -f ".textlintrc.json" ]; then
cat <<'JSON' > .textlintrc.json
{
  "extends": ["vendor/review-extensions/packages/review-lint-preset"],
  "rules": {
    "prh": { "rulePaths": ["vendor/review-extensions/packages/review-prh-dict/prh.yml"] }
  }
}
JSON
  echo "[INFO] .textlintrc.json を作成しました。"
fi

echo "[DONE] セットアップ完了。npm run lint を実行して動作確認してください。"
