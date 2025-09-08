⨯ ERROR   test-extensions.re:110: wrong arg size
⨯ ERROR   test-extensions.re:111: unbaranced #@end
= Ruby拡張機能テスト

== wrap/fold機能のテスト

=== 基本的な自動折り返し

//list[wrap_basic][長い行の自動折り返し][wrap=on,fold=on,foldmark=↩]{
public class VeryLongClassNameForDemonstrationPurposes implements InterfaceWithLongName, AnotherInterfaceWithEvenLongerName, YetAnotherVeryVeryLongInterfaceName {
    private static final String VERY_LONG_CONSTANT_NAME_FOR_DEMONSTRATION = "This is a very long string constant that demonstrates how automatic line wrapping works";
}
//}

=== 80桁での厳密な折り返し

//list[wrap_80][80桁での折り返し][wrap=80,foldmark=→]{
def process_complex_data_with_very_long_method_name(first_parameter, second_parameter, third_parameter, fourth_parameter):
    """This is a very long docstring that should be wrapped at exactly 80 characters."""
    return first_parameter + second_parameter
//}

=== 行番号とインデント

//listnum[lineno_indent][行番号付きコード][wrap=on,lineno=on,indent=20]{
function complexFunction(param1, param2, param3) {
    const configuration = { endpoint: "https://api.example.com/v1/very/long/endpoint", token: "Bearer token" };
    return fetch(configuration.endpoint);
}
//}

== #@mapfile機能のテスト

=== 外部ファイルの取り込み

//list[mapfile_test][hello.rbの内容]{
#@mapfile(../test/fixtures/hello.rb)
#!/usr/bin/env ruby
# frozen_string_literal: true

# Sample Ruby file for testing #@mapfile functionality

class HelloWorld
  # start_basic_operations
  def initialize(name = 'World')
    @name = name
    @greetings = []
  end

  def greet
    message = "Hello, #{@name}!"
    @greetings << message
    puts message
    message
  end

  def farewell
    message = "Goodbye, #{@name}!"
    @greetings << message
    puts message
    message
  end
  # end_basic_operations

  # start_advanced
  def greet_multiple(names)
    names.each do |name|
      puts "Hello, #{name}!"
    end
  end

  def custom_greeting(template)
    message = template.gsub('{name}', @name)
    @greetings << message
    puts message
    message
  end
  # end_advanced

  def history
    @greetings
  end

  private

  def format_message(text)
    "[#{Time.now}] #{text}"
  end
end

# Example usage
if __FILE__ == $0
  hw = HelloWorld.new('Re:VIEW')
  hw.greet
  hw.farewell
  hw.greet_multiple(['Alice', 'Bob', 'Charlie'])
  puts "History: #{hw.history.inspect}"
end
#@end
//}

=== C#ファイルの取り込みテスト

//list[cs_test][Calculator.csの内容]{
#@mapfile(../examples/mapfile-sample/code/sample.cs)
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

=== C#ファイルの部分取り込み

//list[cs_range][Calculator.csの基本演算部分]{
//}

== ファイル名バッジ表示

//list[with_filename][ファイル名付きコード][filename=app/models/user.rb,wrap=on,lang=ruby]{
class User < ApplicationRecord
  validates :email, presence: true, uniqueness: true
  has_many :posts, dependent: :destroy
end
//}