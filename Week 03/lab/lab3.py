import subprocess
from time import time

def time_algorithms():
    files = ["random5000.txt", "random10000.txt", "random50000.txt", 
             "reversed5000.txt", "reversed10000.txt", "reversed50000.txt",
             "sorted5000.txt", "sorted10000.txt", "sorted50000.txt"]

    sorting_alg = ["sort1", "sort2", "sort3"]

    sorting_durations = {}
    for alg in sorting_alg: sorting_durations[alg] = []

    for sort in sorting_alg:
        print(f'{sort}')        
        for f in files:
            t0 = time()
            subprocess.run([f"./{sort}", f], stdout=subprocess.DEVNULL)
            t1 = time()

            dt = t1 - t0
            sorting_durations[sort].append(dt)
            print(f'File {f}: {dt} s')
            
        print()

    print(f'Results:')
    print(f'{sorting_durations}')  


if __name__ == '__main__':
    time_algorithms()