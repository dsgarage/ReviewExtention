#!/usr/bin/env ruby
# frozen_string_literal: true

# Re:VIEW Extension Hub
# This file loads all Ruby extensions for Re:VIEW 5.8
# 
# Architecture: Hybrid Pipeline (JS preprocessor + Ruby Builder)
# - JS: Input normalization and metadata annotation
# - Ruby: Final rendering for PDF/HTML quality
# Priority: PDF build success > HTML/EPUB parity

require 'review'

# Version check
unless ReVIEW::VERSION >= '5.8'
  warn "Warning: This extension is designed for Re:VIEW 5.8 or later (current: #{ReVIEW::VERSION})"
end

# Load order matters: tags → builders → hooks

# Tag extensions (Compiler extensions)
require_relative 'lib/ruby/tags/list'
require_relative 'lib/ruby/tags/mapfile'

# Builder extensions
require_relative 'lib/ruby/builders/latex/list'
require_relative 'lib/ruby/builders/html/list'

# Future extensions (uncomment as implemented):
# require_relative 'lib/ruby/builders/latex/note'
# require_relative 'lib/ruby/builders/latex/miniblock'
# require_relative 'lib/ruby/builders/epub/list'

# Post-processing hooks (minimal use)
# require_relative 'lib/hooks/beforetexcompile' # Enable when needed

puts "ReviewExtention loaded successfully (Re:VIEW #{ReVIEW::VERSION})" if ENV['DEBUG']