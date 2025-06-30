import 'package:flutter/material.dart';

class WishedPage extends StatefulWidget {
  const WishedPage({super.key});

  @override
  State<WishedPage> createState() => _WishedPageState();
}

class _WishedPageState extends State<WishedPage> {
  final List<String> deseados = [
    'Producto',
    'Random',
    'Idk'
  ];

  void verDetalles(String producto) {
    // Redireccionar al producto
  }

  void quitarDeDeseados(String producto) {
    setState(() {
      deseados.remove(producto);
    });

    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(content: Text('Has dejado de seguir a "$producto"')),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("Lista de deseados")),
      body: Padding(
        padding: const EdgeInsets.all(10),
        child: ListView.builder(
          itemCount: deseados.length,
          itemBuilder: (context, index) {
            final producto = deseados[index];
            return Container(
              margin: const EdgeInsets.symmetric(vertical: 6),
              padding: const EdgeInsets.all(12),
              decoration: BoxDecoration(
                color: const Color.fromARGB(255, 241, 249, 255),
                borderRadius: BorderRadius.circular(8),
              ),
              child: Row(
                children: [
                  Expanded(
                    flex: 7,
                    child: InkWell(
                      onTap: () => verDetalles(producto),
                      child: Text(
                        producto,
                        style: const TextStyle(
                          fontSize: 16,
                        ),
                      ),
                    ),
                  ),
                  Expanded(
                    flex: 3,
                    child: OutlinedButton(
                      onPressed: () => quitarDeDeseados(producto),
                      style: OutlinedButton.styleFrom(
                        backgroundColor: Colors.red,
                        foregroundColor: Colors.white,
                        side: const BorderSide(color: Colors.red),
                        alignment: Alignment.center,
                        padding: const EdgeInsets.symmetric(vertical: 12),
                      ),
                      child: const Text(
                        'Quitar de deseados',
                        textAlign: TextAlign.center,
                        style: TextStyle(fontSize: 12),
                      ),
                    ),
                  ),
                ],
              ),
            );
          },
        ),
      ),
    );
  }
}