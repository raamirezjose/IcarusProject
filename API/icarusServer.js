const express = require('express');
const app = express();
var MongoClient = require('mongodb').MongoClient;
const SocketIO = require('socket.io');
const path = require('path');
const { mongoPath,mongoDb,collectionRouteMap } = require('../env');

app.set('port',process.env.PORT || 3000);
 app.use(express.static(path.join(__dirname,'../front'))); 

const server = app.listen(app.get('port'),()=>{
  console.log('serve on port',app.get('port'));
});

const io = SocketIO.listen(server);


io.on('connection',(socket)=>{
  socket.on('postroutemap',(data)=>{
      postRouteMap(data);  
      getRouteMap((dataRoute)=>{
        io.emit('getroutemap',dataRoute);
      });   
  });

    getRouteMap((dataRoute)=>{
      io.emit('getroutemap',dataRoute);
    }); 
})  

/* 
const getJourneys  =  () => {
  MongoClient.connect(mongoPath,
  function (err, client) {
    if (err) throw err;
    var db = client.db(mongoDb);
    db.collection(collectionRouteMap).findOne({}, function (findErr, result) {
      if (findErr) throw findErr;
      console.log(result.msg);
      client.close();
      res.json({"msg":result.msg});
    });
  }); 
}  */

const postRouteMap = (data) =>{
  MongoClient.connect(mongoPath,
   function (err, client) {
    if (err) throw err;
  
    var db = client.db(mongoDb);
  
    db.collection(collectionRouteMap).insertOne(data, function (findErr, result) {
      if (findErr) throw findErr;
      client.close();      
    });
  }); 
}

const getRouteMap = (call) => {
 MongoClient.connect(mongoPath,
    function (err, client) {
       if (err) throw err;
       var db = client.db(mongoDb);  
       db.collection(collectionRouteMap).find().toArray(function (err, result) {
          if (err) throw err;
           call(result);
          })
    }); 
}
/* db.coleccion.deleteMany({}) */
