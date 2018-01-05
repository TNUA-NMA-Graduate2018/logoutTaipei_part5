var http = require('http');
var req = http.request({
	hostname: 'www.google.com',
	path: '/',
	method: 'GET'
},function(res){
	//receive data
	res.on('data',function(chunk){
		console.log(chunk.toString());
	});
	// finish program
	res.on('end',function(chunk){
		process.exit();
	});
});
req.end();

