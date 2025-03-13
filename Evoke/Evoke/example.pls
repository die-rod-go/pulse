env Point {
	var x : init;
	var y : init;
	emit init;
} : define_point;
emit define_point;

Point test	: init_test;
test.x = 10 : init_test;
test.y = 10	: init_test;
emit init_test;

print test.x + test.y : print_point;
emit print_point;
