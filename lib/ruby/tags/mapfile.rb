# frozen_string_literal: true

require 'pathname'
require 'review/preprocessor'

module ReVIEW
  module PreprocessorExtension
    module MapfileExtension
      # Security constraints
      MAX_FILE_SIZE = 200 * 1024  # 200KB
      ALLOWED_EXTENSIONS = %w[
        .rb .py .c .cpp .cc .cxx .h .hpp
        .java .js .ts .jsx .tsx .cs .go
        .rs .swift .kt .scala .php .pl
        .sh .bash .zsh .fish .ps1 .bat
        .sql .html .css .scss .sass .less
        .xml .json .yaml .yml .toml .ini
        .md .txt .re .tex .sty
      ].freeze

      # Preprocess directives
      def preproc_mapfile(param)
        path = parse_file_path(param)
        validate_and_read_file(path)
      end

      def preproc_maprange(param)
        parts = param.split(',').map(&:strip)
        path = parse_file_path(parts[0])
        from_marker = parts[1]
        to_marker = parts[2]
        
        content = validate_and_read_file(path)
        extract_range(content, from_marker, to_marker)
      end

      def preproc_mapoutput(param)
        # Security: Do not execute arbitrary commands
        # This should be disabled or heavily restricted
        warn "Warning: #@mapoutput is disabled for security reasons"
        "# mapoutput disabled for security"
      end

      private

      def parse_file_path(path_str)
        # Remove quotes if present
        path_str = path_str.strip.gsub(/^["']|["']$/, '')
        
        # Resolve path relative to current document
        base_dir = @basedir || Dir.pwd
        path = Pathname.new(path_str)
        
        # Make absolute if relative
        unless path.absolute?
          path = Pathname.new(base_dir) + path
        end
        
        path
      end

      def validate_and_read_file(path)
        # Security checks
        validate_path_security(path)
        validate_extension(path)
        validate_file_size(path)
        
        # Read file content
        content = File.read(path.to_s, encoding: 'UTF-8')
        
        # Handle BOM if present
        content = content.sub(/\A\xEF\xBB\xBF/, '') if content.start_with?("\xEF\xBB\xBF")
        
        content
      rescue Errno::ENOENT
        error "file not found: #{path}"
      rescue Errno::EACCES
        error "permission denied: #{path}"
      rescue => e
        error "failed to read file: #{path}: #{e.message}"
      end

      def validate_path_security(path)
        # Prevent path traversal attacks
        real_path = path.realpath rescue nil
        unless real_path
          error "invalid path: #{path}"
        end
        
        # Check if path is within project directory
        project_root = Pathname.new(@basedir || Dir.pwd).realpath
        unless real_path.to_s.start_with?(project_root.to_s)
          error "path outside project directory: #{path}"
        end
      end

      def validate_extension(path)
        ext = path.extname.downcase
        unless ALLOWED_EXTENSIONS.include?(ext)
          error "file type not allowed: #{ext} (#{path})"
        end
      end

      def validate_file_size(path)
        size = File.size(path)
        if size > MAX_FILE_SIZE
          error "file too large: #{size} bytes (max: #{MAX_FILE_SIZE} bytes)"
        end
      end

      def extract_range(content, from_marker, to_marker)
        lines = content.lines
        from_idx = nil
        to_idx = nil
        
        lines.each_with_index do |line, idx|
          if from_marker && line.include?(from_marker)
            from_idx = idx + 1
          end
          if to_marker && line.include?(to_marker)
            to_idx = idx
            break
          end
        end
        
        if from_marker && from_idx.nil?
          warn "Warning: start marker '#{from_marker}' not found"
          from_idx = 0
        end
        
        if to_marker && to_idx.nil?
          warn "Warning: end marker '#{to_marker}' not found"
          to_idx = lines.size
        end
        
        from_idx ||= 0
        to_idx ||= lines.size
        
        lines[from_idx...to_idx].join
      end
    end
  end

  class Preprocessor
    # Include the mapfile extension
    prepend PreprocessorExtension::MapfileExtension
    
    # Register new directives
    def initialize(*args)
      super
      # Register custom directives if not already registered
      @leave_content = true  # Preserve content by default
    end
    
    # Override the directive handler to support our custom directives
    alias_method :original_replace_block, :replace_block if method_defined?(:replace_block)
    
    def replace_block(f, directive_name, args, lines)
      case directive_name
      when 'mapfile'
        content = preproc_mapfile(args)
        content.lines
      when 'maprange'
        content = preproc_maprange(args)
        content.lines
      when 'mapoutput'
        content = preproc_mapoutput(args)
        content.lines
      else
        if respond_to?(:original_replace_block)
          original_replace_block(f, directive_name, args, lines)
        else
          lines
        end
      end
    end
  end
end