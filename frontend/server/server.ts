import {default as app} from './app';
import {logger} from './logger';

let port = process.env.PORT || 3000;
app.listen(port, (err: Error) => {
    if (err) {
        return logger.critical(`Error on server startup: ${err}`);
    } else {
        return logger.success(`Server started on: http://localhost:${port}`);
    }
});

