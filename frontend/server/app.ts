import * as bodyParser from 'body-parser';
import * as ex from 'express';

import {logger} from './logger';
import {errorHandler} from './errors';

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
                logger.success(`Responded to ${req.ip}`);
            }
        );
        this.errorWare();
    }

    private middleWare(): void {
        logger.debug('Loading basic middleware');
        this.express.use(bodyParser.json());
        this.express.use(bodyParser.urlencoded({extended: false}));
    }

    private mountRoutes(): void {
        const defRouter: ex.Router = ex.Router();
        defRouter.get('/', (req: ex.Request, res: ex.Response): void => {
            res.json({
                message: 'Welcome to the WordBrush UI server!',
                important: 'Endpoints start from /api/v1'
            });
        });
        this.express.use('/api/v1', V1_Router);
        this.express.use('/', defRouter);
        this.express.use(
            (req: ex.Request, res: ex.Response, next: ex.NextFunction): void => {
                if (Object.keys(res.locals).length === 0) {
                    res.status(404)
                        .json({
                            status: 'error',
                            error: 'invalid_route',
                            error_description: 'Endpoints start from /api/v1'
                        });
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
