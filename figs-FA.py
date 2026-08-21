import numpy as np
import matplotlib.pyplot as plt

# ============================================================
# 1. Función ReLU
# f(x) = max(0, x)
# ============================================================

x = np.linspace(-5, 5, 1000)
relu = np.maximum(0, x)

fig, ax = plt.subplots(figsize=(7, 5))

ax.plot(x, relu, linewidth=2)

ax.axhline(0, color='black', linewidth=0.8)
ax.axvline(0, color='black', linewidth=0.8)

ax.set_xlabel(r"$x$", fontsize=13)
ax.set_ylabel(r"$f(x)$", fontsize=13)

ax.set_title(r"Función de activación ReLU: $f(x)=\max(0,x)$",
             fontsize=14)

ax.grid(True, alpha=0.25)

plt.tight_layout()

# Exportar
plt.savefig("funcion_relu.png", dpi=600, bbox_inches="tight")
plt.savefig("funcion_relu.pdf", bbox_inches="tight")

plt.show()


# ============================================================
# 2. Función Sigmoide
# f(x) = 1 / (1 + e^(-x))
# ============================================================

sigmoid = 1 / (1 + np.exp(-x))

fig, ax = plt.subplots(figsize=(7, 5))

ax.plot(x, sigmoid, linewidth=2)

ax.axhline(0, color='black', linewidth=0.8)
ax.axvline(0, color='black', linewidth=0.8)

ax.set_xlabel(r"$x$", fontsize=13)
ax.set_ylabel(r"$f(x)$", fontsize=13)

ax.set_title(
    r"Función de activación sigmoide: "
    r"$f(x)=\frac{1}{1+e^{-x}}$",
    fontsize=14
)

ax.grid(True, alpha=0.25)

plt.tight_layout()

# Exportar
plt.savefig("funcion_sigmoide.png", dpi=600, bbox_inches="tight")
plt.savefig("funcion_sigmoide.pdf", bbox_inches="tight")

plt.show()