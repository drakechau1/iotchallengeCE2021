var express = require('express');
var app = express();

// Establish connection to MongoDB
const { MongoClient } = require('mongodb');
const uri = "mongodb+srv://iotchallenge:<iotchallenge>@cluster0.umzli.mongodb.net/IOT1?retryWrites=true&w=majority";
const client = new MongoClient(uri, { useNewUrlParser: true, useUnifiedTopology: true });
client.connect(err => {
  const collection = client.db("test").collection("devices");
  // perform actions on the collection object
  client.close();
});

app.set('view engine', 'ejs');
app.set('views', './views');

app.listen(8080, ()=>{
    console.log('listening port 8080');
})

app.get('/', (req, res) =>{
    res.render('home');
})