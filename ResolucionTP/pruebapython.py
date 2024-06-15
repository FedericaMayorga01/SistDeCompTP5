import RPi.GPIO as GPIO
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Configuración del pin GPIO
GPIO_PIN = 17  # Cambiar a 27 si deseas usar el otro pin

# Configuración de GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(GPIO_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

# Datos para la gráfica
x_data, y_data = [], []

# Función para actualizar la gráfica
def update(frame):
    signal = GPIO.input(GPIO_PIN)
    y_data.append(signal)
    x_data.append(frame)
    ax.clear()
    ax.plot(x_data, y_data, label='GPIO {}'.format(GPIO_PIN))
    plt.ylim(-0.1, 1.1)
    plt.legend(loc='upper right')

# Función para limpiar los recursos de GPIO
def clean_exit(event):
    print("Cerrando el programa y limpiando GPIO...")
    GPIO.cleanup()
    plt.close()

# Inicialización de la gráfica
fig, ax = plt.subplots()
fig.canvas.mpl_connect('close_event', clean_exit)
ani = FuncAnimation(fig, update, interval=1000)

plt.title('Lectura del pin GPIO {}'.format(GPIO_PIN))
plt.xlabel('Tiempo')
plt.ylabel('Estado')
plt.show()
