import 'package:flutter/material.dart';
import 'package:scrollinghome/src/view/main_scaffold.dart';

class App extends StatelessWidget {
  const App({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      debugShowCheckedModeBanner: false,
      home: MainScaffold(),
    );
  }
}
