# frozen_string_literal: true

# Re:VIEW Extension - Automatic loader for Re:VIEW projects
# This file is automatically loaded when placed in the project's lib directory

# Load all extensions
require_relative 'ruby/tags/list'
require_relative 'ruby/tags/mapfile'
require_relative 'ruby/builders/latex/list'
require_relative 'ruby/builders/html/list'

puts "ReviewExtention loaded successfully" if ENV['DEBUG']