% ----------------------------------------------------------------------------------------------------------
% Tíulo: Plantilla LaTeX para tesis Usach
% Asunto: Incorpora la Guía para trabajos de titulación y/o graduación
% Autor: Francisco Bustos Naulin
% Fecha: 2025-08-21 (AAAA-MM-DD)
% Versión: 2.0
% ----------------------------------------------------------------------------------------------------------
% Sitio web: http://dmcc.usach.cl/plantillas
% Contacto: biblioteca.dmcc@usach.cl
% ----------------------------------------------------------------------------------------------------------
% Este plantilla puede ser distribuida y/o modificada bajo los términos de la
% LaTeX Project Public License, versión 1.3c.
% La versión más reciente de esta licencia está disponible en:
% https://www.latex-project.org/lppl/lppl-1-3c
% ----------------------------------------------------------------------------------------------------------

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{tesisUsach}[2025-08-21 v2.0]

\RequirePackage{etoolbox,xkeyval,iftex,textcomp,xcolor}

% -----------------
% OPCIONES DE CLASE
% -----------------

% Variables internas
\newcommand{\tesisusach@codigo}{00000}
\newcommand{\tesisusach@genero}{neutro}
\newcommand{\tesisusach@facultad}{Facultad}
\newcommand{\tesisusach@departamento}{Departamento}
\newcommand{\tesisusach@programa}{Programa}
\newcommand{\tesisusach@titulacion}{Titulación}

% Comandos públicos
\newcommand{\codigo}{\tesisusach@codigo}
\newcommand{\genero}{\tesisusach@genero}
\newcommand{\facultad}{\tesisusach@facultad}
\newcommand{\departamento}{\tesisusach@departamento}
\newcommand{\programa}{\tesisusach@programa}
\newcommand{\titulacion}{\tesisusach@titulacion}

