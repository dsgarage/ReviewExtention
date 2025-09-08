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

          case key
          when 'wrap'
            options[:wrap] = value == 'on' || value == 'true' || value.to_i > 0
          when 'fold'
            options[:fold] = value == 'on' || value == 'true'
          when 'foldmark'
            options[:foldmark] = value
          when 'indent'
            options[:indent] = value.to_i
          when 'lineno'
            options[:lineno] = value == 'on' || value == 'true' || value.to_i > 0
          when 'fontsize'
            options[:fontsize] = value
          when 'lang', 'language'
            options[:lang] = value
          when 'filename'
            options[:filename] = value
          when 'highlight'
            options[:highlight] = value
          end
        end
        
        ListOptions.new(**options)
      end

      # Store options in thread-local variable for builder access
      def list_header(id, caption, lang)
        # Store options for builder
        if lang && lang.include?('=')
          options = parse_list_options(lang)
          Thread.current[:list_options] = options
          # Pass only actual language to original method
          actual_lang = options.lang
        else
          Thread.current[:list_options] = nil
          actual_lang = lang
        end
        
        super(id, caption, actual_lang)
      ensure
        Thread.current[:list_options] = nil
      end

      def list_body(id, lines, lang)
        super
      end

      def listnum_header(id, caption, lang)
        # Force line numbering
        if lang && lang.include?('=')
          options = parse_list_options(lang)
          options.lineno = true unless options.lineno
          Thread.current[:list_options] = options
          actual_lang = options.lang
        else
          Thread.current[:list_options] = ListOptions.new(lineno: true)
          actual_lang = lang
        end
        
        super(id, caption, actual_lang)
      ensure
        Thread.current[:list_options] = nil
      end

      def emlist(lines, caption = nil, lang = nil)
        if lang && lang.include?('=')
          options = parse_list_options(lang)
          Thread.current[:list_options] = options
          actual_lang = options.lang
        else
          Thread.current[:list_options] = nil
          actual_lang = lang
        end
        
        super(lines, caption, actual_lang)
      ensure
        Thread.current[:list_options] = nil
      end

      def cmd(lines, caption = nil, lang = nil)
        # Use smaller font for command output by default
        if lang && lang.include?('=')
          options = parse_list_options(lang)
          options.fontsize ||= 'small'
          Thread.current[:list_options] = options
          actual_lang = options.lang
        else
          Thread.current[:list_options] = ListOptions.new(fontsize: 'small')
          actual_lang = lang
        end
        
        super(lines, caption, actual_lang)
      ensure
        Thread.current[:list_options] = nil
      end
    end
  end

  class Compiler
    prepend CompilerExtension::ListExtension

    # Store original block handlers
    alias_method :original_list_block, :list_block if method_defined?(:list_block)
    alias_method :original_listnum_block, :listnum_block if method_defined?(:listnum_block)

    # Parse options from caption (backward compatibility)
    def parse_caption_options(caption)
      return [caption, nil] unless caption

      # Check if caption contains embedded options (space-separated)
      if caption.match(/\s+(wrap|fold|lineno|fontsize|lang|filename|highlight)=/)
        parts = caption.split(/\s+(?=\w+=)/, 2)
        if parts.size == 2
          actual_caption = parts[0].strip
          options_str = parts[1].strip
          options = parse_list_options(options_str)
          return [actual_caption, options]
        end
      end

      [caption, nil]
    end

    # Enhanced list block handler with standard 3rd parameter support
    def list_block(id, caption, lang)
      options_from_param = nil
      actual_lang = lang

      # First: Parse options from 3rd parameter (standard Re:VIEW way)
      if lang && lang.include?('=')
        options_from_param = parse_list_options(lang)
        actual_lang = options_from_param.lang
      end

      # Second: Check caption for embedded options (backward compatibility)
      actual_caption, options_from_caption = parse_caption_options(caption)

      # Third parameter takes precedence over caption
      final_options = options_from_param || options_from_caption
      
      Thread.current[:list_options] = final_options
      
      # Call original method with cleaned parameters
      if respond_to?(:original_list_block)
        original_list_block(id, actual_caption, actual_lang)
      end
    ensure
      Thread.current[:list_options] = nil
    end

    # Enhanced listnum block handler
    def listnum_block(id, caption, lang)
      options_from_param = nil
      actual_lang = lang

      # Parse options from 3rd parameter
      if lang && lang.include?('=')
        options_from_param = parse_list_options(lang)
        actual_lang = options_from_param.lang
      end

      # Check caption for embedded options
      actual_caption, options_from_caption = parse_caption_options(caption)
      
      # Merge options and force line numbering for listnum
      final_options = options_from_param || options_from_caption || ListOptions.new
      final_options.lineno = true unless final_options.lineno
      
      Thread.current[:list_options] = final_options
      
      # Call original method
      if respond_to?(:original_listnum_block)
        original_listnum_block(id, actual_caption, actual_lang)
      end
    ensure
      Thread.current[:list_options] = nil
    end
  end
end