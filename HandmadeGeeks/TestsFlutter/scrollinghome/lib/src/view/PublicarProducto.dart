import 'package:flutter/material.dart';

class PublicarProducto extends StatefulWidget {
  const PublicarProducto({super.key});

  @override
  State<PublicarProducto> createState() => _PublicarProductoState();
}

class _PublicarProductoState extends State<PublicarProducto> {
  final _formKey = GlobalKey<FormState>();

  final TextEditingController _nombreController = TextEditingController();
  final TextEditingController _descripcionController = TextEditingController();
  final TextEditingController _stockController = TextEditingController();
  final TextEditingController _precioController = TextEditingController();

  String? _categoriaSeleccionada;
  final List<String> _categorias = [
    'Lana',
    'Ropa',
    'Arcilla',
  ];

  @override
  void dispose() {
    _nombreController.dispose();
    _descripcionController.dispose();
    _stockController.dispose();
    _precioController.dispose();
    super.dispose();
  }

  void _confirmar() {
    if (_formKey.currentState?.validate() ?? false) {
      debugPrint('Producto validado para publicación');
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Producto en análisis. Todo producto debe ser revisado por un administrador antes de ser publicado.')),
      );
      _nombreController.clear();
      _descripcionController.clear();
      _stockController.clear();
      _precioController.clear();
      setState(() {
        _categoriaSeleccionada = null;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Publicar Producto')),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Form(
          key: _formKey,
          child: ListView(
            children: [
              TextFormField(
                controller: _nombreController,
                decoration: const InputDecoration(
                  labelText: 'Nombre del producto',
                  border: OutlineInputBorder(),
                ),
                validator: (value) {
                  if (value == null || value.trim().isEmpty) {
                    return 'Por favor ingresa el nombre';
                  }
                  return null;
                },
              ),
              const SizedBox(height: 16),

              GestureDetector(
                onTap: () {
                  ScaffoldMessenger.of(context).showSnackBar(
                    const SnackBar(content: Text('Función para subir imágenes no implementada')),
                  );
                },
                child: Container(
                  height: 140,
                  color: Colors.grey[300],
                  child: const Center(
                    child: Icon(Icons.add_photo_alternate, size: 50, color: Colors.grey),
                  ),
                ),
              ),
              const SizedBox(height: 16),

              TextFormField(
                controller: _stockController,
                keyboardType: TextInputType.number,
                decoration: const InputDecoration(
                  labelText: 'Cantidad en stock',
                  border: OutlineInputBorder(),
                ),
                validator: (value) {
                  if (value == null || value.trim().isEmpty) {
                    return 'Por favor ingresa la cantidad';
                  }
                  final cantidad = int.tryParse(value);
                  if (cantidad == null || cantidad < 0) {
                    return 'Ingrese una cantidad válida';
                  }
                  return null;
                },
              ),
              const SizedBox(height: 16),

              TextFormField(
                controller: _precioController,
                keyboardType: const TextInputType.numberWithOptions(decimal: true),
                decoration: const InputDecoration(
                  labelText: 'Precio',
                  prefixText: '\$ ',
                  border: OutlineInputBorder(),
                ),
                validator: (value) {
                  if (value == null || value.trim().isEmpty) {
                    return 'Por favor ingresa el precio';
                  }
                  final precio = double.tryParse(value);
                  if (precio == null || precio < 0) {
                    return 'Ingrese un precio válido';
                  }
                  return null;
                },
              ),
              const SizedBox(height: 16),

              TextFormField(
                controller: _descripcionController,
                maxLines: 3,
                decoration: const InputDecoration(
                  labelText: 'Descripción breve',
                  border: OutlineInputBorder(),
                ),
                validator: (value) {
                  if (value == null || value.trim().isEmpty) {
                    return 'Por favor ingresa una descripción';
                  }
                  return null;
                },
              ),
              const SizedBox(height: 16),

              DropdownButtonFormField<String>(
                value: _categoriaSeleccionada,
                decoration: const InputDecoration(
                  labelText: 'Categoría',
                  border: OutlineInputBorder(),
                ),
                items: _categorias
                    .map((cat) => DropdownMenuItem(value: cat, child: Text(cat)))
                    .toList(),
                onChanged: (value) {
                  setState(() {
                    _categoriaSeleccionada = value;
                  });
                },
                validator: (value) {
                  if (value == null) {
                    return 'Por favor selecciona una categoría';
                  }
                  return null;
                },
              ),
              const SizedBox(height: 24),

              ElevatedButton(
                onPressed: _confirmar,
                child: const Padding(
                  padding: EdgeInsets.symmetric(vertical: 14),
                  child: Text('Confirmar', style: TextStyle(fontSize: 16)),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}