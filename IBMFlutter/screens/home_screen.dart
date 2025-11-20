import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:collection/collection.dart';
import '../models/time_entry.dart';
import '../provider/project_task_provider.dart';
import 'add_time_entry_screen.dart';
import 'project_task_management_screen.dart';

class HomeScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return DefaultTabController(
      length: 2,
      child: Scaffold(
        appBar: AppBar(
          backgroundColor: const Color(0xFF7777AA),
          title: Text('Time Tracking', style: TextStyle(color: Colors.white)),
          bottom: TabBar(
            tabs: [
              Tab(child: Text('All Entries', style: TextStyle(color: Colors.white))),
              Tab(child: Text('Grouped by Projects', style: TextStyle(color: Colors.white))),
            ],
          ),
        ),
        drawer: Drawer(
          child: ListView(
            children: [
              DrawerHeader(child: Text('Menu')),
              ListTile(
                title: Text('Your Projects'),
                onTap: () {
                  Navigator.pop(context);
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                        builder: (_) =>
                            ProjectTaskManagementScreen(initialIndex: 0)),
                  );
                },
              ),
              ListTile(
                title: Text('Your Tasks'),
                onTap: () {
                  Navigator.pop(context);
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                        builder: (_) =>
                            ProjectTaskManagementScreen(initialIndex: 1)),
                  );
                },
              ),
            ],
          ),
        ),
        body: Consumer<TimeEntryProvider>(
          builder: (context, provider, child) {
            final List<TimeEntry> items = provider.entries;
            return TabBarView(
              children: [
                items.isEmpty
                    ? Center(child: Text('Theres No entries yet. Add one now!'))
                    : ListView.builder(
                        itemCount: items.length,
                        itemBuilder: (context, index) {
                          final e = items[index];
                          return Dismissible(
                            key: ValueKey(e.id),
                            background: Container(color: Colors.red),
                            onDismissed: (_) {
                              Provider.of<TimeEntryProvider>(context,
                                      listen: false)
                                  .deleteTimeEntry(e.id);
                            },
                            child: ListTile(
                              title:
                                  Text('${e.projectId} - ${e.totalTime} hours'),
                              subtitle: Text(
                                  '${e.date.toString()} - Notes: ${e.notes}'),
                            ),
                          );
                        },
                      ),
                items.isEmpty
                    ? Center(child: Text('You have no projects yet!'))
                    : Builder(
                        builder: (context) {
                          final grouped = groupBy<TimeEntry, String>(
                              items, (e) => e.projectId);
                          final tiles = <Widget>[];
                          grouped.forEach((project, list) {
                            tiles.add(
                              ExpansionTile(
                                title: Text(project),
                                children: list
                                    .map((e) => ListTile(
                                          title: Text(
                                              '${e.taskId} - ${e.totalTime} hours'),
                                          subtitle: Text(
                                              '${e.date.toString()} - Notes: ${e.notes}'),
                                        ))
                                    .toList(),
                              ),
                            );
                          });
                          return ListView(children: tiles);
                        },
                      ),
              ],
            );
          },
        ),
        floatingActionButton: FloatingActionButton(
          onPressed: () {
            Navigator.push(context,
                MaterialPageRoute(builder: (_) => AddTimeEntryScreen()));
          },
          child: Icon(Icons.add),
        ),
      ),
    );
  }
}