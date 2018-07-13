const express = require("express");
const router = express.Router();
const myModule = require('../functions');

// Route handlers
//Recieve user signal
router.post("/notification", function(req, res){
	console.log("Message from website: \n"+req.body.info+"\n");

	var promise = myModule.cConnect(req.body.info);

	// First do promise, afterwards send data to user
	promise.then(function(Cresponse){
		res.send({
			type: 'POST',
			response: JSON.stringify(Cresponse)
		});
	}).catch(function(error){
		console.log(error);
	});
});

// Export router
module.exports = router;

// 


