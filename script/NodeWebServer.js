
if (process.argv.length == 2)
{
	console.log("ERROR: You must specify the directory to host files from!");
}
else
{
	console.log(process.argv[2]);
	const express = require('express');
	const app = express();
	app.use(express.static(process.argv[2]));
	app.listen(8000, () => console.log('Serving at http://localhost:8000!'));

}
