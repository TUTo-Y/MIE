import matplotlib.pyplot as plt
import numpy as np

# 定义时间范围
time = np.linspace(0, 0.4, 100)

# 计算S的值
S = 350 - 0.5 * 875 * time * time

# 绘制图像
plt.plot(time, S, label='S = 350 - 0.5 * 875 * time^2')
plt.xlabel('Time')
plt.ylabel('S')
plt.title('Graph of S = 350 - 0.5 * 875 * time^2')
plt.legend()
plt.grid(True)
plt.show()