#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys
from typing import Tuple

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

file_type = "jpg"


def read_tsv_fast(path: str) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
    # Columns: n, is_prime, time_ns
    # Use numpy.loadtxt with converters to avoid Python-level loops
    try:
        data = np.loadtxt(path, delimiter="\t", skiprows=1, dtype=np.float64)
    except ValueError:
        # Fallback if single-column issues; try genfromtxt which is more tolerant
        data = np.genfromtxt(path, delimiter="\t", skip_header=1, dtype=np.float64)
    if data.ndim == 1:
        data = data.reshape(1, -1)
    n = data[:, 0]
    is_prime = data[:, 1]
    t = data[:, 2]
    return n, is_prime, t


def save_scatter_loglog(
    n: np.ndarray,
    p: np.ndarray,
    t: np.ndarray,
    out_path: str,
    title: str,
    xlabel: str = "log10(n)",
    ylabel: str = "log10(time [ns])",
) -> None:
    # Vectorized log10 with small epsilon to avoid log(0)
    x = np.log2(n)
    y = t

    fig = plt.figure(figsize=(12, 7), dpi=240)
    ax = fig.add_subplot(1, 1, 1)

    cmap = matplotlib.colors.ListedColormap(["#379777", "#F4CE14"])
    ax.scatter(x, y, s=4, c=p, cmap=cmap, vmin=0, vmax=1, alpha=0.6, linewidths=0)

    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)

    # Grid and tight layout for readability
    ax.grid(True, linestyle=":", linewidth=0.8, alpha=0.6)
    fig.tight_layout()

    fig.savefig(out_path + "." + file_type, format=file_type)
    plt.close(fig)


def main() -> int:
    plt.style.use("dark_background")
    cwd = os.getcwd()

    p_prime = os.path.join(cwd, "benchmarks/bench_IsPrime.tsv")
    p_notable = os.path.join(cwd, "benchmarks/bench_IsPrimeNoTable.tsv")

    if not os.path.exists(p_prime):
        print(f"Missing file: {p_prime}", file=sys.stderr)
        return 1
    if not os.path.exists(p_notable):
        print(f"Missing file: {p_notable}", file=sys.stderr)
        return 1

    n1, p1, t1 = read_tsv_fast(p_prime)
    n2, p2, t2 = read_tsv_fast(p_notable)

    save_scatter_loglog(
        n1,
        p1,
        t1,
        os.path.join(cwd, "benchmarks/bench_IsPrime"),
        title="cppr::IsPrime",
    )
    save_scatter_loglog(
        n2,
        p2,
        t2,
        os.path.join(cwd, "benchmarks/bench_IsPrimeNoTable"),
        title="cppr::IsPrimeNoTable",
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
