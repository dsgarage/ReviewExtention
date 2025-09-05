'use strict';

/**
 * Re:VIEW 向け textlint プリセット。
 * - review plugin の読み込み
 * - 日本語文章向けプリセット
 * - prh 連携（rulePaths はプロジェクト側で指定）
 */
module.exports = {
  plugins: ["review"],
  rules: {
    "preset-japanese": true,
    "spellcheck-tech-word": true,
    // prh は各プロジェクトの rulePaths を参照する想定
    "prh": { "rulePaths": [] }
  }
};
