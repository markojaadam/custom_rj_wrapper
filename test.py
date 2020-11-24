from bench.bench import (
    test_parse_pythread,
    test_parse_omp,
    test_parse_tpe,
    test_construct_pythread,
    test_construct_tpe,
    test_construct_omp
)

n_cycles = 1000000

def bench_print_result(elapsed, n_workers, n_cycles):
    print("Number of workers: %s\t Elapsed: %.2f sec\t%.0d ns/op" % (n_workers, elapsed, (elapsed / n_cycles * 10 ** 9)))

# print("\n################ Response test ################")
# response_test()

print("\n################ Python thread test ################")
print("################ Parsing ################")
for n_workers in range(1, 11):
    elapsed = test_parse_pythread(n_cycles, n_workers)
    bench_print_result(elapsed, n_workers, n_cycles)
print("################ Construction ################")
for n_workers in range(1, 11):
    elapsed = test_construct_pythread(n_cycles, n_workers)
    bench_print_result(elapsed, n_workers, n_cycles)
print("\n################ ThreadPoolExecutor test ################")
print("################ Parsing ################")
for n_workers in range(1, 11):
    elapsed = test_parse_tpe(n_cycles, n_workers)
    bench_print_result(elapsed, n_workers, n_cycles)
print("################ Construction ################")
for n_workers in range(1, 11):
    elapsed = test_construct_tpe(n_cycles, n_workers)
    bench_print_result(elapsed, n_workers, n_cycles)
print("\n################ OMP thread test ################")
print("################ Parsing ################")
for n_workers in range(1, 11):
    elapsed = test_parse_omp(n_cycles, n_workers)
    bench_print_result(elapsed, n_workers, n_cycles)
print("################ Construction ################")
for n_workers in range(1, 11):
    elapsed = test_construct_omp(n_cycles, n_workers)
    bench_print_result(elapsed, n_workers, n_cycles)
