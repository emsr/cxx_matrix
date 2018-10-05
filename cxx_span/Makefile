
all: test_span

# test_array_ref

test_span: span test_span.cpp
	$(HOME)/bin/bin/g++ -std=gnu++2a -g -Wall -Wextra -Wno-psabi -I. -o test_span test_span.cpp

test_array_ref: array_ref test_array_ref.cpp
	$(HOME)/bin/bin/g++ -std=gnu++17 -g -Wall -Wextra -Wno-psabi -I. -o array_ref test_array_ref.cpp
