const bodyParser = require('body-parser');
const express = require('express');
const app = express();

const user = require('./routes/user');
const main = require('./routes/main');

app.set('view engine', 'ejs');
app.set('views', './views');
app.use(express.static("public"));
app.use(bodyParser.json());

// Middlewares
app.use('/', main);
app.use('/user', user);

// Listening to the server
app.listen(process.env.PORT || 8080, () => { console.log('Connected to the server') });
