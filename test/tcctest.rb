a = Tcc.new
a.compile_string('main(){ printf("Hello\n");}')
a.relocate
a.call('main')

a = Tcc.new
a.compile_string('foo(int a){ printf("Hello %d\n", a);}')
a.relocate
a.call('foo', 3)

a = Tcc.new
a.compile_string('bar(int a, int b){ printf("Hello %d %d\n", b, a);}')
a.relocate
a.call('bar', 31, 24)
