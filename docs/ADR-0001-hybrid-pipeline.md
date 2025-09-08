# ADR-0001: Hybrid Pipeline (JS Preprocessor + Ruby Builder)

## Status
Accepted

## Context
Re:VIEW の拡張方言（wrap/fold, note/miniblock, #@mapfile等）を処理するために、複数のアプローチを検討：
1. JS のみで完結（現状）
2. Ruby のみで完結  
3. JS（前処理）+ Ruby（最終出力）のハイブリッド

現状の課題：
- JS での PDF 品質制御が困難（LaTeX エスケープ、日本語処理）
- Ruby Builder の拡張が Re:VIEW 本体バージョンに依存
- MCP と手動ビルドで異なる処理パスになりがち

## Decision
**JS（前段）は入力正規化/メタ付与に特化し、Ruby Builder が最終出力責務を担って PDF 品質を確定する**

責務分担：
- **JS Preprocessor**: 
  - 入力正規化（//emlist→//list統一等）
  - メタデータ付与（#@# コメント）
  - 統計/dry-run 機能
- **Ruby Builder**:
  - LaTeX/HTML の最終レンダリング
  - wrap/fold の実装（listings パッケージ制御）
  - セキュアな #@mapfile 処理

## Consequences

### Positive
- デバッグが容易（各段階の出力を確認可能）
- MCP/手動ビルドのパリティ確保
- PDF 品質の決定的制御
- 将来の拡張性（各層独立して改善可能）

### Negative  
- パイプラインが複雑化
- Node と Ruby 両方の実行環境が必要
- 初期セットアップのハードル上昇

### Risks
- Re:VIEW 本体のアップデートで Ruby 拡張が壊れる可能性
  - 緩和策: バージョン固定、CI での継続的テスト

## Implementation
1. Makefile でパイプライン定義
2. review-ext.rb をエントリポイントに
3. lib/ruby/ 以下に Builder 拡張を配置
4. CI で Node + Ruby + TeX Live の行列テスト