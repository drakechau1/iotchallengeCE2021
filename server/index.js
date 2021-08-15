const bodyParser = require('body-parser');
const express = require('express');
const app = express();
const connectDB = require('./models/connectDB');
const postSchema = require('./models/postSchema');

app.use(express.static("public"));
app.use(bodyParser.json());
app.set('view engine', 'ejs');
app.set('views', './views');

// Connect to MongoDB
connectDB();

// Middlewares

//  Routes
app.get('/', async(req, res) => {
    try {
        const posts = await postSchema.find().sort({ 'date': 'desc' });
        res.render('home', { result: posts });
    } catch (err) {
        res.json({ message: err });
    }

})

app.post('/post', async(req, res) => {
    console.log(req.body);
    var post = new postSchema({
        temperature: req.body.temperature,
        humidity: req.body.humidity,
        gas: req.body.gas,
        ledStatus: req.body.ledStatus
    });
    try {
        const savePost = await post.save();
        res.json(savePost);
    } catch (err) {
        res.json({ message: err });
    }
})

// Listening to the server
app.listen(process.env.PORT || 8080, () => { console.log('Connected to the server') });