% Códigos de programas
\newcommand{\tesisusach@agregacodigo}[1]{%
	\DeclareOption{#1}{\renewcommand{\tesisusach@codigo}{#1}}%
}

\forcsvlist{\tesisusach@agregacodigo}{%
	16045,16046,16047,16048,%
	9204,9224,9214,16108,%
	16042,16049,9225,9223%
}

% Géneros
\DeclareOption{femenino}{\renewcommand{\tesisusach@genero}{femenino}}%
\DeclareOption{masculino}{\renewcommand{\tesisusach@genero}{masculino}}%

% Licencia
\DeclareOption{ccby}{%
	\def\licenciaTipo{Esta tesis está bajo una Licencia Creative Commons Atribución 4.0 Internacional}%
	\def\licenciaURL{https://creativecommons.org/licenses/by/4.0/}%
}
\DeclareOption{ccbysa}{%
	\def\licenciaTipo{Esta tesis está bajo una Licencia Creative Commons Atribución-CompartirIgual 4.0 Internacional}%
	\def\licenciaURL{https://creativecommons.org/licenses/by-sa/4.0/}%
}
\DeclareOption{ccbynd}{%
	\def\licenciaTipo{Esta tesis está bajo una Licencia Creative Commons Atribución-SinDerivadas 4.0 Internacional}%
	\def\licenciaURL{https://creativecommons.org/licenses/by-nd/4.0/}%
}
\DeclareOption{ccbync}{%
	\def\licenciaTipo{Esta tesis está bajo una Licencia Creative Commons Atribución-NoComercial 4.0 Internacional}%
	\def\licenciaURL{https://creativecommons.org/licenses/by-nc/4.0/}%
}
\DeclareOption{ccbyncsa}{%
	\def\licenciaTipo{Esta tesis está bajo una Licencia Creative Commons Atribución-NoComercial-CompartirIgual 4.0 Internacional}%
	\def\licenciaURL{https://creativecommons.org/licenses/by-nc-sa/4.0/}%
}
\DeclareOption{ccbyncnd}{%
	\def\licenciaTipo{Esta tesis está bajo una Licencia Creative Commons Atribución-NoComercial-SinDerivadas 4.0 Internacional}%
	\def\licenciaURL{https://creativecommons.org/licenses/by-nc-nd/4.0/}%
}

\providecommand{\licenciaTipo}{Esta tesis está bajo una Licencia Creative Commons Atribución 4.0 Internacional}%
\providecommand{\licenciaURL}{https://creativecommons.org/licenses/by/4.0/}%

% Proyecto
\newif\iftesisusach@proyecto
\tesisusach@proyectofalse
\DeclareOption{proyecto}{\tesisusach@proyectotrue}%

\newcommand{\proyecto}[1]{%
	\iftesisusach@proyecto
		\clearpage
		\thispagestyle{empty}%
		\vspace*{\fill}%
		\noindent\hspace*{0.1\textwidth}%
		\parbox{0.8\textwidth}{\centering\ttfamily #1}%
		\vspace*{\fill}%
		\clearpage
	\fi
}

% Abstract
\newif\iftesisusach@abstract
\tesisusach@abstractfalse
\DeclareOption{abstract}{\tesisusach@abstracttrue}%

\newcommand{\incAbstract}{%
	\iftesisusach@abstract
	\ifdefstring{\DocLang}{english}{%
		\renewcommand{\abstractname}{Resumen}%
	}{%
		\renewcommand{\abstractname}{Abstract}%
	}%
	\include{Secciones/a2_Abstract}%
	\fi
}

% Dedicatoria
\newif\iftesisusach@dedicatoria
\tesisusach@dedicatoriafalse
\DeclareOption{dedicatoria}{\tesisusach@dedicatoriatrue}%

\newcommand{\dedicatoria}[1]{%
    \iftesisusach@dedicatoria
        \clearpage
        \vspace*{\stretch{1}}%
        \begin{FlushRight}%
            \itshape #1%
        \end{FlushRight}%
        \vspace*{\stretch{3}}%
        \clearpage
    \fi
}

% Agradecimientos
\newif\iftesisusach@agradecimientos
\tesisusach@agradecimientosfalse
\DeclareOption{agradecimientos}{\tesisusach@agradecimientostrue}%

\newcommand{\agradecimientos}[1]{%
    \iftesisusach@agradecimientos
        \clearpage
        \chapter*{\acknowledgmentsname}%
        #1%
        \clearpage
    \fi
}

% Glosario
\newif\iftesisusach@glosario
\tesisusach@glosariofalse
\DeclareOption{glosario}{\tesisusach@glosariotrue}%

% Apartados
\newif\iftesisusach@anexos
\tesisusach@anexosfalse
\DeclareOption{anexos}{\tesisusach@anexostrue}%

\newif\iftesisusach@apendices
\tesisusach@apendicesfalse
\DeclareOption{apendices}{\tesisusach@apendicestrue}%

% Citas
\newcommand{\@norma}{apa}
\DeclareOption{ams}{\def\@norma{ams}}%
\DeclareOption{ieee}{\def\@norma{ieee}}%
\DeclareOption{apa}{\def\@norma{apa}}%
\DeclareOption{phys}{\def\@norma{phys}}%

% Implementa opciones
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}%

\DeclareOptionX{english}{\def\DocLang{english}}%
\DeclareOptionX{espanol}{\def\DocLang{spanish}}%
\ExecuteOptionsX{espanol}%

\ProcessOptions\relax
\ProcessOptionsX\relax

\LoadClass[12pt,oneside,openany]{report}%

% Idiomas
\newcommand{\acknowledgmentsname}{}%
\newcommand{\introductionname}{}%
\newcommand{\conclusionsname}{}%
\newcommand{\annexname}{}%
\newcommand{\annextocname}{}%
\newcommand{\annexsecname}{}%
\newcommand{\appendixsecname}{}%

\RequirePackage[title,toc]{appendix}%

\ifluatex
	\RequirePackage{polyglossia}%
	\ifdefstring{\DocLang}{english}{%
		\setmainlanguage{english}%
		\setotherlanguage{spanish}%
		\newcommand{\XMPlang}{en-US}%
		\appto\captionsenglish{%
			\renewcommand{\contentsname}{Table of Contents}%
			\renewcommand{\acknowledgmentsname}{Acknowledgments}%
			\renewcommand{\introductionname}{Introduction}%
			\renewcommand{\conclusionsname}{Conclusions}%
			\renewcommand{\annexname}{Annexes}%
			\renewcommand{\annextocname}{Annexes}%
			\renewcommand{\annexsecname}{Annex}%
			\renewcommand{\appendixname}{Appendices}%
			\renewcommand{\appendixtocname}{Appendices}%
			\renewcommand{\appendixsecname}{Appendix}%
		}%
		\captionsenglish%
		\renewcommand{\abstractname}{Abstract}%
	}{%
		\setmainlanguage{spanish}%
		\setotherlanguage{english}%
		\newcommand{\XMPlang}{es-CL}%
		\appto\captionsspanish{%
			\renewcommand{\contentsname}{Tabla de contenido}%
			\renewcommand{\listtablename}{Índice de tablas}%
			\renewcommand{\acknowledgmentsname}{Agradecimientos}%
			\renewcommand{\introductionname}{Introducción}%
			\renewcommand{\conclusionsname}{Conclusiones}%
			\renewcommand{\annexname}{Anexos}%
			\renewcommand{\annextocname}{Anexos}%
			\renewcommand{\annexsecname}{Anexo}%
			\renewcommand{\appendixname}{Apéndices}%
			\renewcommand{\appendixtocname}{Apéndices}%
			\renewcommand{\appendixsecname}{Apéndice}%
		}%
		\captionsspanish%
		\renewcommand{\abstractname}{Resumen}%
	}%
\else
	\RequirePackage[english,\DocLang]{babel}%
	\selectlanguage{\DocLang}%
	\ifdefstring{\DocLang}{english}{%
		\newcommand{\XMPlang}{en-US}%
		\addto\captionsenglish{%
			\renewcommand{\listtablename}{List of Tables}%
			\renewcommand{\listfigurename}{List of Figures}%
			\renewcommand{\contentsname}{Table of Contents}%
			\renewcommand{\appendixname}{Appendices}%
			\renewcommand{\acknowledgmentsname}{Acknowledgments}%
			\renewcommand{\introductionname}{Introduction}%
			\renewcommand{\conclusionsname}{Conclusions}%
			\renewcommand{\annexname}{Annexes}%
			\renewcommand{\annextocname}{Annexes}%
			\renewcommand{\annexsecname}{Annex}%
			\renewcommand{\appendixname}{Appendices}%
			\renewcommand{\appendixtocname}{Appendices}%
			\renewcommand{\appendixsecname}{Appendix}%
		}%
		\captionsenglish%
		\renewcommand{\abstractname}{Resumen}%
	}{%
		\newcommand{\XMPlang}{es-CL}%
		\addto\captionsspanish{%
			\renewcommand{\listtablename}{Índice de tablas}%
			\renewcommand{\listfigurename}{Índice de figuras}%
			\renewcommand{\contentsname}{Tabla de contenido}%
			\renewcommand{\appendixname}{Apéndices}%
			\renewcommand{\acknowledgmentsname}{Agradecimientos}%
			\renewcommand{\introductionname}{Introducción}%
			\renewcommand{\conclusionsname}{Conclusiones}%
			\renewcommand{\annexname}{Anexos}%
			\renewcommand{\annextocname}{Anexos}%
			\renewcommand{\annexsecname}{Anexo}%
			\renewcommand{\appendixname}{Apéndices}%
			\renewcommand{\appendixtocname}{Apéndices}%
			\renewcommand{\appendixsecname}{Apéndice}%
		}%
		\captionsspanish%
		\renewcommand{\abstractname}{Abstract}%
	}%
\fi

% -------------- 
% IMPLEMENTACIÓN
% --------------

% Glosario
\RequirePackage{glossaries}%
\makeglossaries%
\input{Recursos/glosario}%
\glsaddall%

% Páginas finales
\AtEndDocument{%
	\iftesisusach@glosario
		\begingroup%
			\let\newpage\relax%
			\par\clearpage%
			\printglossary%
		\endgroup%
		\addcontentsline{toc}{chapter}{\glossaryname}%%
	\fi
		
	\begingroup%
	\setlength{\bibitemsep}{1\baselineskip}%%
	\printbibliography[heading=bibintoc]%
	\endgroup%
			
	\iftesisusach@anexos
		\clearpage%
		\addtocontents{toc}{\protect\setcounter{tocdepth}{0}}%
		\let\oldappendixtocname\appendixtocname%
		\renewcommand{\appendixtocname}{\annextocname}
		\renewcommand{\thesection}{\annexsecname~\arabic{section}:}%
		\begin{appendices}%
			\input{Secciones/b1_Anexos}%
		\end{appendices}%
		\let\appendixtocname\oldappendixtocname%
		\addtocontents{toc}{\protect\setcounter{tocdepth}{3}}%
	\fi
	
	\iftesisusach@apendices
		\clearpage%
		\addtocontents{toc}{\protect\setcounter{tocdepth}{0}}%
		\renewcommand{\thesection}{\appendixsecname~\arabic{section}:}%
		\begin{appendices}%
			\input{Secciones/b2_Apendices}%
		\end{appendices}%
		\addtocontents{toc}{\protect\setcounter{tocdepth}{3}}%
	\fi
}

% -------
% ARCHIVO
% -------

% Metadatos
\newcommand{\metadatos}[7]{%
    \gdef\titulo{#1}%
    \gdef\subtitulo{#2}%
    \gdef\autor{#3}%
    \gdef\profGuia{#4}%
    \gdef\palabrasClave{#5}%
    \gdef\keywords{#6}%
    \gdef\matricula{#7}%
}

\input{Recursos/metadatos}%

% CSV
\ifluatex
\RequirePackage{luacode}
\begin{luacode}
	local tesis_data = {}
	
	function load_csv(filename)
	for line in io.lines(filename) do
	if line:find("^%d") then
	local cod, fac, dep, prog, fem, masc = line:match("([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+)")
	if cod then
	tesis_data[cod] = {
		facultad = fac,
		departamento = dep,
		programa = prog,
		femenino = fem,
		masculino = masc
	}
	end
	end
	end
	end
	
	function get_data(codigo, genero)
	local entry = tesis_data[codigo or ""]
	if entry then
	return entry.facultad, entry.departamento, entry.programa, entry[genero or "masculino"]
	end
	return "[Facultad]", "[Departamento]", "[Programa]", "[Titulación]"
	end
\end{luacode}

\newcommand{\tesisusach@cargardatos}{%
	\directlua{load_csv("programasUsach.csv")}%
	\directlua{
		local fac, dep, prog, tit = get_data("\tesisusach@codigo", "\tesisusach@genero")
		token.set_macro("tesisusach@facultad", fac, "global")
		token.set_macro("tesisusach@departamento", dep, "global")
		token.set_macro("tesisusach@programa", prog, "global")
		token.set_macro("tesisusach@titulacion", tit, "global")
	}%
	\protected@xdef\facultad{\unexpanded\expandafter{\tesisusach@facultad}}%
	\protected@xdef\departamento{\unexpanded\expandafter{\tesisusach@departamento}}%
	\protected@xdef\programa{\unexpanded\expandafter{\tesisusach@programa}}%
	\protected@xdef\titulacion{\unexpanded\expandafter{\tesisusach@titulacion}}%
}
\else
\RequirePackage{datatool}
\newcommand{\tesisusach@cargardatos}{%
	\DTLloaddb[keys={codigo,facultad,departamento,programa,femenino,masculino}]{tesisdata}{programasUsach.csv}%
	\DTLforeach*{tesisdata}{%
		\dbcodigo=codigo,%
		\dbfac=facultad,%
		\dbdep=departamento,%
		\dbprog=programa,%
		\dbtitfem=femenino,%
		\dbtitmasc=masculino%
	}{%
		\ifnum\pdfstrcmp{\dbcodigo}{\tesisusach@codigo}=0
		\protected@xdef\facultad{\unexpanded\expandafter{\dbfac}}%
		\protected@xdef\departamento{\unexpanded\expandafter{\dbdep}}%
		\protected@xdef\programa{\unexpanded\expandafter{\dbprog}}%
		\ifnum\pdfstrcmp{\tesisusach@genero}{femenino}=0
		\protected@xdef\titulacion{\unexpanded\expandafter{\dbtitfem}}%
		\else
		\protected@xdef\titulacion{\unexpanded\expandafter{\dbtitmasc}}%
		\fi
		\dtlbreak
		\fi
	}%
}
\fi

\tesisusach@cargardatos%

\def\tesisusach@facultad{\detokenize{\tesisusach@facultad}}%
\def\tesisusach@departamento{\detokenize{\tesisusach@departamento}}%
\def\tesisusach@programa{\detokenize{\tesisusach@programa}}%
\def\tesisusach@titulacion{\detokenize{\tesisusach@titulacion}}%

% XMP
\begin{filecontents*}[overwrite]{\jobname.xmpdata}
	\Title{\titulo\ \subtitulo}
	\Subject{\programa}
	\Author{\autor}
	\Language{\XMPlang}
	\Publisher{Universidad de Santiago de Chile}
	\Keywords{\palabrasClave}
	\PublicationType{Text}
	\CreatorTool{LaTeX}
	\Copyright{\licenciaTipo}
	\CopyrightURL{\licenciaURL}
	\Copyrighted{True}
\end{filecontents*}

% PDF
\PassOptionsToPackage{bookmarks,pdfpagelabels,hidelinks,spanish}{hyperref}%
\RequirePackage[a-3b]{pdfx}%
\RequirePackage{pdfpages}%

% ------
% ESTILO
% ------

% Diagramación
\RequirePackage[skip=2ex plus1ex, indent=0em]{parskip}
\RequirePackage{setspace,ragged2e,geometry}

\geometry{%
	total={166mm,229mm},%
	left=25mm,%
	top=25mm%
}

\AtBeginDocument{%
    \onehalfspacing%
    \justifying%
}

\setlength{\parindent}{0em}
\usepackage[none]{hyphenat}

% Tipografías
\RequirePackage[nopatch=footnote]{microtype}

\ifluatex
    \usepackage{fontspec}
    \usepackage{unicode-math}
    \setmainfont{TeX Gyre Heros}
    \setsansfont{TeX Gyre Heros}
    \setmonofont{TeX Gyre Cursor}
    \setmathfont{TeX Gyre Termes Math}
    \renewcommand{\familydefault}{\sfdefault}
\else\ifXeTeX
    \usepackage{fontspec}
    \usepackage{unicode-math}
    \setmainfont{TeX Gyre Heros}
    \setsansfont{TeX Gyre Heros}
    \setmonofont{TeX Gyre Cursor}
    \setmathfont{TeX Gyre Termes Math}
    \renewcommand{\familydefault}{\sfdefault}
\else
    \usepackage[T1]{fontenc}
	\usepackage[utf8]{inputenc}
	\usepackage{tgheros}
	\usepackage{tgtermes}
	\usepackage{inconsolata}
	\renewcommand{\ttdefault}{zi4}
	\renewcommand\familydefault{\sfdefault}
\fi\fi

% Bibliografía
\RequirePackage{csquotes}%

\ifdefstring{\@norma}{ams}{%
	\RequirePackage[
	backend=biber,
	style=numeric,
	sorting=none,
	giveninits=true,
	maxbibnames=10
	]{biblatex}
	\DeclareFieldFormat[article]{volume}{\mkbibbold{#1}}
	\DeclareFieldFormat[article]{year}{\mkbibparens{#1}}
	\DeclareFieldFormat[article]{title}{\mkbibemph{#1}}
	\DeclareFieldFormat[book]{title}{\mkbibemph{#1}}
	\DeclareFieldFormat[article]{journaltitle}{#1}
	\DeclareFieldFormat[article]{number}{no.\addnbspace #1}
	\DeclareFieldFormat{doi}{DOI\addspace #1}
	\DeclareFieldFormat{mrnumber}{MR\addnbspace #1}
	\DeclareBibliographyDriver{article}{%
		\printnames{author}%
		\setunit{\addcomma\space}%
		\printfield{title}%
		\setunit{\addcomma\space}%
		\printfield{journaltitle}%
		\setunit{\addspace}%
		\printfield{volume}%
		\setunit{\addspace}%
		\printfield{year}%
		\setunit{\addcomma\space}%
		\printfield{number}%
		\setunit{\addcomma\space}%
		\printfield{pages}%
		\setunit{\addcomma\space}%
		\printfield{doi}%
		\setunit{\addcomma\space}%
		\printfield{mrnumber}%
		\finentry
	}%
	\DeclareBibliographyDriver{book}{%
		\printnames{author}%
		\setunit{\addcomma\space}%
		\printfield{title}%
		\setunit{\addcomma\space}%
		\printfield{series}%
		\setunit{\addcomma\space}%
		\printfield{volume}%
		\setunit{\addcomma\space}%
		\printlist{publisher}%
		\setunit{\addcomma\space}%
		\printfield{location}%
		\setunit{\addcomma\space}%
		\printfield{year}%
		\setunit{\addcomma\space}%
		\printfield{mrnumber}%
		\finentry
	}%
}{%
	\ifdefstring{\@norma}{ieee}{%
		\RequirePackage[backend=biber,style=ieee]{biblatex}
	}{%
		\ifdefstring{\@norma}{phys}{%
			\RequirePackage[backend=biber,style=phys]{biblatex}
		}{%
			\RequirePackage[backend=biber,style=apa]{biblatex}
			\DeclareLanguageMapping{spanish}{spanish-apa}
		}%
	}%
}

\addbibresource{Recursos/referencias.bib}%

\ifdefstring{\DocLang}{english}{%
	\DefineBibliographyStrings{english}{%
		bibliography = {References},
	}%
}{%
	\DefineBibliographyStrings{spanish}{%
		bibliography = {Referencias},
	}%
}%

% Matemáticas
\RequirePackage{amsmath,amsthm,pgfplots}
\pgfplotsset{compat=1.18}
\newtheorem{theorem}{Teorema}
\newtheorem{proposition}{Proposición}
\newtheorem{lemma}{Lema}
\newtheorem{definition}{Definición}
\newtheorem{remark}{Observación}
\newtheorem{example}{Ejemplo}
\newtheorem{corollary}{Corolario}

\newcommand{\fuente}[1]{%
	\par\small\textbf{Fuente: }#1%
}

% Figuras
\RequirePackage{graphicx,graphbox}
\graphicspath{ {./Figuras/} }

% Tablas
\RequirePackage[export]{adjustbox}
\RequirePackage{caption}
\captionsetup[table]{position=above}

% Capítulos y secciones
\RequirePackage{titlesec}

\titleformat{\chapter}[display]
{\normalfont\huge\bfseries}
{\chaptername\ \thechapter}
{0pt}
{\Huge}
\titlespacing*{\chapter}{0em}{0ex}{5ex}

\titleformat{name=\section,numberless}{\bfseries\LARGE}{}{0pt}{}

\renewenvironment{abstract}{%
	\section*{\abstractname}%f
	\normalfont%
}{}

% Portada
\RequirePackage{titling,xstring}

\newcommand{\autores}{%
	\StrSubstitute{\autor}{, }{\\}%
}

\makeatletter
\renewcommand{\titlepage}{%
	\thispagestyle{empty}%
	\noindent%
	\begin{minipage}[c][0.15\textheight][c]{0.75\textwidth}%
		\centering%
		\begin{spacing}{1.2}%
			\large\MakeUppercase{Universidad de Santiago de Chile}\\%
			\facultad%
		\end{spacing}%
		\small\departamento%
	\end{minipage}%
	\hfill%
	\begin{minipage}[c][0.15\textheight][c]{0.2\textwidth}%
		\includegraphics[width=0.9\linewidth]{logoUsach.png}%
	\end{minipage}\par%

	\begin{minipage}[c][0.825\textheight][c]{0.75\textwidth}%
		\centering%
		\vspace*{\stretch{3}}%
		\large{\titulo}\\%
		\normalsize{\subtitulo\par}%
		\vspace{\stretch{2}}%

		\autores%

		\vspace{\stretch{3}}%
		\small\begin{flushright}%
			Profesor guía:\\%
			\profGuia\par%
			\vspace{\stretch{1}}%
			Tesis para optar al título de\\%
			\titulacion\par%
			\vspace{\stretch{2}}%
		\end{flushright}%

		\small{Santiago, Chile\\%
		\matricula}%
	\end{minipage}%
}
\makeatother

\AtBeginDocument{%
	\titlepage%
}

% Fragmentos de código
\RequirePackage{listings,listingsutf8}

\DeclareCaptionFormat{listing}{\rule{\linewidth}{0.4pt}\par#1#2#3}
\captionsetup[lstlisting]{format=listing,position=bottom}

\lstset{
	basicstyle=\ttfamily\small,
	numbers=left,
	numberstyle=\tiny,
	frame=single,
	breaklines=true,
	captionpos=b,
	keywordstyle=\color{blue}\bfseries,
	commentstyle=\color{gray},
	stringstyle=\color{red},
	showstringspaces=false
}

% Por omisión en pdfLaTeX
\makeatletter
\def\tesisusach@facultad{[Facultad]}
\def\tesisusach@departamento{[Departamento]}
\def\tesisusach@titulacion{[Titulación]}
\makeatother

\endinput