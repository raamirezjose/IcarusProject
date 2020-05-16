import express from 'express';
import { ApolloServer } from 'apollo-server-express';
import RouteMap from './models/routemap';
import mongoose from 'mongoose';
import { mongoDb,port } from '../env';
import http  from 'http';
import mqtt from 'mqtt';

var client  = mqtt.connect('mqtt://35.208.61.217');

export const conectionmqtt = (direction)=>{
   
  client.publish('JuggernautDirection', direction);
  console.log(direction);
};
  
client.on('connect', function () {
 
});  


const app = express(); 

mongoose.connect(`mongodb://localhost/${mongoDb}`,{useNewUrlParser: true})
  .then(() => console.log(`conected to ${mongoDb}`))
  .catch(err => console.log(err));

import typeDefs from './schema';
import resolvers from './resolvers'; 

app.set('port', process.env.PORT || port);

const server = new ApolloServer({
  typeDefs,
  resolvers,
  context: {
    RouteMap
  },
  introspection: true,
  playground: true,
  playground: {
      endpoint: `http://localhost:${app.get('port')}/graphql`,
      settings: {
          'editor.theme': 'dark'
      }
  }
});

server.applyMiddleware({
    app
  });

  /* Create GraphQL subscriptions server */
  const httpServer = http.createServer(app);
  server.installSubscriptionHandlers(httpServer);
  /* Start server */
  httpServer.listen(port, () => {
    console.log(`http://localhost:${port}${server.graphqlPath}`)
    console.log(`ws://localhost:${port}${server.subscriptionsPath}`)
  })

