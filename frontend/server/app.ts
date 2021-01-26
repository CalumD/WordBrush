import * as bodyParser from 'body-parser';
import * as ex from 'express';
import {cors} from "./cors";
import {logger} from './logger';
import {errorHandler, RequestError} from './errors';

import V1_Router from './router/words_router_v1';
import serveFavicon = require('serve-favicon');

const onHeaders = require('on-headers');

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
        this.express.use(cors());
        this.middleWare();
        this.mountRoutes();
        this.express.use(
            async (req: ex.Request, res: ex.Response, next: ex.NextFunction):
            Promise<void> => {
                if (Object.keys(res.locals).length !== 0) {
                    res.locals.data = await res.locals.data;
                    if (res.locals.function) {
                        res.locals.function.bind(res)(await res.locals.data);
                    } else {
                        res.status(200).json(await res.locals.data);
                    }
                    if (typeof res.locals.data == 'string' && res.locals.data.length > 100) {
                        res.locals.data = res.locals.data.substr(0, 100) + ' [...]';
                    } else if (typeof res.locals.data == 'object' && res.locals.data && res.locals.data.words) {
                        const wordcount = Object.keys(res.locals.data.words).length;
                        if (wordcount > 15) {
                            res.locals.data.words = `This object contained ${wordcount} values which have been truncated for logging's sake.`
                        }
                    }
                    logger.debug('res.locals at end of MiddleWare chain', res.locals);
                    logger.success(`Responded to ${req.ip} in ${res.locals.responseTime}`);
                }
                next();
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
                important: 'Endpoints start from /api/v1'
            });
        });
        this.express.use(serveFavicon(__dirname + '/favicon.ico'));
        this.express.use('/api/v1', V1_Router);
        this.express.use('/', defRouter);
        this.express.use(
            (req: ex.Request, res: ex.Response, next: ex.NextFunction): void => {
                if (Object.keys(res.locals).length === 0) {
                    next(new RequestError({
                        code: 404,
                        name: 'invalid_endpoint',
                        description: 'No matching endpoints for url provided.',
                        data: req.url
                    }));
                } else {
                    next();
                }
            });
    }

    private errorWare(): void {
        this.express.use(errorHandler);
    }
}

export default new App().express;
