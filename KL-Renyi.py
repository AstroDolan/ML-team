import pandas as pd
import numpy as np

#Cargamos los datos en un dataframe
df = pd.read_csv('Egresos_Hospitalarios_2019.csv', encoding='latin-1', on_bad_lines='skip', sep=';')
counts_2019 = df['GLOSA_REGION_RESIDENCIA'].value_counts().sort_index()

## Aqui existen las 16 regiones, ademas de Extranjero e Ignorada
print(f'\n\nLa cantidad de regiones es: {len(counts_2019)}. Ademas la cantidad de registros es: {counts_2019.sum()}')

#Obtenemos la frecuencia absoluta
X = counts_2019.values / counts_2019.sum()

# Usamos la distribución uniforme para la parte b de KL
num_regiones = len(X)
Y = np.full(num_regiones, 1/num_regiones)

def kl_divergence(p, q):
    #Añadimos un valor epsilon infinitesimal para evitar log(0) o divisiones por 0
    epsilon = 1e-10
    p = p + epsilon
    q = q + epsilon
    return np.sum(p * np.log(p / q))

resultado_kl = kl_divergence(X, Y)
print(f"\nDivergencia KL entre 2019 y Uniforme: {resultado_kl}")

##Divergencia de Renyi
def renyi_divergence(p, q, alpha):
    #Pasamos las distribuciones como arrays de numpy
    p = np.asarray(p, dtype=float)
    q = np.asarray(q, dtype=float)
    
    #Epsilon para evitar ceros
    epsilon = 1e-10
    p = np.clip(p, epsilon, 1) #p array, epsilon minimo, 1 maximo. Los valores fuera del intervalo se convierten en las cotas
    q = np.clip(q, epsilon, 1)
    
    if alpha == 1:
        # alpha = 1 equivale a KL
        return np.sum(p * np.log(p / q))
    else:
        sum_term = np.sum((p**alpha) * (q**(1-alpha)))
        return (1.0 / (alpha - 1.0)) * np.log(sum_term)
        
alphas = [0.1, 0.5, 2, 5]
resultados = []

for a in alphas:
    d_renyi = renyi_divergence(X, Y, a)
    resultados.append({'Alpha': a, 'Renyi': d_renyi})

df_renyi = pd.DataFrame(resultados)
print(f"\nDivergencia de Rényi para X vs Uniforme (Y):")
print(df_renyi)