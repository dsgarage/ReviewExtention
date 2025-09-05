# Lint / 辞書のカスタマイズ

Re:VIEW Extensions は textlint をベースにしています。
各プロジェクトごとにルールや辞書を拡張できます。

## カスタムルール追加（例）
```json
{
  "rules": {
    "max-ten": { "max": 3 },
    "sentence-length": { "max": 100 }
  }
}
```

## 辞書 (prh) の拡張（例）
`prh.yml` に追記してプロジェクト固有の表記揺れを吸収できます。

```yaml
rules:
  - expected: アルゴリズム
    patterns: [アルゴリスム, アルゴリズムス]
```
