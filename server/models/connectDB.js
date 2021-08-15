const mongoDB = require('mongoose');
require('dotenv/config')

const connectDB = async function()
{
    await mongoDB.connect(process.env.DB_URL, { useNewUrlParser: true, useUnifiedTopology: true },  () => {
        console.log('Connected to the database');
    });
}

module.exports = connectDB;