import os
import json
import matplotlib.pyplot as plt

script_dir = os.path.dirname(os.path.abspath(__file__))
json_path = os.path.join(script_dir, 'plot.json')

with open(json_path, 'r') as f:
    data = json.load(f)

relaxation = [point["relaxation"] for point in data["plot"]]
iterations = [point["iterations"] for point in data["plot"]]

normal_relaxation = [relaxation[i] for i in range(len(iterations)) if iterations[i] != -1]
normal_iterations = [iterations[i] for i in range(len(iterations)) if iterations[i] != -1]

special_relaxation = [relaxation[i] for i in range(len(iterations)) if iterations[i] == -1]
special_iterations = [-1 for _ in special_relaxation]

min_y_index = normal_iterations.index(min(normal_iterations))
min_y_relaxation = normal_relaxation[min_y_index]
min_y_iteration = normal_iterations[min_y_index]

plt.plot(normal_relaxation, normal_iterations, marker='o', linestyle='-', color='b', label='Iterations')
plt.scatter(special_relaxation, special_iterations, color='r', marker='x', s=100, label='Special: No solution')

plt.scatter(min_y_relaxation, min_y_iteration, color='g', marker='D', s=150, label='Min iterations')

plt.title('Relaxation vs Iterations')
plt.xlabel('Relaxation Parameter')
plt.ylabel('Iterations')
plt.grid(True)
plt.legend()

plt.show()
