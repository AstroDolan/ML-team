import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'login.dart';

class SigninPage extends StatefulWidget {
  const SigninPage({super.key});

  @override
  State<SigninPage> createState() => _SigninPageState();
}

class _SigninPageState extends State<SigninPage> {
  final TextEditingController nameController = TextEditingController();
  final TextEditingController emailController = TextEditingController();
  final TextEditingController passwordController = TextEditingController();
  final TextEditingController apellidoController = TextEditingController();
  bool _isLoading = false;

  void showSnackbar(String msg) {
    final snack = SnackBar(content: Text(msg));
    ScaffoldMessenger.of(context).showSnackBar(snack);
  }

  // 🔥 FUNCIÓN: Registro con backend Django
  Future<void> registerWithBackend() async {
    if (emailController.text.isEmpty ||
        passwordController.text.isEmpty ||
        nameController.text.isEmpty) {
      showSnackbar("Nombre, email y contraseña son requeridos");
      return;
    }

    setState(() {
      _isLoading = true;
    });

    try {
      // 📡 Conectar a tu backend Django
      final response = await http.post(
        Uri.parse('http://127.0.0.1:8000/api/registro-usuario/'),
        headers: {'Content-Type': 'application/json'},
        body: json.encode({
          'correo': emailController.text,
          'contraseña': passwordController.text,
          'nombre': nameController.text,
          'apellido': apellidoController.text.isEmpty
              ? 'vacio_'
              : apellidoController.text,
        }),
      );

      if (response.statusCode == 201) {
        final data = json.decode(response.body);
        showSnackbar("Usuario registrado exitosamente!");

        // Redirigir al login
        Navigator.pushReplacement(
          context,
          MaterialPageRoute(builder: (_) => const LoginPage()),
        );
      } else {
        final error = json.decode(response.body);
        showSnackbar(error['error'] ?? 'Error de registro');
      }
    } catch (e) {
      showSnackbar("Error de conexión: $e");
    } finally {
      setState(() {
        _isLoading = false;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Registro'),
        backgroundColor: Colors.cyan,
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: SingleChildScrollView(
          child: Column(
            children: [
              const SizedBox(height: 20),
              Container(
                alignment: Alignment.center,
                child: Image.asset(
                  'lib/imagenes/perro.jpeg',
                  width: 120,
                  height: 70,
                  errorBuilder: (context, error, stackTrace) {
                    return const Icon(
                      Icons.image,
                      size: 70,
                      color: Colors.cyan,
                    );
                  },
                ),
              ),
              const SizedBox(height: 20),
              Container(
                alignment: Alignment.center,
                child: const Text(
                  'Crear Cuenta',
                  style: TextStyle(
                    fontSize: 24,
                    fontWeight: FontWeight.bold,
                    color: Colors.cyan,
                  ),
                ),
              ),
              const SizedBox(height: 30),
              Container(
                padding: const EdgeInsets.all(10),
                child: TextField(
                  controller: nameController,
                  decoration: const InputDecoration(
                    border: OutlineInputBorder(),
                    labelText: 'Nombre *',
                    prefixIcon: Icon(Icons.person),
                  ),
                ),
              ),
              Container(
                padding: const EdgeInsets.all(10),
                child: TextField(
                  controller: apellidoController,
                  decoration: const InputDecoration(
                    border: OutlineInputBorder(),
                    labelText: 'Apellido (opcional)',
                    prefixIcon: Icon(Icons.person_outline),
                  ),
                ),
              ),
              Container(
                padding: const EdgeInsets.all(10),
                child: TextField(
                  controller: emailController,
                  decoration: const InputDecoration(
                    border: OutlineInputBorder(),
                    labelText: 'Correo electrónico *',
                    prefixIcon: Icon(Icons.email),
                  ),
                  keyboardType: TextInputType.emailAddress,
                ),
              ),
              Container(
                padding: const EdgeInsets.all(10),
                child: TextField(
                  controller: passwordController,
                  obscureText: true,
                  decoration: const InputDecoration(
                    border: OutlineInputBorder(),
                    labelText: 'Contraseña *',
                    prefixIcon: Icon(Icons.lock),
                  ),
                ),
              ),
              const SizedBox(height: 20),
              Container(
                height: 50,
                width: double.infinity,
                padding: const EdgeInsets.symmetric(horizontal: 10),
                child: ElevatedButton(
                  onPressed: _isLoading ? null : registerWithBackend,
                  style: ElevatedButton.styleFrom(
                    backgroundColor: Colors.cyan,
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(8),
                    ),
                  ),
                  child: _isLoading
                      ? const CircularProgressIndicator(color: Colors.white)
                      : const Text(
                          'Registrarse',
                          style: TextStyle(color: Colors.white, fontSize: 18),
                        ),
                ),
              ),
              const SizedBox(height: 20),
              Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  const Text('¿Ya tienes cuenta?'),
                  TextButton(
                    onPressed: () {
                      Navigator.pushReplacement(
                        context,
                        MaterialPageRoute(builder: (_) => const LoginPage()),
                      );
                    },
                    child: const Text(
                      'Iniciar Sesión',
                      style: TextStyle(
                        color: Colors.cyan,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }
}