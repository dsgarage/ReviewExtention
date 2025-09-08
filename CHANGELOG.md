# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.1] - 2024-09-08

### Fixed
- LaTeX自動折り返し（breaklines）が実際にPDFに反映されるよう修正
- review-custom.styでグローバル`\lstset`設定を適用
- 折り返し記号（↩）が正しく表示されるよう設定

### Changed
- sty/review-custom.styを簡潔化し、環境再定義による競合を回避

## [0.1.0] - 2024-09-08

### Added
- 初回リリース
- Ruby拡張によるハイブリッドパイプライン実装
- wrap/fold/linenoオプション対応
- セキュアな#@mapfile/#@maprange機能
- JS前処理 + Ruby Builderアーキテクチャ
- Makefile による統一ビルドターゲット

### Security
- ファイルサイズ制限（200KB）
- 拡張子ホワイトリスト
- パストラバーサル攻撃防止
- プロジェクトルート外アクセス禁止

[0.1.1]: https://github.com/dsgarage/ReviewExtention/compare/v0.1.0...v0.1.1
[0.1.0]: https://github.com/dsgarage/ReviewExtention/releases/tag/v0.1.0