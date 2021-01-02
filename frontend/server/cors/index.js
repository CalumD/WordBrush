const corser = require('corser');
const cors = corser.create.bind(null, {
	requestHeaders: corser.simpleRequestHeaders.concat([
		'Authorisation',
		'Authorization'
	])
});
module.exports = { cors: cors };
