const mongoDB = require('mongoose');

const schema = mongoDB.Schema({
    date: {
        type: Date,
        default: Date.now
    },
    temperature: {
        type: Number,
        required: true
    },
    humidity: {
        type: Number,
        required: true
    },
    gas: {
        type: Number,
        required: true
    },
    ledStatus: {
        type: String,
        required: true
    },
});

module.exports = mongoDB.model('posts', schema);