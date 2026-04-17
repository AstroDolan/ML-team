# Plantilla LaTeX para tesis Usach
Versión 2.0 2025-08-21

Esta plantilla en LaTeX:

- favorece la adecuación de su tesis al formato de la Universidad,

- reduce el tiempo dedicado al aspecto formal, permitiendo enfocarse en la investigación,

- agiliza los procesos de revisión y corrección.

Para un uso adecuado, siga las indicaciones de la [Guía para el formato de trabajos de titulación y/o graduación](https://biblioteca.usach.cl/sites/default/files/Gu%C3%ADa%20para%20el%20formato%20de%20trabajo%20de%20titulaci%C3%B3n.pdf), así como las orientaciones de su profesor guía y bibliotecólogo.

## Tabla de contenido
- [Configuración](#configuración)
- [Metadatos](#metadatos)
- [Compilador](#compilador)
- [Estándares](#estándares)
- [Tipografías](#tipografías)
- [Licencia](#licencia)
- [Contacto](#contacto)

### Configuración
Agregue su carrera, género y licencia de uso en el comando `\documentclass`, dentro del archivo `main.tex`.

**Ejemplo:** tesis del LCC, autora, bajo licencia CC BY 4.0.

```latex
\documentclass[
  16047,
  femenino,
  ccby,
]{tesisUsach}
```

## Opciones (obligatorias)
| Género    |           |
|-----------|-----------|
| femenino  | Femenino  |
| masculino | Masculino |

> Para varios autores de distintos géneros, utilice masculino.

| Código  | Departamento de Matemática y Ciencia de la Computación | 
|---------|--------------------------------------------------------|
| 16045   | Pedagogía en Matemática y Computación                  |
| 16046   | Licenciatura en Ciencia de la Computación              |
| 16047   | Ingeniería Matemática                                  |
| 16048   | Ingeniería Estadística                                 |
| 9204    | Magister en Educación Matemática                       |
| 9224    | Magister en Ciencia en la Especialidad de Matemática   |
| 9214    | Doctorado en Ciencia con Mención en Matemática         |

| Código  | Departamento de Física                                 |
|---------|--------------------------------------------------------|
| 16042   | Ingeniería Física                                      |
| 16049   | Pedagogía en Física y Matemática                       |
| 16108   | Astrofísica con Mención en Ciencia de Datos            |
| 9225    | Magíster en Ciencia con Mención en Física              |
| 9223    | Doctorado en Ciencia con Mención en Física             |

| Licencia | Creative Commons                                        |
|----------|---------------------------------------------------------|
| ccby     | Atribución 4.0 Internacional                            |
| ccbysa   | Atribución-CompartirIgual 4.0 Internacional             |
| ccbynd   | Atribución-SinDerivadas 4.0 Internacional               |
| ccbync   | Atribución-NoComercial 4.0 Internacional                |
| ccbyncsa | Atribución-NoComercial-CompartirIgual 4.0 Internacional |
| ccbyncnd | Atribución-NoComercial-SinDerivadas 4.0 Internacional   |

> La licencia por defecto es CC BY 4.0.

## Opciones (alternativas)
| Secciones       |                       |
|-----------------|-----------------------|
| proyecto        | Proyecto patrocinador |
| abstract        | Resumen en inglés     |
| dedicatoria     | Dedicatoria           |
| agradecimientos | Agradecimientos       |
| glosario        | Glosario              |
| anexos          | Anexos                |
| apendices       | Apéndices             |

> Estas secciones son optativas, agréguelas solo en caso de ser necesarias.

| Estilo  |      |
|---------|------|
| ams     | AMS  |
| ieee    | IEEE |
| apa     | APA  |
| phys    | IAP  |

> El estilo por defecto es APA.

| Idioma  |         |
|---------|---------|
| english | Inglés  |
| espanol | Español |

> El idioma por defecto es español.

**Ejemplo:** tesis en inglés, utiliza IEEE e incluye dedicatoria y glosario.
```latex
\documentclass[
  16046,
  masculino,
  ccbynd,
  english,
  ieee,
  dedicatoria,
  glosario,
]{tesisUsach}
```
## Metadatos
Agregue los argumentos correspondientes al título, subtítulo, autores, profesor guía, palabras clave y año de su última matrícula en el comando `\metadatos`, dentro del archivo `Recursos/metadatos.tex`.

> Al compilar, la portada se actualizará según sus opciones y metadatos.

## Compilador
Esta plantilla es compatible con pdfLaTeX y XeLaTeX, pero le sugerimos compilar con **LuaLaTeX**, dado que ofrece una experiencia optimizada para textos multilenguaje, automatización y catalogación.

# Bibliografía
La bibliografía es gestionada con **BibLaTeX** (Biber), con mayor control y compatibilidad con estándares modernos en comparación con el sistema clásico de BibTeX. Las referencias se almacenan en un archivo BIB.

> Ajuste su editor de LaTeX de acuerdo a lo anterior. Por ejemplo, en TeXstudio: Opciones > Configurar TeXstudio > Compilar > Herramienta bibliográfica por defecto: Biber.

# Glosarios
Si añade un glosario a su tesis, es posible que necesite ejecutar makeglossaries manualmente después de compilar:
```sh
$ lualatex main
$ makeglossaries main
$ lualatex main
```

## Estándares
Al compilar, se producirá un archivo PDF 1.4 o 1.7 en conformidad con el estandar A-3b respecto a accesibilidad, preservación digital, íntegridad, interoperabilidad y extensibilidad.

Más información en [PDF Association - ISO 19005](https://pdfa.org/resource/iso-19005-pdfa/).

El PDF incluye metadatos embebidos en formato XMP, que incorporan los siguientes campos del esquema Dublin Core:

| Campo               | DC           |
|---------------------|--------------|
| Título              | dc:title     |
| Autor               | dc:creator   |
| Idioma              | dc:language  |
| Editorial           | dc:publisher |
| Palabras clave      | dc:subject   |
| Tipo de publicación | dc:type      |
| Derechos de autor   | dc:rights    |
| Licencia de uso     | dc:license   |

> Al compilar, los metadatos se actualizarán con los valores que haya proporcionado.

## Tipografías
Hemos utilizado TeX Gyre Heros como fuente principal, Termes para matemática y Cursor para código. La familiar TeX Gyre ofrece versiones modernas y mejoradas de las clásicas Times New Roman, Helvetica y Courier New.

| Categoría  | Windows           | PostScript    | LaTeX           |
|------------|-------------------|---------------|-----------------|
| Serif      | Times New Roman   | Nimbus Roman  | TeX Gyre Termes |
| Sans serif | Arial (Helvetica) | Nimbus Sans   | TeX Gyre Heros  |
| Monospace  | Courier New       | Nimbus Mono L | TeX Gyre Cursor |

Además, presenta ventajas respecto a sus símiles Nimbus, como mejor legibilidad en pantalla, más glifos y soporte OpenType, métricas refinadas, licencia libre (GUST), soporte para símbolos técnicos y optimización para LaTeX.

## Licencia
Plantilla LaTeX tesis Usach puede ser distribuida y/o modificada bajo los términos de la LaTeX Project Public License, versión 1.3c.  
La versión más reciente de esta licencia está disponible en [The LaTeX Project](https://www.latex-project.org/lppl/lppl-1-3c).

## Contacto
¿Tiene alguna sugerencia para mejorar la plantilla o esta ayuda?  
Escríbanos a [Biblioteca DMCC](mailto:biblioteca.dmcc@usach.cl).