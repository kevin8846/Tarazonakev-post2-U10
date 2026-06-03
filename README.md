# PostContenido 2 - Unidad 10

## Arquitectura de Computadores – Memoria Virtual en Linux

### Estudiante

Kevin Alejandro Tarazona Martínez

### Objetivo

Analizar el funcionamiento de la memoria virtual en Linux mediante la inspección del espacio de direcciones de un proceso utilizando los archivos `/proc/PID/maps` y `/proc/PID/smaps`, así como observar el comportamiento de los page faults y la asignación diferida de memoria.

---

## Entorno de Trabajo

* Sistema Operativo: Linux
* Compilador: GCC
* Herramientas utilizadas:

  * /proc filesystem
  * pmap
  * vmstat
  * free
  * /usr/bin/time

---

## Archivos del Proyecto

### mymaps.c

Programa utilizado para explorar el mapa de memoria virtual de un proceso. Permite identificar regiones como:

* Segmento de texto (.text)
* Segmento de datos (.data)
* Heap
* Stack
* Bibliotecas compartidas (libc)
* vDSO

### pagefault_demo.c

Programa utilizado para analizar la asignación diferida de memoria y observar el crecimiento del RSS al acceder a páginas reservadas mediante `malloc()`.

---

## Compilación

### mymaps.c

```bash
gcc -o mymaps mymaps.c
```

### pagefault_demo.c

```bash
gcc -o pagefault_demo pagefault_demo.c
```

---

## Ejecución

### Mapa de memoria

```bash
./mymaps &
PID=$!
cat /proc/$PID/maps
```

### Información detallada de memoria

```bash
cat /proc/$$/smaps | head -60
pmap -x $$
```

### Estadísticas generales

```bash
vmstat -s | head -15
free -h
```

### Demostración de Page Faults

```bash
./pagefault_demo
/usr/bin/time -v ./pagefault_demo
```

---

## Checkpoint 1 – Mapa de Memoria Virtual

Se identificaron las siguientes regiones de memoria:

| Región | Descripción                                |
| ------ | ------------------------------------------ |
| .text  | Código ejecutable del programa             |
| .data  | Variables globales inicializadas           |
| heap   | Memoria dinámica reservada mediante malloc |
| stack  | Variables locales y llamadas a funciones   |
| libc   | Biblioteca estándar de C compartida        |
| vDSO   | Interfaz entre kernel y espacio de usuario |

---

## Checkpoint 2 – Análisis de smaps

El archivo `smaps` permitió observar estadísticas detalladas de cada región de memoria.

### Shared_Clean

Corresponde principalmente a bibliotecas compartidas como libc. Estas páginas pueden ser utilizadas simultáneamente por varios procesos sin duplicar memoria física.

### Private_Dirty

Corresponde a páginas modificadas por el proceso actual. No pueden compartirse con otros procesos porque contienen información específica de la ejecución.

---

## Checkpoint 3 – Page Faults

Antes de ejecutar `memset()`, la memoria reservada mediante `malloc()` no se encuentra completamente asignada en RAM debido al mecanismo de asignación diferida (demand paging).

Después de ejecutar `memset()`, cada página es accedida físicamente y el sistema operativo debe crear las entradas correspondientes en las tablas de páginas, generando miles de minor page faults.

El incremento observado en `VmRSS` confirma que las páginas reservadas pasaron a ocupar memoria física.

---

## Conclusiones

1. Linux implementa memoria virtual mediante paginación y tablas de páginas.
2. El espacio de direcciones de un proceso está dividido en múltiples regiones con funciones específicas.
3. Las bibliotecas compartidas permiten optimizar el uso de memoria física.
4. La asignación diferida evita cargar páginas en RAM hasta que son utilizadas.
5. Los page faults son fundamentales para el funcionamiento de la memoria virtual y permiten administrar eficientemente los recursos del sistema.

---

## Referencias

Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). Operating System Concepts (10th ed.). Wiley.

Love, R. (2010). Linux Kernel Development (3rd ed.). Addison-Wesley Professional.
