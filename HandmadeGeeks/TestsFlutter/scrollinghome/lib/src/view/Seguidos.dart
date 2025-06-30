import 'package:flutter/material.dart';

class FollowedPage extends StatefulWidget {
  const FollowedPage({super.key});

  @override
  State<FollowedPage> createState() => _FollowedPageState();
}

class _FollowedPageState extends State<FollowedPage> {
  final List<String> seguidos = [
    'Asignar',
    'Nombres',
    'De ejemplo'
  ];

  void verDetalles(String tienda) {
    // Redireccionar a la tienda
  }

  void dejarDeSeguir(String tienda) {
    setState(() {
      seguidos.remove(tienda);
    });

    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(content: Text('Has dejado de seguir a "$tienda"')),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("Lista de seguidos")),
      body: Padding(
        padding: const EdgeInsets.all(10),
        child: ListView.builder(
          itemCount: seguidos.length,
          itemBuilder: (context, index) {
            final tienda = seguidos[index];
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
                      onTap: () => verDetalles(tienda),
                      child: Text(
                        tienda,
                        style: const TextStyle(
                          fontSize: 16,
                        ),
                      ),
                    ),
                  ),
                  Expanded(
                    flex: 3,
                    child: OutlinedButton(
                      onPressed: () => dejarDeSeguir(tienda),
                      style: OutlinedButton.styleFrom(
                        backgroundColor: Colors.red,
                        foregroundColor: Colors.white,
                        side: const BorderSide(color: Colors.red),
                        alignment: Alignment.center,
                        padding: const EdgeInsets.symmetric(vertical: 12),
                      ),
                      child: const Text(
                        'Dejar de seguir',
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