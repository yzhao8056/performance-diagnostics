import csv
import matplotlib.pyplot as plt

sizes_kib = []
latencies_ns = []

with open("latency_results.csv") as f:
    reader = csv.DictReader(f)

    for row in reader:
        sizes_kib.append(int(row["size_kib"]))
        latencies_ns.append(float(row["latency_ns"]))
        
plt.plot(sizes_kib, latencies_ns, marker="o")

plt.xscale("log", base=2)

plt.xlabel("Working-set size (KiB)")
plt.ylabel("Average latency (ns)")
plt.title("Random pointer-chasing memory latency")
plt.grid(True)

plt.tight_layout()
plt.show()
        