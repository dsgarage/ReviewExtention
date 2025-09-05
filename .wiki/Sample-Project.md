# サンプルプロジェクト

このリポジトリには `examples/sample-book` が含まれており、Re:VIEW 5.x での利用を確認できます。

## 動かし方
```bash
cd examples/sample-book
npm run lint
gem install review
review-pdfmaker config.yml
```

## 検証内容
- Lint のエラー/警告が正しく出るか
- ReviewMCP と組み合わせたチェックが通るか
- Re:VIEW 5.6 / 5.7 / 5.8 での互換性
