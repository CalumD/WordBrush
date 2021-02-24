import {logger} from '../logger';
import {NextFunction, Request, Response, Router} from 'express';
import * as bodyParser from 'body-parser';
import {RequestError} from '../errors';
import {uuid} from '../uuid';
import * as wb_inter from "../logic/wordbrush_interface";
import {addToCache, tryCache} from '../logic/request_cache';
import {FileFilterCallback} from "multer";

const multer = require('multer');

export type Middleware =
    (req: Request, res: Response, next: NextFunction) => void;


const getWords: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    const cacheKey = tryCache(req, res);
    if (cacheKey === 'HIT') {
        logger.debug("Skipping new getWords() as cache HIT.")
        return next();
    }

    res.locals.methodCalled = getWords.name;
    logger.follow(`Called ${res.locals.methodCalled}`);

    res.locals.data = await wb_inter.getWords({
        wbArgs: {
            width: req.query.w ? Number(req.query.w) : undefined,
            height: req.query.h ? Number(req.query.h) : undefined,
            sfo: req.query.sfo ? Number(req.query.sfo) : undefined,
            keyboard: req.query.k ? (<string>req.query.k) === 'true' : undefined,
            words: req.query.input ? (<string>req.query.input).replace(/[,;|>&-]/g, ' ') : undefined,
            hasInputFile: !!req.file
        },
        file: req.file,
        next: next
    });

    addToCache({
        hash: cacheKey,
        url: req.originalUrl,
        fileName: req.file ? req.file.originalname : undefined,
        outputDirectory: res.locals.data.resultSetID
    });

    next();
}

const getExistingResults: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    res.locals.methodCalled = getExistingResults.name;
    logger.follow(`Called ${res.locals.methodCalled}`);

    res.locals.data = wb_inter.getExistingResults();

    next();
}

const getResultSet: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    res.locals.methodCalled = getResultSet.name;
    logger.follow(`Called ${res.locals.methodCalled}`);

    res.locals.data = wb_inter.getResultSet({directory: req.params.resultSet, next: next});

    next();
}

const getOutput: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    res.locals.methodCalled = getOutput.name;
    logger.follow(`Called ${res.locals.methodCalled}`);

    res.locals.function = res.send;
    res.setHeader('content-type', "image/svg+xml");
    res.locals.data = wb_inter.getOutput({file: req.params.output, directory: req.params.resultSet, next: next});

    next();
}

const getMethods: Middleware = async (
    req: Request, res: Response, next: NextFunction
): Promise<void> => {
    res.locals.methodCalled = getMethods.name;
    logger.follow(`Called ${res.locals.methodCalled}`);

    res.locals.data = {
        endpoints: routerRoutes
    };

    next();
}


const badQueryParam: Function = (description: string, message?: string, data?: any): void | RequestError => {
    return new RequestError({
        code: 400,
        name: 'bad_query_param',
        description: description,
        message: message,
        data: data
    });
}

export class WordsRouterV1 {

    public router: Router;

    public constructor() {
        this.router = Router();
        this.precondition();

        // CLI only params
        this.router.get('/words', this.validateQueryParamsRequired, this.validateQueryParamsOptional, getWords);

        // Input File in addition to CLI
        this.router.post('/words',
            bodyParser.raw({limit: '10MB'}),
            multer({
                storage: multer.memoryStorage(),
                fileFilter: function (req: Request, file: any, cb: FileFilterCallback) {
                    if (file.originalname.endsWith(".txt") && file.mimetype.startsWith("text/")) {
                        return cb(null, true);
                    }
                    return cb(null, false);
                },
                limit: {fileSize: 10485760} // 10MiB
            }).single('input_txt_file'),
            this.validateQueryParamsOptional,
            getWords
        );

        // A result set
        this.router.get('/results', getExistingResults);
        this.router.get('/results/:resultSet', getResultSet);
        this.router.get('/results/:resultSet/:output', getOutput);

        this.router.get('', getMethods);
    }

    private static urlParamIntegerMatcher(input: string, forParam: string): undefined | RequestError {
        if (!input.match(/^[0-9]+$/)) {
            return badQueryParam(`URL parameter for ${forParam} expected an integer, received non-integer input.`, 'invalid_size_param', input);
        }
        return undefined;
    }

