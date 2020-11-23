from messaging.messaging import test

n_cycles = 1000000

for n_workers in range(1, 11):
    elapsed = test(n_cycles, n_workers)
    print(
        "Number of workers: %s\t Elapsed: %.2f sec\t%.0d ns/op" % (n_workers, elapsed, (elapsed / n_cycles * 10 ** 9)))
