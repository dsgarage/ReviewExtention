# frozen_string_literal: true

# Re:VIEW Extension Initializer
# This file is automatically loaded by Re:VIEW when placed in lib/review/

# Load custom extensions
require_relative 'tags/list'
require_relative 'tags/mapfile'
require_relative 'builders/latex/list'
require_relative 'builders/html/list'

puts "ReviewExtention initialized" if ENV['DEBUG']