    private static urlParamBooleanMatcher(input: string, forParam: string): undefined | RequestError {
        if (input !== 'true' && input !== 'false') {
            return badQueryParam(`URL parameter for ${forParam} expected a boolean, received non-bool input.`, 'invalid_flag_param', input);
        }
        return undefined;
    }

    private static urlParamUUIDMatcher(input: string, forParam: string): undefined | RequestError {
        if (!uuid.verify(input)) {
            return badQueryParam(`URL parameter for ${forParam} expected a valid uuid.`, 'invalid_uuid', input);
        }
        return undefined;
    }

    private static urlParamFilenameMatcher(input: string, forParam: string): undefined | RequestError {
        if (!input.match(/^([0-9]+)(\.svg)?$/)) {
            return badQueryParam(`URL parameter for ${forParam} expected /^([0-9]+)(\\.svg)?$/.`, 'invalid_filename', input);
        }
        return undefined;
    }

    private static integerBoundChecker(input: Number, forParam: string, minBound: Number, maxBound: Number): undefined | RequestError {
        if (input < minBound || input > maxBound) {
            return badQueryParam(`URL parameter for ${forParam} expected to be (${minBound} <= x <= ${maxBound}).`, 'invalid_size_param', input);
        }
        return undefined;
    }

    private validateQueryParamsRequired: Middleware = (
        req: Request, res: Response, next: NextFunction
    ): void => {
        logger.trace('Called validateQueryParamsRequired')
        if (!req.query.input) {
            return next(new RequestError({
                code: 400,
                name: 'missing_query_param',
                message: 'Bad Search Data.',
                description: 'Required query param (input) was missing from the request.'
            }));
        }
        next();
    }

    private validateQueryParamsOptional: Middleware = (
        req: Request, res: Response, next: NextFunction
    ): void => {
        logger.trace('Called validateQueryParamsOptional')
        if (req.query.w) {
            let potentialError = WordsRouterV1.urlParamIntegerMatcher(<string>req.query.w, 'width');
            if (potentialError) {
                return next(potentialError);
            }
            potentialError = WordsRouterV1.integerBoundChecker(+req.query.w, 'width', 50, 3840);
            if (potentialError) {
                return next(potentialError);
            }
        }
        if (req.query.h) {
            let potentialError = WordsRouterV1.urlParamIntegerMatcher(<string>req.query.h, 'height');
            if (potentialError) {
                return next(potentialError);
            }
            potentialError = WordsRouterV1.integerBoundChecker(+req.query.h, 'height', 50, 3840);
            if (potentialError) {
                return next(potentialError);
            }
        }
        if (req.query.sfo) {
            let potentialError = WordsRouterV1.urlParamIntegerMatcher(<string>req.query.sfo, 'sfo');
            if (potentialError) {
                return next(potentialError);
            }
            potentialError = WordsRouterV1.integerBoundChecker(+req.query.sfo, 'sfo', 1, 100);
            if (potentialError) {
                return next(potentialError);
            }
        }
        if (req.query.k) {
            let potentialError = WordsRouterV1.urlParamBooleanMatcher(<string>req.query.k, 'keyboard');
            if (potentialError) {
                return next(potentialError);
            }
        }
        next();
    }

    private precondition(): void {
        this.router.param(
            'resultSet',
            async (
                req: Request, res: Response, next: NextFunction,
                results: string
            ): Promise<void> => {
                let potentialError = WordsRouterV1.urlParamUUIDMatcher(results, 'resultSet');
                if (potentialError) {
                    return next(potentialError);
                }
                next();
            }
        );
        this.router.param(
            'output',
            async (
                req: Request, res: Response, next: NextFunction,
                output: string
            ): Promise<void> => {
                let potentialError = WordsRouterV1.urlParamFilenameMatcher(output, 'output');
                if (potentialError) {
                    return next(potentialError);
                }
                next();
            }
        );
    }
}

const routerRoutes: { [path: string]: { methods: string[] } } = {};
new WordsRouterV1().router.stack.forEach((route) => {
    if (routerRoutes[`api/v1${route.route.path}`]) {
        Object.keys(route.route.methods).forEach((key: string) => {
            if (!routerRoutes[`api/v1${route.route.path}`].methods.includes(key)) {
                routerRoutes[`api/v1${route.route.path}`].methods.push(key)
            }
        })
    } else {
        routerRoutes[`api/v1${route.route.path}`] = {methods: Object.keys(route.route.methods)}
    }
});

export default new WordsRouterV1().router;
