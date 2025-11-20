import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../provider/project_task_provider.dart';

class ProjectTaskManagementScreen extends StatefulWidget {
  final int initialIndex;
  const ProjectTaskManagementScreen({this.initialIndex = 0, super.key});

  @override
  State<ProjectTaskManagementScreen> createState() =>
      _ProjectTaskManagementScreenState();
}

class _ProjectTaskManagementScreenState
    extends State<ProjectTaskManagementScreen>
    with SingleTickerProviderStateMixin {
  late TabController _tab;
  final TextEditingController _ctrl = TextEditingController();

  @override
  void initState() {
    super.initState();
    _tab = TabController(length: 2, vsync: this, initialIndex: widget.initialIndex);
  }

  @override
  void dispose() {
    _tab.dispose();
    _ctrl.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final Color scaffoldBg = Theme.of(context).scaffoldBackgroundColor;

    return Scaffold(
      appBar: AppBar(
        // Opción 1: literal ARGB con alpha (recomendado)
        backgroundColor: const Color(0xFF7777AA),
        title: const Text('Manage Projects & Tasks'),
        bottom: TabBar(
          controller: _tab,
          tabs: const [
            Tab(child: Text('Projects', style: TextStyle(color: Colors.white)), icon: Icon(Icons.folder)),
            Tab(child: Text('Tasks', style: TextStyle(color: Colors.white)), icon: Icon(Icons.check_box)),
          ],
        ),
      ),
      body: TabBarView(
        controller: _tab,
        children: [
          // Projects tab
          Consumer<ProjectTaskProvider>(
            builder: (context, provider, _) {
              return ListView.separated(
                padding: const EdgeInsets.all(12),
                itemCount: provider.projects.length,
                separatorBuilder: (_, __) => const Divider(),
                itemBuilder: (context, index) {
                  final project = provider.projects[index];
                  return ListTile(
                    title: Text(project),
                    trailing: IconButton(
                      icon: const Icon(Icons.delete, color: Colors.redAccent),
                      tooltip: 'Eliminar proyecto',
                      onPressed: () {
                        // Confirmación simple (opcional)
                        showDialog(
                          context: context,
                          builder: (ctx) => AlertDialog(
                            title: const Text('Confirmar eliminación'),
                            content: Text('Eliminar "$project"?'),
                            actions: [
                              TextButton(
                                onPressed: () => Navigator.of(ctx).pop(),
                                child: const Text('Cancelar'),
                              ),
                              TextButton(
                                onPressed: () {
                                  Provider.of<ProjectTaskProvider>(context, listen: false)
                                      .deleteProject(project);
                                  Navigator.of(ctx).pop();
                                },
                                child: const Text('Eliminar'),
                              ),
                            ],
                          ),
                        );
                      },
                    ),
                  );
                },
              );
            },
          ),

          // Tasks tab (simple list)
          Consumer<ProjectTaskProvider>(
            builder: (context, provider, _) {
              return ListView.separated(
                padding: const EdgeInsets.all(12),
                itemCount: provider.tasks.length,
                separatorBuilder: (_, __) => const Divider(),
                itemBuilder: (context, index) {
                  final task = provider.tasks[index];
                  return ListTile(
                    title: Text(task),
                  );
                },
              );
            },
          ),
        ],
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () {
          // ejemplo: abrir diálogo para añadir proyecto (implementar según necesidad)
          showDialog(
            context: context,
            builder: (ctx) {
              _ctrl.clear();
              return AlertDialog(
                title: const Text('Añadir proyecto'),
                content: TextField(
                  controller: _ctrl,
                  decoration: const InputDecoration(hintText: 'Nombre del proyecto'),
                ),
                actions: [
                  TextButton(onPressed: () => Navigator.of(ctx).pop(), child: const Text('Cancelar')),
                  TextButton(
                    onPressed: () {
                      final name = _ctrl.text.trim();
                      if (name.isNotEmpty) {
                        Provider.of<ProjectTaskProvider>(context, listen: false).addProject(name);
                      }
                      Navigator.of(ctx).pop();
                    },
                    child: const Text('Añadir'),
                  ),
                ],
              );
            },
          );
        },
        child: const Icon(Icons.add),
      ),
    );
  }
}