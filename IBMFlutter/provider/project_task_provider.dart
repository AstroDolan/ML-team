import 'dart:convert';
import 'dart:html' as html;
import 'package:flutter/foundation.dart';
import 'package:shared_preferences/shared_preferences.dart';
import '../models/time_entry.dart';

class TimeEntryProvider with ChangeNotifier {
  late SharedPreferences _prefs;
  final List<TimeEntry> _entries = [];
  List<TimeEntry> get entries => _entries;

  Future<void> init() async {
    _prefs = await SharedPreferences.getInstance();
    String? s = html.window.localStorage['entries'];
    s ??= _prefs.getString('entries');
    if (s != null && s.isNotEmpty) {
      final List list = jsonDecode(s);
      _entries
        ..clear()
        ..addAll(list.map((e) => TimeEntry(
              id: e['id'],
              projectId: e['projectId'],
              taskId: e['taskId'],
              totalTime: (e['totalTime'] as num).toDouble(),
              date: DateTime.parse(e['date']),
              notes: e['notes'],
            )));
    }
    notifyListeners();
  }

  void _save() {
    final s = jsonEncode(_entries
        .map((e) => {
              'id': e.id,
              'projectId': e.projectId,
              'taskId': e.taskId,
              'totalTime': e.totalTime,
              'date': e.date.toIso8601String(),
              'notes': e.notes,
            })
        .toList());
    _prefs.setString('entries', s);
    html.window.localStorage['entries'] = s;
  }

  void addTimeEntry(TimeEntry entry) {
    _entries.add(entry);
    _save();
    notifyListeners();
  }

  void deleteTimeEntry(String id) {
    _entries.removeWhere((e) => e.id == id);
    _save();
    notifyListeners();
  }
}

class ProjectTaskProvider with ChangeNotifier {
  final List<String> projects = ['Project 1', 'Project 2', 'Project 3'];
  final List<String> tasks = ['Task 1', 'Task 2', 'Task 3'];

  void addProject(String name) {
    projects.add(name);
    notifyListeners();
  }

  void addTask(String name) {
    tasks.add(name);
    notifyListeners();
  }

  void deleteProject(String name) {
    projects.remove(name);
    notifyListeners();
  }
}