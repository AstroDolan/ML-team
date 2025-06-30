import 'package:flutter/material.dart';
import 'MiTienda.dart';
import 'Seguidos.dart';
import 'Deseados.dart';

class MiPerfil extends StatelessWidget {
  const MiPerfil({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Nombre del usuario'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              children: [
                CircleAvatar(
                  radius: 45,
                  backgroundImage: AssetImage('lib/imagenes/dylan.png'),
                ),
                const SizedBox(width: 16),
                Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: const [
                    Text('Nombre del usuario', style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold)),
                    Text('Correo'),
                    Text('Teléfono'),
                    Text('Dirección'),
                 ],
                ),
              ],
            ),  
            const SizedBox(height: 24),
            ElevatedButton(
              onPressed: () {
                Navigator.push(
                  context, 
                  MaterialPageRoute(builder: (_) => const MiTienda()),
                );
              },
              child: const Text('Mi Tienda'),
            ),
            const SizedBox(height: 24),
            ElevatedButton(
              onPressed: () {
                Navigator.push(
                  context, 
                  MaterialPageRoute(builder: (_) => const FollowedPage()),
                );
              },
              child: const Text('Tiendas Seguidas'),
            ),
            const SizedBox(height: 24),
            ElevatedButton(
              onPressed: () {
                Navigator.push(
                  context, 
                  MaterialPageRoute(builder: (_) => const WishedPage()),
                );
              },
              child: const Text('Lista de deseos'),
            ),
          ],
        ),
      ),
    );
  }
}