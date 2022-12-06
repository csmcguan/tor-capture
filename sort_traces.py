import os
import multiprocessing as mp
import numpy as np 

def load_trace(f):
    trace = np.loadtxt(f, delimiter='\t')
    trace = trace[trace[:,0].argsort(kind="mergesort")]
    trace[:,0] -= trace[0][0]
    return trace

def dump_trace(trace, fname):
    global dst
    with open(os.path.join(dst, fname), "w") as fp:
        for pkt in trace:
            fp.write("{:.4f}\t{}\n".format(pkt[0], pkt[1]))

def sort_trace(p):
    trace = load_trace(p)
    dump_trace(trace, p.split('/')[-1])

def parallel(flist, n_jobs=25):
    pool = mp.Pool(n_jobs)
    pool.map(sort_trace, flist)

if __name__ == '__main__':
    global src
    global dst

    for config in os.listdir("log"):
        src = dst = os.path.join("log", config)
        flist  = []
        for f in os.listdir(src):
            flist.append(os.path.join(src, f))

        print("sorting traces in {}".format(src))
        parallel(flist)
        print("done")
