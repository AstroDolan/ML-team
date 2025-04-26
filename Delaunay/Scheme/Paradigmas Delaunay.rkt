#lang racket

; || Funciones de lectura de un .csv ||
(define (split str sep)
  (define (loop chars acc current)
    (cond
      ((null? chars)
       (reverse (cons (list->string (reverse current)) acc)))
      ((char=? (car chars) sep)
       (loop (cdr chars) (cons (list->string (reverse current)) acc) '()))
      (else
       (loop (cdr chars) acc (cons (car chars) current)))))
  (loop (string->list str) '() '()))

(define (leer-csv archivo)
  (call-with-input-file archivo
    (lambda (in)
      (let loop ((linea (read-line in))
                 (resultado '()))
        (if (eof-object? linea)
            (reverse resultado)
            (let ((campos (map string->number (split linea #\,))))
              (loop (read-line in) (cons campos resultado))))))))


(define (leer-puntos-ruta ruta)
  (call-with-input-file ruta
    (lambda (in)
      (let loop ((linea (read-line in)) ; saltar encabezado
                 (acum '()))
        (let ((siguiente (read-line in))) ; leer nueva línea
          (if (eof-object? siguiente)
              (reverse acum)
              (let* ((partes (split siguiente #\,)))
                (if (= (length partes) 3)
                    (let ((x (string->number (list-ref partes 0)))
                          (y (string->number (list-ref partes 1)))
                          (z (string->number (list-ref partes 2))))
                      (if (= z 1)
                          (loop siguiente (cons (list x y) acum))
                          (loop siguiente acum)))
                    (loop siguiente acum))))))))) ; <- HAY UN PARÉNTESIS DEMÁS AQUÍ




(define (escribir-csv ruta datos)
  (call-with-output-file ruta
    (lambda (out)
      (for-each
       (lambda (fila)
         (fprintf out "~a\n"
                  (string-join (map number->string (flatten fila)) ",")))
       datos))
    #:exists 'replace))

;||--- obj (1) valid-Delaunay --- ||

(define (cuadrado x) (* x x))

(define (valid-Delaunay A B C D)
  (let* ((Ax (car A)) (Ay (cadr A))
         (Bx (car B)) (By (cadr B))
         (Cx (car C)) (Cy (cadr C))
         (Dx (car D)) (Dy (cadr D))
         (a1 (- Ax Dx)) (a2 (- Ay Dy))
         (b1 (- Bx Dx)) (b2 (- By Dy))
         (c1 (- Cx Dx)) (c2 (- Cy Dy))
         (a3 (+ (* a1 a1) (* a2 a2)))
         (b3 (+ (* b1 b1) (* b2 b2)))
         (c3 (+ (* c1 c1) (* c2 c2)))
         (det (- (+ (* a1 b2 c3)
                    (* a2 b3 c1)
                    (* a3 b1 c2))
                 (+ (* a3 b2 c1)
                    (* a2 b1 c3)
                    (* a1 b3 c2)))))
    (if (>= det 0) #t #f)))


; ||--- obj (2) delaunay-triangles ---||

(define (every pred? lst)
  (cond
    ((null? lst) #t)
    ((pred? (car lst)) (every pred? (cdr lst)))
    (else #f)))

(define (combinaciones-de-3 lista)
  (let ((resultado '()))
    (define (loop l1)
      (cond
        ((null? l1) resultado)
        (else
         (let ((a (car l1)))
           (define (loop2 l2)
             (cond
               ((null? l2) '())
               (else
                (let ((b (car l2)))
                  (define (loop3 l3)
                    (cond
                      ((null? l3) '())
                      (else
                       (let ((c (car l3)))
                         (set! resultado (cons (list a b c) resultado))
                         (loop3 (cdr l3))))))
                  (loop3 (cdr l2))
                  (loop2 (cdr l2))))))
           (loop2 (cdr l1))
           (loop (cdr l1))))))
    (loop lista)
    resultado))

(define (delaunay-triangles puntos)
  (define (triangulo-valido? t)
    (let ((a (car t)) (b (cadr t)) (c (caddr t)))
      (define (no-invalida? p)
        (or (equal? p a) (equal? p b) (equal? p c)
            (valid-Delaunay a b c p)))
      (every no-invalida? puntos)))
  (filter triangulo-valido? (combinaciones-de-3 puntos)))


;||--- obj (3) puntos-asociados-a-triangulos de fn (1) y (2) ---||
 
(define (unique lst)
  (if (null? lst)
      '()
      (let ((x (car lst)))
        (if (member x (cdr lst))
            (unique (cdr lst))
            (cons x (unique (cdr lst)))))))

(define (puntos-asociados-a-triangulos puntos)
  (let ((triangulos (delaunay-triangles puntos)))
    (unique (foldr append '() triangulos))))


;||--- obj (4) triangle-centers y exterior-edge-centers ---||

(define (promedio a b c)
  (/ (+ a b c) 3.0))

(define (centroide-triangulo t)
  (let* ((p1 (car t)) (p2 (cadr t)) (p3 (caddr t))
         (x (promedio (car p1) (car p2) (car p3)))
         (y (promedio (cadr p1) (cadr p2) (cadr p3))))
    (list x y)))

(define (triangle-centers triangulos)
  (map centroide-triangulo triangulos))


(define (less-than? p1 p2)
  (or (< (car p1) (car p2))
      (and (= (car p1) (car p2)) (< (cadr p1) (cadr p2)))))

(define (arista p1 p2)
  (if (less-than? p1 p2)
      (list p1 p2)
      (list p2 p1)))

(define (punto-medio p1 p2)
  (list (/ (+ (car p1) (car p2)) 2.0)
        (/ (+ (cadr p1) (cadr p2)) 2.0)))

(define (aristas-de-triangulo t)
  (let ((a (car t)) (b (cadr t)) (c (caddr t)))
    (list (arista a b) (arista b c) (arista c a))))

(define (contar-aristas triangulos)
  (let ((todas (foldr append '() (map aristas-de-triangulo triangulos))))
    (define (agregar aristas tabla)
      (if (null? aristas)
          tabla
          (let* ((a (car aristas))
                 (existe (assoc a tabla)))
            (agregar (cdr aristas)
                     (if existe
                         (cons (cons a (+ 1 (cdr existe)))
                               (remove (lambda (x) (equal? (car x) a)) tabla))
                         (cons (cons a 1) tabla))))))
    (agregar todas '())))

(define (exterior-edge-centers triangulos)
  (let* ((tabla (contar-aristas triangulos))
         (exteriores (filter (lambda (par) (= (cdr par) 1)) tabla)))
    (map (lambda (par)
           (punto-medio (car (car par)) (cadr (car par))))
         exteriores)))


; || Main ||

(define (main)
  (define puntos (leer-puntos-ruta "puntosPrueba.csv"))
  (define triangulos (delaunay-triangles puntos))
  (define aristas (foldr append '() (map aristas-de-triangulo triangulos)))
  (define centros (triangle-centers triangulos))
  (define centros-exteriores (exterior-edge-centers triangulos))

  ;; Guardar a archivos
  (escribir-csv "edges-delaunay.csv" aristas)
  (escribir-csv "centers.csv" centros)
  (escribir-csv "centers-convex-hull.csv" centros-exteriores))

(main)