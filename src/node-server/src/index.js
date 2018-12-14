const express = require('express');
const bodyParser = require('body-parser');
const loadAssets = require('./load-assets');
const assignRoutes = require('./assign-routes');

const PORT = process.env.PORT || 3000;

const startApp = async () => {
  const app = express();
  app.use(bodyParser.urlencoded({
    extended: true
  }));
  app.use(bodyParser.json());
  const assets = await loadAssets();

  assignRoutes(app, assets);
  // Start the server with the specified port: 
  app.listen(PORT, (err) => {
    if (err) return Promise.reject(err);
    return Promise.resolve();
  });
}

startApp().then(() => console.log(` Server is running on port ${ PORT} `)).catch(err => console.error(` An error occurred: ${ err} `));