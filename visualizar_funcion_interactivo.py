import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.widgets import Slider
from matplotlib import cm

fig = plt.figure(figsize=(16, 7))

# Crear subgráficos
ax_lineal = fig.add_subplot(121, projection='3d')
ax_trig = fig.add_subplot(122, projection='3d')

# Ajustar espacio para los sliders
plt.subplots_adjust(bottom=0.35)

# Crear ejes para los sliders
ax_x_min = plt.axes([0.2, 0.25, 0.6, 0.03])
ax_x_max = plt.axes([0.2, 0.20, 0.6, 0.03])
ax_y_min = plt.axes([0.2, 0.15, 0.6, 0.03])
ax_y_max = plt.axes([0.2, 0.10, 0.6, 0.03])

# Crear sliders
slider_x_min = Slider(ax_x_min, 'X mín', 0, 1, valinit=0, valstep=0.01)
slider_x_max = Slider(ax_x_max, 'X máx', 0, 1, valinit=1, valstep=0.01)
slider_y_min = Slider(ax_y_min, 'Y mín', 0, 1, valinit=0, valstep=0.01)
slider_y_max = Slider(ax_y_max, 'Y máx', 0, 1, valinit=1, valstep=0.01)

def update(val):
    # Obtener valores de los sliders
    x_min = slider_x_min.val
    x_max = slider_x_max.val
    y_min = slider_y_min.val
    y_max = slider_y_max.val
    
    # Validar que mín < máx
    if x_min >= x_max:
        x_max = x_min + 0.01
        slider_x_max.set_val(x_max)
    if y_min >= y_max:
        y_max = y_min + 0.01
        slider_y_max.set_val(y_max)
    
    # Crear malla de puntos
    x = np.linspace(x_min, x_max, 100)
    y = np.linspace(y_min, y_max, 100)
    X, Y = np.meshgrid(x, y)
    
    # Calcular funciones
    Z_lineal = X + Y - 1
    Z_trig = -np.cos(np.pi * (X + Y) / 2)
    
    # Limpiar gráficos
    ax_lineal.clear()
    ax_trig.clear()
    
    # Graficar función lineal
    ax_lineal.plot_surface(X, Y, Z_lineal, cmap='viridis', alpha=0.8)
    ax_lineal.scatter([0, 0, 1, 1], [0, 1, 0, 1], [-1, 0, 0, 1], 
                      color='red', s=100, label='Puntos clave', zorder=5)
    ax_lineal.set_xlabel('x')
    ax_lineal.set_ylabel('y')
    ax_lineal.set_zlabel('f(x,y)')
    ax_lineal.set_title('f(x,y) = x + y - 1')
    ax_lineal.legend()
    
    # Graficar función trigonométrica
    ax_trig.plot_surface(X, Y, Z_trig, cmap='plasma', alpha=0.8)
    ax_trig.scatter([0, 0, 1, 1], [0, 1, 0, 1], [-1, 0, 0, 1], 
                    color='red', s=100, label='Puntos clave', zorder=5)
    ax_trig.set_xlabel('x')
    ax_trig.set_ylabel('y')
    ax_trig.set_zlabel('f(x,y)')
    ax_trig.set_title('f(x,y) = -cos(π(x+y)/2)')
    ax_trig.legend()
    
    fig.canvas.draw_idle()

# Conectar sliders a la función de actualización
slider_x_min.on_changed(update)
slider_x_max.on_changed(update)
slider_y_min.on_changed(update)
slider_y_max.on_changed(update)

# Gráfico inicial
update(None)

plt.show()
