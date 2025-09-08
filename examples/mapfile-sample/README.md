# 外部ファイル参照機能サンプル

このディレクトリには、Re:VIEWの外部ファイル参照機能（`#@mapfile`、`#@maprange`、`#@mapoutput`）のサンプルが含まれています。

## ファイル構成

```
mapfile-sample/
├── README.md           # このファイル
├── mapfile-example.re  # 外部ファイル参照を使用したRe:VIEW文書
└── code/
    └── sample.cs       # 参照されるC#ソースコード
```

## 使用方法

### 1. プリプロセッサで展開

```bash
review-preprocess --replace mapfile-example.re
```

### 2. PDFへのビルド

```bash
review-pdfmaker config.yml
```

## 機能説明

### #@mapfile
ファイル全体を取り込みます：
```review
//listnum[label][caption]{
#@mapfile(code/sample.cs)
#@end
//}
```

### #@maprange
ファイルの一部分を取り込みます：
```review
//listnum[label][caption]{
#@maprange(code/sample.cs,start_marker,end_marker)
#@end
//}
```

### #@mapoutput
コマンドの実行結果を取り込みます：
```review
//cmd[label][caption]{
#@mapoutput(ls -la)
#@end
//}
```

## トラブルシューティング

もし外部ファイル参照が機能しない場合：

1. Re:VIEWのバージョンが5.0以降であることを確認
2. `review-preprocess`コマンドが利用可能か確認
3. ファイルパスが正しいか確認
4. 参照先ファイルが存在するか確認

## 関連ドキュメント

詳細については、[Wiki List Features](../../.wiki/List-Features.md)を参照してください。