import * as bodyParser from 'body-parser';
import * as ex from 'express';

const onHeaders = require('on-headers');

import {logger} from './logger';
import {errorHandler, RequestError} from './errors';

import V1_Router from './words_router_v1';

class App {
    public express: ex.Express;

    public constructor() {
        logger.info('Constructing express server');
        this.express = ex();
        this.express.use(
            (req: ex.Request, res: ex.Response, next: ex.NextFunction): void => {
                logger.info(`Connection from ${req.ip} requesting ${req.url}`);
                next();
            }
        );
        this.middleWare();
        this.mountRoutes();
        this.express.use(
            async (req: ex.Request, res: ex.Response, next: ex.NextFunction):
            Promise<void> => {
                if (res.locals.function) {
                    res.locals.function.bind(res)(await res.locals.data);
                } else {
                    res.json({
                        status: 'success',
                        data: await res.locals.data
                    });
                }
                logger.success(`Responded to ${req.ip} in ${res.locals.responseTime}`);
            }
        );
        this.errorWare();
    }

    private middleWare(): void {
        logger.debug('Loading basic middleware');
        this.express.use(bodyParser.json());
        this.express.use(bodyParser.urlencoded({extended: false}));
        this.express.use((req, res, next) => {
            const startAt = process.hrtime();
            onHeaders(res, () => {
                const diff = process.hrtime(startAt);
                const time = diff[0] * 1e3 + diff[1] * 1e-6;
                const header_name = 'X-Response-Time';
                const header_value = time.toFixed(3) + 'ms';
                res.locals.responseTime = header_value;
                if (res.getHeader(header_name)) return;
                res.setHeader(header_name, header_value);
            });
            next();
        });
    }

    private mountRoutes(): void {
        const defRouter: ex.Router = ex.Router();
        defRouter.get('/', (req: ex.Request, res: ex.Response): void => {
            res.json({
                message: 'Welcome to the WordBrush UI server!',
                important: 'Endpoints start from /api/v1/words'
            });
        });
        defRouter.all('*', (req: ex.Request, res: ex.Response, next: ex.NextFunction): void => {
            next(new RequestError(404, 'invalid_endpoint', 'No matching endpoints for url provided.'));
        });
        this.express.use('/api/v1/words', V1_Router);
        this.express.use('/', defRouter);
    }

    private errorWare(): void {
        this.express.use(errorHandler);
    }
}

export default new App().express;
