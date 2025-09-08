# frozen_string_literal: true

require 'review/compiler'
require 'review/book'

module ReVIEW
  module CompilerExtension
    module ListExtension
      # Extended list options structure
      ListOptions = Struct.new(
        :wrap,       # true/false/number (e.g., 80)
        :fold,       # true/false
        :foldmark,   # string (e.g., "↩", "...")
        :indent,     # number (indentation for wrapped lines)
        :lineno,     # true/false/number (line numbering)
        :fontsize,   # small/normal/large
        :lang,       # programming language for syntax highlighting
        :filename,   # display filename badge
        :highlight,  # lines to highlight (e.g., "3-5,7")
        keyword_init: true
      )

      # Parse options from the third argument of //list
      def parse_list_options(opt_str)
        return ListOptions.new unless opt_str && !opt_str.empty?

        options = {}
        opt_str.split(',').each do |opt|
          key, value = opt.strip.split('=', 2)
          next unless key

          key = key.to_sym
          # Convert string values to appropriate types
          value = case key
                  when :wrap
                    if value.nil? || value == 'on' || value == 'true'
                      true
                    elsif value == 'off' || value == 'false'
                      false
                    else
                      value.to_i
                    end
                  when :fold, :lineno
                    value.nil? || value == 'on' || value == 'true'
                  when :indent
                    value ? value.to_i : 20
                  when :foldmark, :fontsize, :lang, :filename, :highlight
                    value
                  else
                    value
                  end
          options[key] = value
        end
        
        ListOptions.new(**options)
      end

      # Wrap long lines if wrap option is specified
      def wrap_lines(lines, options)
        return lines unless options.wrap

        wrap_at = options.wrap.is_a?(Integer) ? options.wrap : 80
        foldmark = options.foldmark || '↩'
        indent = options.indent || 20

        wrapped = []
        lines.each do |line|
          if line.length <= wrap_at
            wrapped << line
          else
            # Split long lines
            pos = 0
            while pos < line.length
              if pos == 0
                wrapped << line[pos, wrap_at]
              else
                # Add indentation for wrapped lines
                wrapped << (' ' * indent) + line[pos, wrap_at - indent]
              end
              pos += (pos == 0 ? wrap_at : wrap_at - indent)
            end
          end
        end
        wrapped
      end
    end
  end

  class Compiler
    prepend CompilerExtension::ListExtension

    # Override the list block handler to support extended options
    alias_method :original_list_block, :read_block if method_defined?(:read_block)
    
    def list_block(name, args, lines)
      # Parse extended options from third argument
      options = parse_list_options(args[2]) if args.size >= 3
      
      # Apply line wrapping if specified
      if options && options.wrap
        lines = wrap_lines(lines, options)
      end
      
      # Store options in a thread-local variable for builders to access
      Thread.current[:list_options] = options
      
      # Call the original handler
      original_list_block(name, args[0..1], lines) if respond_to?(:original_list_block)
    ensure
      Thread.current[:list_options] = nil
    end
  end
end