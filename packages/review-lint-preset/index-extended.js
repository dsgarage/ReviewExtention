/**
 * Re:VIEW textlint preset with extended rules
 * Includes list unification and Starter-derived policies
 */

import noDeprecatedList from './rules/no-deprecated-list.js';

export default {
  plugins: {
    "@textlint/text": true,
    "@textlint/markdown": true,
    "review": true
  },
  
  rules: {
    // 日本語基本ルール
    "preset-japanese": true,
    "no-doubled-joshi": {
      "min_interval": 1
    },
    "no-mixed-zenkaku-and-hankaku-alphabet": true,
    "sentence-length": {
      "max": 120
    },
    
    // 技術用語スペルチェック
    "spellcheck-tech-word": true,
    
    // prh による用語統一
    "prh": {
      "rulePaths": []  // 使用時に指定
    },
    
    // カスタムルール（リスト統一）
    "@dsgarage/no-deprecated-list": {
      "severity": "warning"
    },
    
    // その他のカスタムルール
    "@dsgarage/no-raw-url": {
      "severity": "warning",
      "message": "生URLは脚注化を推奨: [text](url) 形式に変更してください"
    },
    "@dsgarage/prefer-term-tag": {
      "severity": "warning",
      "message": "索引化する用語は @@<term>{語|よみ} を使用してください"
    },
    "@dsgarage/no-nested-blocks-violate": {
      "severity": "warning",
      "message": "ブロックの入れ子が深すぎます。構造を見直してください"
    }
  },
  
  // カスタムルールローダー
  rulesConfig: {
    "@dsgarage/no-deprecated-list": noDeprecatedList
  }
};