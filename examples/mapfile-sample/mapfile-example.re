= 外部ファイル参照機能のサンプル

== はじめに

このドキュメントでは、Re:VIEWの外部ファイル参照機能（#@mapfile、#@maprange、#@mapoutput）の使用例を示します。

== #@mapfile による全体参照

外部ファイルの内容全体を取り込む例です。

//listnum[complete_code][Calculator.cs の完全なコード]{
#@mapfile(code/sample.cs)
#@end
//}

== #@maprange による部分参照

=== 基本演算の部分のみ

マーカーコメントを使用して、ファイルの一部分だけを取り込みます。

//listnum[basic_ops][基本的な演算メソッド]{
#@maprange(code/sample.cs,start_basic_operations,end_basic_operations)
#@end
//}

=== 高度な演算の部分

//listnum[advanced_ops][高度な演算メソッド]{
#@maprange(code/sample.cs,start_advanced,end_advanced)
#@end
//}

== #@mapoutput によるコマンド出力

コマンドの実行結果を直接取り込むことができます。

//cmd[directory_listing][ディレクトリ内容]{
#@mapoutput(ls -la code/)
#@end
//}

//cmd[file_info][ファイル情報]{
#@mapoutput(wc -l code/sample.cs)
#@end
//}

== 実際の使用例

=== プロジェクト構成の説明

プロジェクトのファイル構成を示す際に便利です：

//cmd[project_structure][プロジェクト構造]{
#@mapoutput(find . -type f -name "*.cs" | head -10)
#@end
//}

=== ビルド出力の記録

ビルドコマンドの出力を文書化できます：

//cmd[build_output][ビルド結果]{
#@mapoutput(echo "dotnet build output would appear here")
#@end
//}

== メリット

 1. **メンテナンス性向上**：コードとドキュメントの同期が保たれる
 2. **DRY原則**：同じコードを複数箇所に記載する必要がない
 3. **自動更新**：ソースコードの変更が自動的にドキュメントに反映される
 4. **エラー削減**：手動コピーペーストによる間違いを防げる

== 注意点

 * 参照先ファイルは必ず存在する必要がある
 * ファイルパスは相対パスまたは絶対パスで指定
 * プリプロセッサ実行時に展開される
 * 大きなファイルを参照する場合はビルド時間に注意

== まとめ

外部ファイル参照機能を使用することで、技術文書の品質とメンテナンス性が大幅に向上します。
特に、頻繁に更新されるコードサンプルを含む文書では必須の機能と言えるでしょう。