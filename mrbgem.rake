MRuby::Gem::Specification.new('mrtcc') do |spec|
  spec.license = 'mruby'
  spec.authors = 'miura1729'

  spec.linker.libraries << ['tcc', 'dl']
end
