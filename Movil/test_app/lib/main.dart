import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'button.dart';
import 'buttontapped.dart';

void main() => runApp(MyApp());

Future<Post> fetchPost() async {
  final response = await http.post('http://localhost:3100/postroutemap');
  final responseJson = json.decode(response.body);
  print(responseJson);
  return Post.fromJson(responseJson);
}

class Post {
  final int userId;
  final int id;
  final String title;
  final String body;

  Post({this.userId, this.id, this.title, this.body});

  factory Post.fromJson(Map<String, dynamic> json) {
    return Post(
      userId: json['userId'],
      id: json['id'],
      title: json['title'],
      body: json['body'],
    );
  }
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: MyHomePage(),
    );
  }
}
 
class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}


class _MyHomePageState extends State<MyHomePage> {

  // SET ICONS HERE
  var iconeast1 = Icons.arrow_right;
  var iconnorth2 = Icons.arrow_drop_up;
  var iconsouth3 = Icons.arrow_drop_down;
  var iconwest4 = Icons.arrow_left;
  var iconstop0 = Icons.stop;

  bool btnPressEast1 = false;
  bool btnPressNorth2 = false;
  bool btnPressSouth3 = false;
  bool btnPressWest4 = false;
  bool btnPressStop0 = false;

  void _letsPressStop0(){
    print("stop");
    stopPost();
    setState(() {
      btnPressStop0 = true;
      btnPressEast1 = false;
      btnPressNorth2 = false;
      btnPressSouth3 = false;
      btnPressWest4 = false;
    });
  }

  void _letsPressEast1() {
    print("east");
    eastPost();
    setState(() {
      btnPressStop0 = false;
      btnPressEast1 = true;
      btnPressNorth2 = false;
      btnPressSouth3 = false;
      btnPressWest4 = false;
    });
  }

  void _letsPressNorth2() {
    print("north");
    northPost();
    setState(() {
      btnPressStop0 = false;
      btnPressEast1 = false;
      btnPressNorth2 = true;
      btnPressSouth3 = false;
      btnPressWest4 = false;
    });
  }

  void _letsPressSouth3() {
    print("south");
    southPost();
    setState(() {
      btnPressStop0 = false;
      btnPressEast1 = false;
      btnPressNorth2 = false;
      btnPressSouth3 = true;
      btnPressWest4 = false;
    });
  }

  void _letsPressWest4() {
    print("west");
    westPost();
    setState(() {
      btnPressStop0 = false;
      btnPressEast1 = false;
      btnPressNorth2 = false;
      btnPressSouth3 = false;
      btnPressWest4 = true;
    });
  }

@override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Routes',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: Scaffold(
          body: Container(
        decoration: BoxDecoration(
            image: DecorationImage(
                image: AssetImage('images/f.jpeg'), fit: BoxFit.cover)),
        child: new Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                getButton(btnPressNorth2, iconnorth2 ,pAccion: () => [_letsPressNorth2()])
                ],
              ),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                getButton(btnPressWest4, iconwest4 ,pAccion: () => [_letsPressWest4()]),
                getButton(btnPressStop0, iconstop0 ,pAccion: () => [_letsPressStop0()]),
                getButton(btnPressEast1, iconeast1 ,pAccion: () => [_letsPressEast1()]) 
                ], 
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                getButton(btnPressSouth3, iconsouth3 ,pAccion: () => [_letsPressSouth3()])
              ],
            )
          ],
        ),
      )),
    );
  }


  Expanded getButton(bool buttonPressed, IconData icon, {Function pAccion}) {  

      var oBoton =  Expanded(
                    child: GestureDetector(
                        // FOURTH BUTTON
                        onTap: pAccion,
                        child: buttonPressed
                            ? ButtonTapped(
                                icon: icon,
                              )
                            : MyButton(
                                icon: icon,
                              )),
                  );
      return oBoton;
  }  

  southPost() async {
    String url = 'http://35.208.61.217:3300/postroutemap';
    Map map = {
      'data': {'time': '2'},
    };

    print(await apiRequest(url, map));
  }

  northPost() async {
    String url = 'http://35.208.61.217:3200/postroutemap';
    Map map = {
      'data': {'time': '3'},
    };

    print(await apiRequest(url, map));
  }

  eastPost() async {
    String url = 'http://35.208.61.217:3100/postroutemap';
    Map map = {
      'data': {'time': '4'},
    };

    print(await apiRequest(url, map));
  }

  westPost() async {
    String url = 'http://35.208.61.217:3400/postroutemap';
    Map map = {
      'data': {'time': '5'},
    };

    print(await apiRequest(url, map));
  }

  stopPost() async {
    String url = 'http://35.208.61.217:3500/postroutemap';
    Map map = {
      'data': {'time': '0'},
    };

    print(await apiRequest(url, map));
  }

  Future<String> getData() async {
    List data;
    var response = await http.get(
        Uri.encodeFull("https://jsonplaceholder.typicode.com/posts"),
        headers: {"Accept": "application/json"});
    data = json.decode(response.body);
    print(data[1]["title"]);

    return "Success!";
  }

  Future<String> apiRequest(String url, Map jsonMap) async {
    HttpClient httpClient = new HttpClient();
    HttpClientRequest request = await httpClient.postUrl(Uri.parse(url));
    request.headers.set('content-type', 'application/json');
    request.add(utf8.encode(json.encode(jsonMap)));
    HttpClientResponse response = await request.close();
    // todo - you should check the response.statusCode
    String reply = await response.transform(utf8.decoder).join();
    httpClient.close();
    return reply;
  }
}