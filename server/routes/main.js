const request = require('request');
const postSchema = require('../models/postSchema');
const connectDB = require('../models/connectDB');
const express = require('express');
const route = express.Router()

// Connect to MongoDB
connectDB();

route.get('/', async (req, res) => {
    try {
        const posts = await postSchema.find().sort({ 'date': 'desc' });
        var maxTemp = -1, maxHumi = -1, maxGas = -1;
        var minTemp = 9999, minHumi = 9999, minGas = 9999;
        var sumTemp = 0, sumHumi = 0, sumGas = 0;
        posts.forEach(item => {
            // Max
            if (item.temperature > maxTemp) maxTemp = item.temperature;
            if (item.humidity > maxHumi) maxHumi = item.humidity;
            if (item.gas > maxGas) maxGas = item.gas;
            // Min
            if (item.temperature < minTemp) minTemp = item.temperature;
            if (item.humidity < minHumi) minHumi = item.humidity;
            if (item.gas < minGas) minGas = item.gas;
            // Avg
            sumTemp += item.temperature;
            sumHumi += item.humidity;
            sumGas += item.gas;
        });
        const max = {
            'maxTemp': maxTemp,
            'maxHumi': maxHumi,
            'maxGas': maxGas
        };
        const min = {
            'minTemp': minTemp,
            'minHumi': minHumi,
            'minGas': minGas
        };
        const avg = {
            'avgTemp': Math.round(sumTemp / posts.length),
            'avgHumi': Math.round(sumHumi / posts.length),
            'avgGas': Math.round(sumGas / posts.length)
        };
        res.render('home', { result: posts, max: max, min: min, avg: avg });
    } catch (err) {
        res.json({ message: err });
    }
})

route.get('/ledstatus', (req, res) => {
    console.log(req.query)
    res.send(req.query);
})

route.post('/post', async (req, res) => {
    console.log(req.body);
    // Send warning to IFTTT
    if (checkWaring(req.body)) {
        fowardIFTTT(req.body);
    }
    // Post data to mongoDB
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

// Functions
function checkWaring(data) {
    if (data.temperature > 50 || data.humidity > 80 || data.gas > 400)
        return true;
    return false;
}

function fowardIFTTT(data) {
    request.post({
        url: 'https://maker.ifttt.com/trigger/romWarning/with/key/QT4LgXT0LnBgNite87hs7',
        form:
            { value1: data.temperature, value2: data.humidity, value3: data.gas }
        ,
        method: 'POST'
    })
}

module.exports = route;