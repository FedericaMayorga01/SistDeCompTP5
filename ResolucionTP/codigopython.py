import os
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

DEVICE_PATH = "/dev/SdeC_drvTTT"

def read_gpio():
    with open(DEVICE_PATH, 'r') as f:
        return f.read().strip()

def write_gpio(selection):
    with open(DEVICE_PATH, 'w') as f:
        f.write(selection)

def update(frame):
    signal = read_gpio()
    y_data.append(int(signal))
    x_data.append(frame)
    ax.clear()
    ax.plot(x_data, y_data)
    plt.ylim(0, 1)

# Inicialización de la gráfica
fig, ax = plt.subplots()
x_data, y_data = [], []

# Inicialización del dispositivo
if not os.path.exists(DEVICE_PATH):
    print("El dispositivo no existe")
    exit(1)

# Interfaz de usuario para cambiar el pin GPIO
print("Seleccione el pin GPIO a leer:")
print("1: GPIO_PIN_1")
print("2: GPIO_PIN_2")
selection = input("Ingrese su selección (1 o 2): ")

if selection not in ['1', '2']:
    print("Selección no válida")
    exit(1)

write_gpio(selection)

# Inicialización de la animación
ani = FuncAnimation(fig, update, interval=1000)

plt.show()
