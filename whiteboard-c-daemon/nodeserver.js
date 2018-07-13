// https://expressjs.com/en/starter/installing.html
// https://stackoverflow.com/questions/45105992/node-js-send-data-to-backend-with-ajax


// const express = require('express');
// const bodyParser = require('body-parser');
// // const dir = path.join(__dirname, 'public');

// const app = express();

// app.use(bodyParser.json);
// // app.use(bodyParser.urlencoded({ extended: true }));


// app.post('http://localhost:3000/whiteboard', function(req, res){
// 	// var obj = {};
// 	console.log('body: ' + JSON.stringify(req.body));
// 	// res.send(req.body);
// });

// // app.listen(3000);
// app.listen(3000, () => console.log('Listening on localhost:3000/whiteboard'));


var express = require('express');
var app = express();
var router = express.Router();
var bodyParser = require('body-parser');

app.use(bodyParser.json());

app.use(express.static('public'));

router.use(function (req,res,next) {
  console.log("/" + req.method);
  next();
});


app.post('/whiteboard-tcpserver', function(req, res) {
   console.log(req.body);
});

app.use("/",router);
 
app.use("*",function(req,res){
  res.sendFile(path + "404.html");
});

app.listen(8080, () => console.log('Listening'));






