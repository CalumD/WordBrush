import { logger } from './logger';

var express = require('express'),
    app = express(),
    port = process.env.PORT || 3000;
app.listen(port);

logger.success('Server started on: http://localhost:' + port);

