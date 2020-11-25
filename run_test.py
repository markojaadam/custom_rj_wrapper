from bench.bench import (
    conversion_test,
    parse_test,
    construct_test
)

conversion_test()

print("\n################ Basic test ################")
print("################ Parsing ################")
parse_test()
print("################ Construction ################")
construct_test()