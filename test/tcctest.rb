a = Tcc.new
a.compile_string('main(){ printf("Hello\n");}')
a.relocate
a.call('main')
p a
