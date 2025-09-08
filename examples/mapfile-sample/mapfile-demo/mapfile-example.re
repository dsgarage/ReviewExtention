= 外部ファイル参照機能のサンプル

== はじめに

このドキュメントでは、Re:VIEWの外部ファイル参照機能（#@mapfile、#@maprange、#@mapoutput）の使用例を示します。

== #@mapfile による全体参照

外部ファイルの内容全体を取り込む例です。

//listnum[complete_code][Calculator.cs の完全なコード]{
#@mapfile(code/sample.cs)
using System;
using System.Collections.Generic;

namespace SampleApp
{
    public class Calculator
    {
        // start_basic_operations
        public double Add(double a, double b)
        {
            return a + b;
        }

        public double Subtract(double a, double b)
        {
            return a - b;
        }

        public double Multiply(double a, double b)
        {
            return a * b;
        }

        public double Divide(double a, double b)
        {
            if (b == 0)
            {
                throw new DivideByZeroException("Division by zero is not allowed.");
            }
            return a / b;
        }
        // end_basic_operations

        // start_advanced
        public double Power(double baseNumber, double exponent)
        {
            return Math.Pow(baseNumber, exponent);
        }

        public double SquareRoot(double number)
        {
            if (number < 0)
            {
                throw new ArgumentException("Cannot calculate square root of negative number.");
            }
            return Math.Sqrt(number);
        }
        // end_advanced

        public double CalculateAverage(List<double> numbers)
        {
            if (numbers == null || numbers.Count == 0)
            {
                throw new ArgumentException("Cannot calculate average of empty list.");
            }

            double sum = 0;
            foreach (var num in numbers)
            {
                sum += num;
            }
            return sum / numbers.Count;
        }
    }
}
#@end
//}

== #@maprange による部分参照

=== 基本演算の部分のみ

マーカーコメントを使用して、ファイルの一部分だけを取り込みます。

//listnum[basic_ops][基本的な演算メソッド]{
//}

=== 高度な演算の部分

//listnum[advanced_ops][高度な演算メソッド]{
//}

== #@mapoutput によるコマンド出力

コマンドの実行結果を直接取り込むことができます。

//cmd[ディレクトリ内容]{
#@mapoutput(ls -la code/)
total 16
drwxr-xr-x   4 daisuketsukada  staff   128  9  8 12:46 .
drwxr-xr-x  18 daisuketsukada  staff   576  9  8 12:46 ..
-rw-r--r--   1 daisuketsukada  staff  1979  9  8 12:46 long-line-sample.cs
-rw-r--r--   1 daisuketsukada  staff  1564  9  8 12:25 sample.cs
#@end
//}

//cmd[ファイル情報]{
#@mapoutput(wc -l code/sample.cs)
      64 code/sample.cs
#@end
//}

== 実際の使用例

=== プロジェクト構成の説明

プロジェクトのファイル構成を示す際に便利です：

//cmd[プロジェクト構造]{
#@mapoutput(find . -type f -name "*.cs" | head -10)
./code/long-line-sample.cs
./code/sample.cs
#@end
//}

=== ビルド出力の記録

ビルドコマンドの出力を文書化できます：

//cmd[ビルド結果]{
#@mapoutput(echo "dotnet build output would appear here")
dotnet build output would appear here
#@end
//}

== 長い行の折り返し対応（Issue #20の回避策）

wrap/foldオプションは現在動作しませんが、LaTeXのlistingsパッケージ設定により対応可能です。

//listnum[long_lines][長い行を含むコード]{
#@mapfile(code/long-line-sample.cs)
using System;
using System.Collections.Generic;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Configuration;

namespace LongLineSample
{
    public class ComplexSystem
    {
        private readonly ILogger<ComplexSystem> _logger;
        private readonly IConfiguration _configuration;
        private readonly IServiceProvider _serviceProvider;

        // This is a very long constructor signature that would normally overflow the page width in PDF output without proper line wrapping configuration
        public ComplexSystem(ILogger<ComplexSystem> logger, IConfiguration configuration, IServiceProvider serviceProvider, IDataRepository dataRepository, ICacheService cacheService, INotificationService notificationService)
        {
            _logger = logger ?? throw new ArgumentNullException(nameof(logger), "Logger cannot be null and must be properly configured with appropriate log levels");
            _configuration = configuration ?? throw new ArgumentNullException(nameof(configuration), "Configuration is required for system initialization");
            _serviceProvider = serviceProvider ?? throw new ArgumentNullException(nameof(serviceProvider), "Service provider is essential for dependency injection");
        }

        // Another example of a long line with a complex LINQ query
        public IEnumerable<ProcessedDataItem> ProcessDataWithComplexFiltering(IEnumerable<RawDataItem> rawData)
        {
            return rawData.Where(item => item.IsValid && item.Timestamp > DateTime.Now.AddDays(-30) && item.Category == "Important" && item.Status != DataStatus.Deleted)
                          .Select(item => new ProcessedDataItem { Id = item.Id, Name = item.Name, ProcessedAt = DateTime.Now, AdditionalInfo = $"Processed from {item.Source} at {DateTime.Now:yyyy-MM-dd HH:mm:ss}" })
                          .OrderBy(item => item.ProcessedAt)
                          .ThenBy(item => item.Name);
        }
    }
}
#@end
//}

上記のコードは、sty/review-custom.styの設定により自動的に折り返されます。

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