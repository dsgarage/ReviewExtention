# frozen_string_literal: true

# Force load Ruby extensions for LaTeX builder
require_relative '../ruby/tags/list'
require_relative '../ruby/tags/mapfile'
require_relative '../ruby/builders/latex/list'
require_relative '../ruby/builders/html/list'

puts "LaTeX extensions loaded" if ENV['DEBUG']