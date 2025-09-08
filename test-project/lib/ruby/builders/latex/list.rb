# frozen_string_literal: true

require 'review/latexbuilder'
require_relative '../../tags/list'

module ReVIEW
  class LATEXBuilder
    # Store original methods
    alias_method :original_list, :list if method_defined?(:list)
    alias_method :original_listnum, :listnum if method_defined?(:listnum)
    alias_method :original_emlist, :emlist if method_defined?(:emlist)
    alias_method :original_cmd, :cmd if method_defined?(:cmd)

    # Parse options from language parameter string
    def parse_language_options(lang_str)
      options = {}
      return options unless lang_str
      
      # Split by comma and parse each option
      lang_str.split(',').each do |opt|
        opt = opt.strip
        if opt.include?('=')
          key, value = opt.split('=', 2)
          key = key.strip.to_sym
          value = value.strip
          
          # Convert to appropriate type
          case key
          when :wrap
            options[:wrap] = value == 'on' || value == 'true' || value.to_i > 0
          when :fold
            options[:fold] = value == 'on' || value == 'true'
          when :lineno
            options[:lineno] = value == 'on' || value == 'true' || value.to_i > 0
          when :indent
            options[:indent] = value.to_i
          when :lang, :language
            options[:lang] = value
          when :fontsize
            options[:fontsize] = value
          when :filename
            options[:filename] = value
          when :foldmark
            options[:foldmark] = value
          when :highlight
            options[:highlight] = value
          end
        end
      end
      
      options
    end
    
    # Create options object from parsed hash
    def create_options_from_hash(hash)
      return nil if hash.empty?
      
      # Use the ListOptions class from CompilerExtension
      CompilerExtension::ListExtension::ListOptions.new(**hash)
    end
    
    # Merge existing options with parsed options
    def merge_options(existing, parsed)
      return existing unless parsed && !parsed.empty?
      
      # Create a new options object with merged values
      merged = existing.dup
      parsed.each do |key, value|
        if merged.respond_to?("#{key}=")
          merged.send("#{key}=", value)
        end
      end
      merged
    end
    
    # Generate lstset parameters from options
    def generate_lstset_params(options)
      return nil unless options

      params = []
      
      # Line wrapping
      if options.wrap
        params << 'breaklines=true'
        params << 'breakatwhitespace=false'  # Break anywhere, not just at whitespace
        params << 'columns=fullflexible'
      end
      
      # Fold mark
      if options.foldmark
        # Escape special LaTeX characters
        mark = escape(options.foldmark)
        params << "postbreak=\\mbox{\\textcolor{red}{#{mark}}\\space}"
      elsif options.wrap
        # Default fold mark when wrap is enabled
        params << 'postbreak=\mbox{\textcolor{red}{$\hookrightarrow$}\space}'
      end
      
      # Indentation for wrapped lines
      if options.indent
        params << "breakindent=#{options.indent}pt"
      elsif options.wrap
        params << 'breakindent=20pt'  # Default indent
      end
      
      # Line numbering
      if options.lineno
        if options.lineno.is_a?(Integer)
          params << 'numbers=left'
          params << "firstnumber=#{options.lineno}"
        else
          params << 'numbers=left'
        end
      end
      
      # Font size
      case options.fontsize
      when 'small'
        params << 'basicstyle=\ttfamily\small'
      when 'large'
        params << 'basicstyle=\ttfamily\large'
      else
        params << 'basicstyle=\ttfamily'
      end
      
      # Programming language for syntax highlighting
      if options.lang
        lang_map = {
          'ruby' => 'Ruby',
          'python' => 'Python',
          'java' => 'Java',
          'javascript' => 'Java',  # Use Java as approximation
          'typescript' => 'Java',
          'c' => 'C',
          'cpp' => 'C++',
          'csharp' => '[Sharp]C',
          'sql' => 'SQL',
          'bash' => 'bash',
          'sh' => 'sh'
        }
        if lang_map[options.lang.downcase]
          params << "language=#{lang_map[options.lang.downcase]}"
        end
      end
      
      # Additional settings
      params << 'keepspaces=true'
      params << 'showstringspaces=false'
      params << 'frame=single'
      params << 'framesep=5pt'
      params << 'xleftmargin=0.5em'
      params << 'xrightmargin=0.5em'
      params << 'extendedchars=true'
      params << 'inputencoding=utf8'
      
      params.empty? ? nil : params.join(",\n  ")
    end

    # Enhanced list handler with wrap/fold support
    def list(lines, id, caption, lang = nil)
      options = Thread.current[:list_options]
      
      # Parse options from language parameter if present
      actual_lang = lang
      if lang && lang.include?('=')
        # Language parameter contains options like "wrap=80,lang=ruby"
        parsed_options = parse_language_options(lang)
        actual_lang = parsed_options[:lang]
        
        # Merge with existing options
        if options
          options = merge_options(options, parsed_options)
        else
          options = create_options_from_hash(parsed_options)
        end
      end
      
      if highlight_listings?
        # Use listings package with custom settings
        lstset_params = generate_lstset_params(options)
        
        if lstset_params
          # Apply custom settings for this specific listing
          puts "\\lstset{%"
          puts "  #{lstset_params}"
          puts "}%"
        end
        
        # Add filename badge if specified
        if options && options.filename
          puts "\\begin{tcolorbox}[title={\\scriptsize\\ttfamily #{escape(options.filename)}}]"
        end
        
        # Call original method with actual language only
        original_list(lines, id, caption, actual_lang)
        
        if options && options.filename
          puts "\\end{tcolorbox}"
        end
        
        # Reset to default settings
        if lstset_params
          puts "\\lstset{basicstyle=\\ttfamily}%"  # Reset to defaults
        end
      else
        # Fallback to original implementation
        original_list(lines, id, caption, actual_lang)
      end
    end

    # Enhanced listnum handler
    def listnum(lines, id, caption, lang = nil)
      options = Thread.current[:list_options]
      
      # Parse options from language parameter if present
      actual_lang = lang
      if lang && lang.include?('=')
        parsed_options = parse_language_options(lang)
        actual_lang = parsed_options[:lang]
        
        # Merge with existing options
        if options
          options = merge_options(options, parsed_options)
        else
          options = create_options_from_hash(parsed_options)
        end
      end
      
      # Force line numbering for listnum
      if options
        if options.is_a?(Struct)
          if !options.lineno
            options.lineno = true
          end
        else
          options = options.dup
          options.lineno = true unless options.lineno
        end
      else
        options = create_options_from_hash({lineno: true})
      end
      
      Thread.current[:list_options] = options
      list(lines, id, caption, actual_lang)
    ensure
      Thread.current[:list_options] = nil
    end

    # Enhanced emlist handler
    def emlist(lines, caption = nil, lang = nil)
      options = Thread.current[:list_options]
      
      # Parse options from language parameter if present
      actual_lang = lang
      if lang && lang.include?('=')
        parsed_options = parse_language_options(lang)
        actual_lang = parsed_options[:lang]
        
        # Merge with existing options
        if options
          options = merge_options(options, parsed_options)
        else
          options = create_options_from_hash(parsed_options)
        end
      end
      
      if highlight_listings?
        lstset_params = generate_lstset_params(options)
        
        if lstset_params
          puts "\\lstset{%"
          puts "  #{lstset_params}"
          puts "}%"
        end
        
        original_emlist(lines, caption, actual_lang)
        
        if lstset_params
          puts "\\lstset{basicstyle=\\ttfamily}%"
        end
      else
        original_emlist(lines, caption, actual_lang)
      end
    end

    # Enhanced cmd handler
    def cmd(lines, caption = nil, lang = nil)
      options = Thread.current[:list_options]
      
      # Parse options from language parameter if present
      actual_lang = lang
      if lang && lang.include?('=')
        parsed_options = parse_language_options(lang)
        actual_lang = parsed_options[:lang]
        
        # Merge with existing options
        if options
          options = merge_options(options, parsed_options)
        else
          options = create_options_from_hash(parsed_options)
        end
      end
      
      # Use smaller font for command output by default
      if options
        if options.is_a?(Struct)
          if !options.fontsize
            options.fontsize = 'small'
          end
        else
          options = options.dup
          options.fontsize ||= 'small'
        end
      else
        options = create_options_from_hash({fontsize: 'small'})
      end
      
      Thread.current[:list_options] = options
      
      if highlight_listings?
        lstset_params = generate_lstset_params(options)
        
        if lstset_params
          puts "\\lstset{%"
          puts "  #{lstset_params}"
          puts "}%"
        end
        
        original_cmd(lines, caption, actual_lang)
        
        if lstset_params
          puts "\\lstset{basicstyle=\\ttfamily}%"
        end
      else
        original_cmd(lines, caption, actual_lang)
      end
    ensure
      Thread.current[:list_options] = nil
    end

    # Helper method to check if listings highlighting is enabled
    def highlight_listings?
      @book.config['highlight'] && @book.config['highlight']['latex'] == 'listings'
    end
  end
end