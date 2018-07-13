// https://stackoverflow.com/questions/20807900/connecting-to-an-already-established-unix-socket-with-node-js
// function cConnect(data){
	// return new Promise(function(resolve, reject){
	// 	var net = require('net');
	// 	var response = {};

	// 	var client = net.createConnection("/tmp/icp-test");
		

	// 	client.on("connect", function() {
	// 		console.log('Connected to server!');
	// 	    client.write(data + "\n");
	// 	});

	// 	client.on("data", function(data) {
	// 		console.log("Server Answered: ");
	// 		response.CServer = data.toString();

	// 		console.log(response.CServer);
	// 		client.end();
	// 	});

	// 	client.on('end', () => {
	// 		console.log('Disconnected from Server');
	// 		resolve(response.CServer);
	// 	});

	// 	client.on('error', function(error) {
	// 		console.log("Error: " + error.message);
	// 		reject(error.message);
	// 	});

		
	// });
// }

module.exports = {
	cConnect: function(data){
		return new Promise(function(resolve, reject){
			var net = require('net');
			var response = {};

			var client = net.createConnection("/tmp/icp-test");
			

			client.on("connect", function() {
				console.log('Connected to server!');
			    client.write(data + "\n");
			});

			client.on("data", function(data) {
				console.log("Server Answered: ");
				response.CServer = data.toString();

				console.log(response.CServer);
				client.end();
			});

			client.on('end', () => {
				console.log('Disconnected from Server');
				resolve(response.CServer);
			});

			client.on('error', function(error) {
				console.log("Error: " + error.message);
				reject(error.message);
			});

			
		});
	}
}