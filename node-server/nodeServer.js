const express = require ("express");
const bodyParser = require("body-parser");
// const routes = require("./routes/api");

//Set up express app
const app = express();

// Must be before routes
app.use(bodyParser.json());

//Initialize routes
app.use("/", require("./routes/api"));

//Listen for requests
app.listen(process.env.port || 4000, function(){
	console.log("Now listening for requests: ");
});