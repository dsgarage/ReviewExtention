# frozen_string_literal: true

require 'review/htmlbuilder'
require 'cgi'

module ReVIEW
  class HTMLBuilder
    # Store original methods
    alias_method :original_list, :list if method_defined?(:list)
    alias_method :original_listnum, :listnum if method_defined?(:listnum)
    alias_method :original_emlist, :emlist if method_defined?(:emlist)
    alias_method :original_cmd, :cmd if method_defined?(:cmd)

    # Generate CSS classes and data attributes from options
    def generate_list_attributes(options)
      return {} unless options

      classes = ['code-block']
      data_attrs = {}
      style_parts = []

      # Wrap/fold options
      if options.wrap
        classes << 'wrap-enabled'
        style_parts << 'white-space: pre-wrap'
        style_parts << 'word-break: break-word'
        style_parts << 'overflow-wrap: break-word'
        
        if options.wrap.is_a?(Integer)
          data_attrs['data-wrap-at'] = options.wrap.to_s
          # Approximate character width (not perfect for proportional fonts)
          style_parts << "max-width: #{options.wrap}ch"
        end
      else
        style_parts << 'white-space: pre'
        style_parts << 'overflow-x: auto'
      end

      # Fold mark
      if options.foldmark
        data_attrs['data-foldmark'] = CGI.escapeHTML(options.foldmark)
      end

      # Line numbering
      if options.lineno
        classes << 'line-numbers'
        if options.lineno.is_a?(Integer)
          data_attrs['data-line-start'] = options.lineno.to_s
        end
      end

      # Font size
      case options.fontsize
      when 'small'
        style_parts << 'font-size: 0.9em'
      when 'large'
        style_parts << 'font-size: 1.2em'
      end

      # Programming language
      if options.lang
        classes << "language-#{options.lang}"
        data_attrs['data-lang'] = options.lang
      end

      # Filename badge
      if options.filename
        data_attrs['data-filename'] = CGI.escapeHTML(options.filename)
      end

      # Highlighted lines
      if options.highlight
        data_attrs['data-highlight'] = options.highlight
      end

      {
        class: classes.join(' '),
        style: style_parts.empty? ? nil : style_parts.join('; '),
        data: data_attrs
      }
    end

    # Generate opening div tag with attributes
    def list_opening_tag(attrs)
      parts = ["<div"]
      parts << %Q( class="#{attrs[:class]}") if attrs[:class]
      parts << %Q( style="#{attrs[:style]}") if attrs[:style]
      attrs[:data]&.each do |key, value|
        parts << %Q( #{key}="#{value}")
      end
      parts << ">"
      parts.join
    end

    # Enhanced list handler with wrap/fold support
    def list(lines, id, caption, lang = nil)
      options = Thread.current[:list_options]
      attrs = generate_list_attributes(options)

      # Add filename badge if specified
      if options && options.filename
        puts %Q(<div class="code-filename">#{CGI.escapeHTML(options.filename)}</div>)
      end

      # Generate wrapping div with attributes
      puts list_opening_tag(attrs)

      # Add custom CSS if needed
      if options && options.wrap
        puts <<~CSS
          <style>
            .wrap-enabled::after {
              content: attr(data-foldmark);
              color: #999;
              font-size: 0.8em;
              position: absolute;
              right: 0;
            }
          </style>
        CSS
      end

      # Call original method
      original_list(lines, id, caption, lang)

      puts "</div>"
    end

    # Enhanced listnum handler
    def listnum(lines, id, caption, lang = nil)
      options = Thread.current[:list_options]
      
      # Force line numbering for listnum
      if options
        options = options.dup
        options.lineno = true unless options.lineno
      else
        options = CompilerExtension::ListExtension::ListOptions.new(lineno: true)
      end
      
      Thread.current[:list_options] = options
      list(lines, id, caption, lang)
    ensure
      Thread.current[:list_options] = nil
    end

    # Enhanced emlist handler
    def emlist(lines, caption = nil, lang = nil)
      options = Thread.current[:list_options]
      attrs = generate_list_attributes(options)

      puts list_opening_tag(attrs)
      original_emlist(lines, caption, lang)
      puts "</div>"
    end

    # Enhanced cmd handler
    def cmd(lines, caption = nil, lang = nil)
      options = Thread.current[:list_options]
      
      # Use smaller font for command output by default
      if options
        options = options.dup
        options.fontsize ||= 'small'
      else
        options = CompilerExtension::ListExtension::ListOptions.new(fontsize: 'small')
      end
      
      Thread.current[:list_options] = options
      attrs = generate_list_attributes(options)

      puts list_opening_tag(attrs.merge(class: "#{attrs[:class]} command-output"))
      original_cmd(lines, caption, lang)
      puts "</div>"
    ensure
      Thread.current[:list_options] = nil
    end
  end
end

# Add default CSS for wrap/fold functionality
module ReVIEW
  class HTMLBuilder
    # Inject CSS into HTML head
    def html_head_add
      return unless Thread.current[:reviewext_css_injected].nil?
      Thread.current[:reviewext_css_injected] = true
      
      <<~CSS
        <style>
          /* ReviewExtention: Code block wrapping styles */
          .code-block {
            position: relative;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 10px;
            margin: 1em 0;
            background: #f8f8f8;
          }
          
          .code-block.wrap-enabled {
            white-space: pre-wrap;
            word-break: break-word;
            overflow-wrap: break-word;
            hyphens: auto;
          }
          
          .code-block .code-filename {
            background: #333;
            color: #fff;
            padding: 4px 8px;
            font-size: 0.9em;
            font-family: monospace;
            border-radius: 4px 4px 0 0;
            margin: -10px -10px 10px -10px;
          }
          
          .code-block.line-numbers {
            counter-reset: line-number;
            padding-left: 3em;
          }
          
          .code-block.line-numbers > span::before {
            counter-increment: line-number;
            content: counter(line-number);
            display: inline-block;
            width: 2em;
            margin-left: -3em;
            margin-right: 1em;
            text-align: right;
            color: #999;
            font-size: 0.9em;
          }
          
          .command-output {
            background: #000;
            color: #0f0;
            font-family: 'Courier New', monospace;
          }
        </style>
      CSS
    end
  end
end