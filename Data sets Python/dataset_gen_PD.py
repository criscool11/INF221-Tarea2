import random
import string

def generar_cadena_aleatoria(longitud):
    return ''.join(random.choice(string.ascii_lowercase) for _ in range(longitud))

def caso1():
    # Generar cadenas vacías y aumentar la longitud en cada iteración
    casos = []
    for longitud in range(5, 101, 5):
        casos.append(("",""))
    return casos

def caso2():
    # Palabras con caracteres iguales pero distintas palabras, longitud incrementada
    casos = []
    for longitud in range(5, 101, 5):
        base = generar_cadena_aleatoria(longitud)
        palabra1 = ''.join(sorted(base))
        palabra2 = ''.join(sorted(base, reverse=True))
        casos.append((palabra1, palabra2))
    return casos

def caso3():
    # Cadenas donde son necesarias transposiciones entre caracteres adyacentes
    casos = []
    for longitud in range(5, 101, 5):
        palabra1 = generar_cadena_aleatoria(longitud)
        i = random.randint(0, len(palabra1) - 2)
        palabra2 = list(palabra1)
        palabra2[i], palabra2[i + 1] = palabra2[i + 1], palabra2[i]
        casos.append((palabra1, ''.join(palabra2)))
    return casos

def caso4():
    #Ir variando la longitud de las cadenas, empezando de longitudes 5 hasta 100(cadenas de igual tamaño)
    casos = []
    
    for longitud in range(5, 101, 5):  # Longitudes de 2 hasta 18
        palabra1 = generar_cadena_aleatoria(longitud)
        palabra2 = generar_cadena_aleatoria(longitud)
        casos.append((palabra1, palabra2))
    
    return casos

def caso5():
    # Cadenas con distintas longitudes incrementadas
    casos = []
    for longitud1 in range(5, 101, 5):
        longitud2 = longitud1
        while longitud2 == longitud1:
            longitud2 = random.randint(2, 100)
        casos.append((generar_cadena_aleatoria(longitud1), generar_cadena_aleatoria(longitud2)))
    return casos

def generar_dataset_por_caso(caso_funcion, nombre_archivo):
    with open(nombre_archivo, "w") as file:
        for palabra1, palabra2 in caso_funcion():
            file.write(f"{palabra1} {palabra2}\n")
        file.write("\n")
    
casos = {
    "caso1PD.txt": caso1,
    "caso2PD.txt": caso2,
    "caso3PD.txt": caso3,
    "caso4PD.txt": caso4,
    "caso5PD.txt": caso5,
} 
for nombre_archivo, caso_funcion in casos.items():
    generar_dataset_por_caso(caso_funcion, nombre_archivo)

