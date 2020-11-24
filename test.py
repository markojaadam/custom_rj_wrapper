from messaging.messaging import (
    test_parse_pythread,
    test_parse_omp,
    test_parse_tpe,
    response_test
)

n_cycles = 1000000

def bench_print_result(elapsed, n_workers, n_cycles):
    print("Number of workers: %s\t Elapsed: %.2f sec\t%.0d ns/op" % (n_workers, elapsed, (elapsed / n_cycles * 10 ** 9)))

print("\n################ Response test ################")
response_test()

print("\n################ Python thread test ################")
for n_workers in range(1, 11):
    elapsed = test_parse_pythread(n_cycles, n_workers)
    bench_print_result(elapsed, n_workers, n_cycles)

print("\n################ ThreadPoolExecutor test ################")
for n_workers in range(1, 11):
    elapsed = test_parse_tpe(n_cycles, n_workers)
    bench_print_result(elapsed, n_workers, n_cycles)

print("\n################ OMP thread test ################")
for n_workers in range(1, 11):
    elapsed = test_parse_omp(n_cycles, n_workers)
    bench_print_result(elapsed, n_workers, n_cycles)