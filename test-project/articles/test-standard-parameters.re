= Issue #30/#31 Test: Standard Parameter Processing

This test file demonstrates the fix for Issue #30 and #31 - proper handling of Re:VIEW's standard 3rd parameter.

== Standard 3rd Parameter Support

=== Basic wrap option in 3rd parameter

The following should work with standard Re:VIEW syntax:

//list[standard-wrap][Standard 3rd parameter wrap][wrap=80]{
This is a very long line of text that should be automatically wrapped when it exceeds 80 characters using the standard Re:VIEW 3rd parameter syntax.
//}

=== Multiple options in 3rd parameter

//list[multi-options][Multiple options in 3rd parameter][wrap=on,fold=on,foldmark=↩]{
public class VeryLongClassNameForDemonstrationPurposes implements InterfaceWithLongName, AnotherInterfaceWithEvenLongerName {
    private static final String VERY_LONG_CONSTANT_FOR_TESTING = "This demonstrates multiple options";
}
//}

=== Language with wrap options

//list[lang-wrap][Language and wrap options][wrap=80,lang=ruby]{
def very_long_method_name_with_many_parameters(first_param, second_param, third_param, fourth_param, fifth_param)
  puts "This is a Ruby method with automatic wrapping at 80 characters"
  return first_param + second_param + third_param
end
//}

=== listnum with standard parameters

//listnum[standard-listnum][Standard listnum with wrap][wrap=on,lineno=on,lang=python]{
def process_data_with_very_long_function_name(input_data, configuration_dict, additional_parameters):
    """This is a Python function that should have line numbers and wrapping."""
    processed_data = input_data.strip().lower().replace(" ", "_")
    return processed_data.upper()
//}

== Backward Compatibility Tests

The following should still work (caption-embedded options):

=== Caption-embedded wrap (backward compatibility)

//list[backward-compat][Caption with embedded options wrap=on,foldmark=→]{
const configuration = {
  apiEndpoint: "https://api.example.com/v1/very/long/endpoint/path",
  authentication: "Bearer very-long-token-string-for-testing-purposes"
};
//}

== Mixed Cases

=== 3rd parameter takes precedence over caption

//list[precedence][Caption wrap=off should be ignored][wrap=on,fontsize=small]{
// The 3rd parameter wrap=on should take precedence over caption wrap=off
function testPrecedence() { return "third parameter wins"; }
//}

== Complex Options

=== All options combined

//listnum[complex][Complex options test][wrap=80,fold=on,foldmark=↩,lineno=on,fontsize=small,lang=javascript]{
function processVeryLongDataWithMultipleParametersAndComplexLogic(inputData, configurationObject, additionalMetadata, processingOptions) {
    const result = inputData.map(item => item.value).filter(value => value !== null && value !== undefined);
    return result.reduce((accumulator, current) => accumulator + current, 0);
}
//}

== Expected Results

All the above code blocks should:

1. **Standard 3rd parameter processing**: Options in `[wrap=80,lang=ruby]` format should be parsed correctly
2. **Backward compatibility**: Caption-embedded options should still work
3. **Precedence handling**: 3rd parameter options take priority over caption-embedded options
4. **Clean language handling**: Only actual language names (ruby, python, etc.) should be passed to LaTeX listings
5. **No LaTeX errors**: No more "Couldn't load requested language" errors
6. **Proper line wrapping**: Long lines should wrap with red arrow indicators
7. **Line numbering**: `//listnum` should show line numbers regardless of other options