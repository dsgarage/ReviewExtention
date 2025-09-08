= Issue #28 Test: English Code Wrapping

This test file demonstrates the fix for Issue #28, where English code with wrap options was failing.

== Test Cases

=== Basic English Code with Wrap

The following code should wrap at 80 characters:

//list[english-wrap][English code with wrap=80]{
This is a very long line of English text that should be automatically wrapped when it exceeds 80 characters in the PDF output.
//}

=== English Code with Multiple Options

//list[multi-options][Multiple options including wrap and language][wrap=on,lang=ruby]{
def calculate_something_with_a_very_long_method_name(parameter_one, parameter_two, parameter_three, parameter_four)
  result = parameter_one + parameter_two + parameter_three + parameter_four
  return result
end
//}

=== Long URL Wrapping

//list[url-wrap][Long URL that needs wrapping][wrap=on]{
https://github.com/dsgarage/ReviewExtention/issues/28-this-is-a-very-long-url-that-should-be-wrapped-automatically-when-rendered-in-pdf
//}

=== Code with Line Numbers and Wrap

//listnum[numbered-wrap][Code with line numbers and wrap][wrap=on,lang=python]{
def process_data_with_very_long_function_name(input_data, configuration_settings, additional_parameters):
    """This is a docstring that explains what this function does in great detail."""
    processed = input_data.strip().lower().replace(" ", "_")
    return processed
//}

=== Command Output with Wrap

//cmd[cmd-wrap][Long command output][wrap=on]{
$ docker run --rm -it -v /Users/very/long/path/to/project:/app -w /app node:latest npm install --save-dev @types/node @types/express typescript eslint prettier
//}

== Expected Results

All the above code blocks should:
1. Wrap long lines automatically
2. Show a red arrow (↩) at wrap points
3. Indent wrapped lines by 20pt
4. Not show "Couldn't load requested language" errors